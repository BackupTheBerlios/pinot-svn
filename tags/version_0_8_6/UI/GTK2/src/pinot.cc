/*
 *  Copyright 2005,2006 Fabrice Colin
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <libintl.h>
#include <getopt.h>
#include <strings.h>
#include <iostream>
#include <fstream>
#include <glibmm.h>
#include <glibmm/thread.h>
#include <glibmm/ustring.h>
#include <glibmm/miscutils.h>
#include <glibmm/convert.h>
#include "config.h"
extern "C"
{
#if DBUS_NUM_VERSION < 1000000
#define DBUS_API_SUBJECT_TO_CHANGE
#endif
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-lowlevel.h>
}
#include <gtkmm/main.h>

#include "NLS.h"
#include "FilterFactory.h"
#include "Languages.h"
#include "MIMEScanner.h"
#include "ModuleFactory.h"
#include "ActionQueue.h"
#include "QueryHistory.h"
#include "ViewHistory.h"
#include "DownloaderInterface.h"
#include "PinotSettings.h"
#include "mainWindow.hh"

using namespace std;

static ofstream g_outputFile;
static streambuf *g_coutBuf = NULL;
static streambuf *g_cerrBuf = NULL;
static struct option g_longOptions[] = {
	{"help", 0, 0, 'h'},
	{"version", 0, 0, 'v'},
	{0, 0, 0, 0}
};

static void closeAll(void)
{
	cout << "Exiting..." << endl;

	// Save the settings
	PinotSettings &settings = PinotSettings::getInstance();
	if (settings.save() == false)
	{
		cerr << "Couldn't save configuration file" << endl;
	}

	// Close everything
	ModuleFactory::unloadModules();
	Dijon::FilterFactory::unloadFilters();
	Dijon::HtmlFilter::shutdown();

	// Restore the stream buffers
	if (g_coutBuf != NULL)
	{
		cout.rdbuf(g_coutBuf);
	}
	if (g_cerrBuf != NULL)
	{
		cerr.rdbuf(g_cerrBuf);
	}
	g_outputFile.close();

	DownloaderInterface::shutdown();
	MIMEScanner::shutdown();
}

static void quitAll(int sigNum)
{
	cout << "Quitting..." << endl;

	Gtk::Main::quit();
}

int main(int argc, char **argv)
{
	string prefixDir(PREFIX);
	Glib::ustring errorMsg;
	struct sigaction newAction;
	int longOptionIndex = 0;
	bool warnAboutVersion = false;

	// Look at the options
	int optionChar = getopt_long(argc, argv, "hv", g_longOptions, &longOptionIndex);
	while (optionChar != -1)
	{
		switch (optionChar)
		{
			case 'h':
				// Help
				cout << "pinot - A metasearch tool for the Free Desktop\n\n"
					<< "Usage: pinot [OPTIONS]\n\n"
					<< "Options:\n"
					<< "  -h, --help		display this help and exit\n"
					<< "  -v, --version		output version information and exit\n"
					<< "\nReport bugs to " << PACKAGE_BUGREPORT << endl;
				return EXIT_SUCCESS;
			case 'v':
				cout << "pinot - " << PACKAGE_STRING << "\n\n" 
					<< "This is free software.  You may redistribute copies of it under the terms of\n"
					<< "the GNU General Public License <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>.\n"
					<< "There is NO WARRANTY, to the extent permitted by law." << endl;
				return EXIT_SUCCESS;
			default:
				return EXIT_FAILURE;
		}

		// Next option
		optionChar = getopt_long(argc, argv, "hv", g_longOptions, &longOptionIndex);
	}

#if defined(ENABLE_NLS)
	bindtextdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);
#endif //ENABLE_NLS

	// Initialize threads support before doing anything else
	if (Glib::thread_supported() == false)
	{
		Glib::thread_init();
	}
	// Initialize the GType and the D-Bus thread system
	g_type_init();
#if DBUS_NUM_VERSION > 1000000
	dbus_threads_init_default();
#endif
	dbus_g_thread_init();

	Gtk::Main m(&argc, &argv);
	Glib::set_application_name("Pinot GTK2 UI");

	// This should make Xapian use Flint rather than Quartz
	Glib::setenv("XAPIAN_PREFER_FLINT", "1");
	// This will be useful for indexes served by xapian-progsrv+ssh
	Glib::setenv("SSH_ASKPASS", prefixDir + "/libexec/openssh/ssh-askpass");

	// This is a hack to force the locale to UTF-8
	char *pLocale = setlocale(LC_ALL, NULL);
	if (pLocale != NULL)
	{
		string locale(pLocale);

		if (locale != "C")
		{
			bool appendUTF8 = false;

			string::size_type pos = locale.find_last_of(".");
			if ((pos != string::npos) &&
				((strcasecmp(locale.substr(pos).c_str(), ".utf8") != 0) &&
				(strcasecmp(locale.substr(pos).c_str(), ".utf-8") != 0)))
			{
				locale.resize(pos);
				appendUTF8 = true;
			}

			if (appendUTF8 == true)
			{
				locale += ".UTF-8";

				pLocale = setlocale(LC_ALL, locale.c_str());
				if (pLocale != NULL)
				{
#ifdef DEBUG
					cout << "Changed locale to " << pLocale << endl;
#endif
				}
			}
		}
	}

	// This will create the necessary directories on the first run
	PinotSettings &settings = PinotSettings::getInstance();
	// Talk to the daemon through DBus
	settings.enableClientMode(true);

	string confDirectory = PinotSettings::getConfigurationDirectory();
	if (chdir(confDirectory.c_str()) == 0)
	{
		// Redirect cout and cerr to a file
		string logFileName = confDirectory;
		logFileName += "/pinot.log";
		g_outputFile.open(logFileName.c_str());
		g_coutBuf = cout.rdbuf();
		g_cerrBuf = cerr.rdbuf();
		cout.rdbuf(g_outputFile.rdbuf());
		cerr.rdbuf(g_outputFile.rdbuf());
	}

	// Initialize utility classes
	if (MIMEScanner::initialize(PinotSettings::getHomeDirectory() + "/.local",
		string(SHARED_MIME_INFO_PREFIX)) == false)
	{
		cerr << "Couldn't load MIME settings" << endl;
	}
	DownloaderInterface::initialize();
	// Load tokenizer libraries, if any
	Dijon::HtmlFilter::initialize();
	Dijon::FilterFactory::loadFilters(string(LIBDIR) + "/pinot/filters");
	Dijon::FilterFactory::loadFilters(confDirectory + "/filters");
	// Load backends, if any
	ModuleFactory::loadModules(string(LIBDIR) + "/pinot/backends");
	ModuleFactory::loadModules(confDirectory + "/backends");

	// Localize language names
	Languages::setIntlName(0, _("Unknown"));
	Languages::setIntlName(1, _("Danish"));
	Languages::setIntlName(2, _("Dutch"));
	Languages::setIntlName(3, _("English"));
	Languages::setIntlName(4, _("Finnish"));
	Languages::setIntlName(5, _("French"));
	Languages::setIntlName(6, _("German"));
	Languages::setIntlName(7, _("Hungarian"));
	Languages::setIntlName(8, _("Italian"));
	Languages::setIntlName(9, _("Norwegian"));
	Languages::setIntlName(10, _("Portuguese"));
	Languages::setIntlName(11, _("Romanian"));
	Languages::setIntlName(12, _("Russian"));
	Languages::setIntlName(13, _("Spanish"));
	Languages::setIntlName(14, _("Swedish"));
	Languages::setIntlName(15, _("Turkish"));

	// Load search engines
	settings.loadSearchEngines(prefixDir + "/share/pinot/engines");
	settings.loadSearchEngines(confDirectory + "/engines");
	// Load the settings
	settings.loadGlobal(string(SYSCONFDIR) + "/pinot/globalconfig.xml");
	settings.load();

	// Catch interrupts
	sigemptyset(&newAction.sa_mask);
	newAction.sa_flags = 0;
	newAction.sa_handler = quitAll;
	sigaction(SIGINT, &newAction, NULL);
	sigaction(SIGQUIT, &newAction, NULL);
	sigaction(SIGTERM, &newAction, NULL);

	// Open this index read-write
	bool wasObsoleteFormat = false;
	if (ModuleFactory::openOrCreateIndex(settings.m_defaultBackend, settings.m_docsIndexLocation, wasObsoleteFormat, false) == false)
	{
		errorMsg = _("Couldn't open index");
		errorMsg += " ";
		errorMsg += settings.m_docsIndexLocation;
	}
	else
	{
		warnAboutVersion = wasObsoleteFormat;
	}
	// ...and the daemon index in read-only mode
	// If it can't be open, it just means the daemon has not yet created it
	ModuleFactory::openOrCreateIndex(settings.m_defaultBackend, settings.m_daemonIndexLocation, wasObsoleteFormat, true);
	// Merge these two, this will be useful later
	ModuleFactory::mergeIndexes(settings.m_defaultBackend, "MERGED", settings.m_docsIndexLocation, settings.m_daemonIndexLocation);

	// Do the same for the history database
	string historyDatabase(settings.getHistoryDatabaseName());
	if ((historyDatabase.empty() == true) ||
		(ActionQueue::create(historyDatabase) == false) ||
		(QueryHistory::create(historyDatabase) == false) ||
		(ViewHistory::create(historyDatabase) == false))
	{
		errorMsg = _("Couldn't create history database");
		errorMsg += " ";
		errorMsg += historyDatabase;
	}
	else
	{
		ActionQueue actionQueue(historyDatabase, Glib::get_prgname());
		QueryHistory queryHistory(historyDatabase);
		ViewHistory viewHistory(historyDatabase);
		time_t timeNow = time(NULL);

		// Expire all actions left from last time
		actionQueue.expireItems(timeNow);
		// Expire items older than a month
		queryHistory.expireItems(timeNow - 2592000);
		viewHistory.expireItems(timeNow - 2592000);
	}

	atexit(closeAll);

	IndexInterface *pIndex = settings.getIndex(settings.m_docsIndexLocation);
	if (pIndex != NULL)
	{
		string indexVersion(pIndex->getMetadata("version"));

		// What version is the index at ?
		if (indexVersion.empty() == true)
		{
			indexVersion = "0.0";
		}
		// Is an upgrade necessary ?
		if ((indexVersion < PINOT_INDEX_MIN_VERSION) &&
			(pIndex->getDocumentsCount() > 0))
		{
			warnAboutVersion = true;
		}
#ifdef DEBUG
		cout << "My Web Pages was set to version " << indexVersion << endl;
#endif
		pIndex->setMetadata("version", VERSION);

		delete pIndex;
	}
	if (warnAboutVersion == true)
	{
		settings.m_warnAboutVersion = warnAboutVersion;
	}

	try
	{
		// Set an icon for all windows
		Gtk::Window::set_default_icon_from_file(prefixDir + "/share/icons/hicolor/48x48/apps/pinot.png");

		// Create and open the main dialog box
		mainWindow mainBox;
		if (errorMsg.empty() == false)
		{
			mainBox.set_status(errorMsg);
		}
		m.run(mainBox);
	}
	catch (const Glib::Exception &e)
	{
		cerr << e.what() << endl;
		return EXIT_FAILURE;
	}
	catch (const char *pMsg)
	{
		cerr << pMsg << endl;
		return EXIT_FAILURE;
	}
	catch (...)
	{
		cerr << "Unknown exception" << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
