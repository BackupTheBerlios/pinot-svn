# Process this file with automake to produce Makefile.in

noinst_HEADERS = \
	mainWindow_glade.hh \
	mainWindow.hh \
	prefsDialog_glade.hh \
	prefsDialog.hh \
	queryDialog_glade.hh \
	queryDialog.hh \
	importDialog_glade.hh \
	importDialog.hh \
	propertiesDialog_glade.hh \
	propertiesDialog.hh \
	indexDialog_glade.hh \
	indexDialog.hh \
	DaemonState.h \
	EnginesTree.h \
	IndexPage.h \
	IndexTree.h \
	MboxHandler.h \
	ModelColumns.h \
	Notebook.h \
	OnDiskHandler.h \
	PinotSettings.h \
	PinotUtils.h \
	ResultsTree.h \
	WorkerThreads.h

pinot_SOURCES = \
	pinot.cc \
	mainWindow_glade.cc \
	mainWindow.cc \
	prefsDialog_glade.cc \
	prefsDialog.cc \
	queryDialog_glade.cc \
	queryDialog.cc \
	importDialog_glade.cc \
	importDialog.cc \
	propertiesDialog_glade.cc \
	propertiesDialog.cc \
	indexDialog_glade.cc \
	indexDialog.cc \
	EnginesTree.cpp \
	IndexPage.cpp \
	IndexTree.cpp \
	MboxHandler.cpp \
	ModelColumns.cpp \
	Notebook.cpp \
	PinotSettings.cpp \
	PinotUtils.cpp \
	ResultsTree.cpp \
	WorkerThreads.cpp

pinot_dbus_daemon_SOURCES = \
	pinot-dbus-daemon.cc \
	DaemonState.cpp \
	MboxHandler.cpp \
	OnDiskHandler.cpp \
	PinotSettings.cpp \
	WorkerThreads.cpp

bin_PROGRAMS = pinot pinot-dbus-daemon

AM_CXXFLAGS = -I$(top_srcdir)/Utils -I$(top_srcdir)/Tokenize -I$(top_srcdir)/SQL \
	-I$(top_srcdir)/Monitor -I$(top_srcdir)/Collect -I$(top_srcdir)/Index \
	-I$(top_srcdir)/Search -I$(top_srcdir)/Search/Google \
	@SQL_CFLAGS@ @HTTP_CFLAGS@ @MIME_CFLAGS@ @XML_CFLAGS@ @SOAP_CFLAGS@ @DBUS_CFLAGS@ @INDEX_CFLAGS@ @UI_CFLAGS@

pinot_LDADD = -L$(top_srcdir)/Utils -L$(top_srcdir)/Tokenize -L$(top_srcdir)/SQL -L$(top_srcdir)/Monitor \
	-L$(top_srcdir)/Collect -L$(top_srcdir)/Index -L$(top_srcdir)/Search -L$(top_srcdir)/Search/Google \
	@SEARCH_LIBS@ -lIndex -lMonitor -lCollect -lSQL -lTokenize -lUtils -lBasicUtils \
	@LIBS@ @UI_LIBS@ @INDEX_LIBS@ @DBUS_LIBS@ @SOAP_LIBS@ @XML_LIBS@ @MIME_LIBS@ @HTTP_LIBS@ @SQL_LIBS@ @MISC_LIBS@

pinot_dbus_daemon_LDADD = -L$(top_srcdir)/Utils -L$(top_srcdir)/Tokenize -L$(top_srcdir)/SQL -L$(top_srcdir)/Monitor \
	-L$(top_srcdir)/Collect -L$(top_srcdir)/Index -L$(top_srcdir)/Search -L$(top_srcdir)/Search/Google \
	@SEARCH_LIBS@ -lIndex -lMonitor -lCollect -lSQL -lTokenize -lUtils -lBasicUtils \
	@LIBS@ @GLIBMM_LIBS@ @INDEX_LIBS@ @DBUS_LIBS@ @SOAP_LIBS@ @XML_LIBS@ @MIME_LIBS@ @HTTP_LIBS@ @SQL_LIBS@ @MISC_LIBS@

