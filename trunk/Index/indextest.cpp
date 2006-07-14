/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
#include <getopt.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>

#include "MIMEScanner.h"
#include "Url.h"
#include "XapianDatabaseFactory.h"
#include "TokenizerFactory.h"
#include "DownloaderFactory.h"
#include "XapianIndex.h"
#include "config.h"

using namespace std;

static struct option g_longOptions[] = {
	{"check", 1, 0, 'c'},
	{"help", 0, 0, 'h'},
	{"index", 1, 0, 'i'},
	{"stats", 0, 0, 's'},
	{"version", 0, 0, 'v'},
	{0, 0, 0, 0}
};

int main(int argc, char **argv)
{
	string type, option;
	int longOptionIndex = 0;
	string parameter;
	bool checkDocument = false, indexDocument = false, statIndex = false, success = false;

	// Look at the options
	int optionChar = getopt_long(argc, argv, "hv", g_longOptions, &longOptionIndex);
	while (optionChar != -1)
	{
		set<string> engines;

		switch (optionChar)
		{
			case 'c':
				if (optarg != NULL)
				{
					parameter = optarg;
				}
				checkDocument = true;
				break;
			case 'h':
				// Help
				cout << "pinot-index - Query search engines from the command-line\n\n"
					<< "Usage: pinot-search [OPTIONS] INDEXTYPE INDEXLOCATION\n\n"
					<< "Options:\n"
					<< "  -c, --check		check whether the given URL is in the index\n"
					<< "  -h, --help		display this help and exit\n"
					<< "  -i, --index		index the given URL\n"
					<< "  -s, --stats		print statistics about the index (default option)\n"
					<< "  -v, --version		output version information and exit\n\n";
				cout << "\n\nExamples:\n"
					<< "pinot-index --check http://pinot.berlios.de/ xapian ~/.pinot/index\n\n"
					<< "pinot-index --stats xapian ~/.pinot/index\n\n"
					<< "pinot-index --index http://pinot.berlios.de/ xapian ~/.pinot/index\n\n"
					<< "Report bugs to " << PACKAGE_BUGREPORT << endl;
				return EXIT_SUCCESS;
			case 'i':
				if (optarg != NULL)
				{
					parameter = optarg;
				}
				indexDocument = true;
				break;
			case 's':
				if (optarg != NULL)
				{
					parameter = optarg;
				}
				statIndex = true;
				break;
			case 'v':
				cout << "pinot-index - " << PACKAGE_STRING << "\n\n"
					<< "This is free software.  You may redistribute copies of it under the terms of\n"
					<< "the GNU General Public License <http://www.gnu.org/licenses/gpl.html>.\n"
					<< "There is NO WARRANTY, to the extent permitted by law." << endl;
				return EXIT_SUCCESS;
			default:
				return EXIT_FAILURE;
		}

		// Next option
		optionChar = getopt_long(argc, argv, "hv", g_longOptions, &longOptionIndex);
	}

	if (argc < 3)
	{
		cerr << "Not enough parameters" << endl;
		return EXIT_FAILURE;
	}

	MIMEScanner::initialize();
	DownloaderInterface::initialize();

	// FIXME: do stuff
	XapianIndex index(argv[2]);

	if (checkDocument == true)
	{
		if (index.isGood() == true)
		{
			unsigned int docId = index.hasDocument(parameter);
			if (docId > 0)
			{
				cout << parameter << ": document ID " << docId << endl;
				success = true;
			}
		}
	}
	if (indexDocument == true)
	{
		Url thisUrl(parameter);

		// Which Downloader ?
		DownloaderInterface *pDownloader = DownloaderFactory::getDownloader(thisUrl.getProtocol());
		if (pDownloader == NULL)
		{
			cerr << "Couldn't obtain downloader for protocol " << thisUrl.getProtocol() << endl;

			XapianDatabaseFactory::closeAll();
			DownloaderInterface::shutdown();
			MIMEScanner::shutdown();

			return EXIT_FAILURE;
		}

		DocumentInfo docInfo(parameter, parameter, MIMEScanner::scanUrl(thisUrl), "");
		Document *pDoc = pDownloader->retrieveUrl(docInfo);
		if (pDoc == NULL)
		{
			cerr << "Download operation failed !" << endl;
		}
		else
		{
			// Tokenize this document
			Tokenizer *pTokens = TokenizerFactory::getTokenizerByType(docInfo.getType(), pDoc);
			if (pTokens != NULL)
			{
				set<string> labels;

				index.setStemmingMode(IndexInterface::STORE_BOTH);

				// Update an existing document or add to the index ?
				unsigned int docId = index.hasDocument(parameter);
				if (docId > 0)
				{
					// Update the document
					if (index.updateDocument(docId, *pTokens) == true)
					{
						success = true;
					}
				}
				else
				{
					// Index the document
					success = index.indexDocument(*pTokens, labels, docId);
				}

				if (success == true)
				{
					// Flush the index
					index.flush();
				}

				delete pTokens;
			}

			delete pDoc;
		}

		delete pDownloader;
	}
	if (statIndex == true)
	{
		if (index.isGood() == true)
		{
			success = true;
		}
	}

	XapianDatabaseFactory::closeAll();
	DownloaderInterface::shutdown();
	MIMEScanner::shutdown();

	// Did whatever operation we carried out succeed ?
	if (success == true)
	{
		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}
