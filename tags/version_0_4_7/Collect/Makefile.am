# Process this file with automake to produce Makefile.in

noinst_HEADERS = \
	@HTTP_DOWNLOADER@.h \
	DownloaderFactory.h \
	DownloaderInterface.h \
	FileCollector.h \
	MboxCollector.h \
	XapianCollector.h

noinst_LTLIBRARIES = libCollect.la

bin_PROGRAMS = pinot-collect

pinot_collect_SOURCES = \
	pinot-collect.cpp

pinot_collect_LDADD = -L../Utils -L../Tokenize \
	-lCollect -lTokenize -lUtils \
	@INDEX_LIBS@ @XML_LIBS@ @MIME_LIBS@ @HTTP_LIBS@ @MISC_LIBS@

libCollect_la_SOURCES = \
	@HTTP_DOWNLOADER@.cpp \
	DownloaderFactory.cpp \
	DownloaderInterface.cpp \
	FileCollector.cpp \
	MboxCollector.cpp \
	XapianCollector.cpp

AM_CXXFLAGS = -I../Utils -I../Tokenize \
	@INDEX_CFLAGS@ @XML_CFLAGS@ @HTTP_CFLAGS@ @MIME_CFLAGS@

