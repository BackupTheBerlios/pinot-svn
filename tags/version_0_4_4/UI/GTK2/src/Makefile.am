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
	EnginesTree.h \
	IndexPage.h \
	ModelColumns.h \
	Notebook.h \
	PinotUtils.h \
	WorkerThreads.h \
	HtmlView.h \
	IndexTree.h \
	MonitorHandler.h \
	PinotSettings.h \
	ResultsTree.h

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
	ModelColumns.cpp \
	Notebook.cpp \
	PinotUtils.cpp \
	WorkerThreads.cpp \
	HtmlView.cpp \
	IndexTree.cpp \
	MonitorHandler.cpp \
	PinotSettings.cpp \
	ResultsTree.cpp

bin_PROGRAMS = pinot

AM_CXXFLAGS = -I../../../Utils -I../../../Tokenize -I../../../SQL \
	-I../../../Collect -I../../../Search -I../../../Index -I../../RenderHTML \
	@SQL_CFLAGS@ @HTTP_CFLAGS@ @MIME_CFLAGS@ @XML_CFLAGS@ @INDEX_CFLAGS@ @UI_CFLAGS@

pinot_LDADD = -L../../../Utils -L../../../Tokenize -L../../../SQL \
        -L../../../Collect -L../../../Search -L../../../Index -L../../RenderHTML \
	-lHTML -lIndex -lSearch -lCollect -lSQL -lTokenize -lUtils \
	@LIBS@ @UI_LIBS@ @INDEX_LIBS@ @XML_LIBS@ @MIME_LIBS@ @HTTP_LIBS@ @SQL_LIBS@ @MISC_LIBS@

