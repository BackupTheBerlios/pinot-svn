# Process this file with automake to produce Makefile.in

noinst_HEADERS = \
	DBusIndex.h \
	FilterWrapper.h \
	IndexInterface.h \
	ModuleFactory.h \
	ModuleProperties.h \
	OpenSearchParser.h \
	PluginParsers.h \
	PluginWebEngine.h \
	QueryProperties.h \
	ResultsExporter.h \
	SearchEngineInterface.h \
	SearchPluginProperties.h \
	SherlockParser.h \
	WebEngine.h \
	XesamLog.h

if HAVE_DBUS
noinst_LTLIBRARIES = libIndex.la libIndexSearch.la
else
noinst_LTLIBRARIES = libIndexSearch.la
endif

libIndex_la_SOURCES = \
	DBusIndex.cpp

libIndexSearch_la_SOURCES = \
	FilterWrapper.cpp \
	ModuleFactory.cpp \
	OpenSearchParser.cpp \
	PluginWebEngine.cpp \
	QueryProperties.cpp \
	ResultsExporter.cpp \
	SearchEngineInterface.cpp \
	SearchPluginProperties.cpp \
	WebEngine.cpp

if HAVE_DBUS
libIndexSearch_la_SOURCES += DBusIndex.cpp
endif

if HAVE_BOOST_SPIRIT
libIndexSearch_la_SOURCES += SherlockParser.cpp
endif

if HAVE_DBUS
bin_PROGRAMS = pinot-label
endif

pinot_label_LDFLAGS = \
	-export-dynamic

pinot_label_LDADD = \
	-L$(top_builddir)/Utils \
	-lIndex -lUtils -lBasicUtils \
	@GLIBMM_LIBS@ @DBUS_LIBS@ \
	@GTHREAD_LIBS@ @HTTP_LIBS@ @MISC_LIBS@

pinot_label_SOURCES = pinot-label.cpp

pinot_label_DEPENDENCIES = libIndex.la

AM_CXXFLAGS = \
	@MISC_CFLAGS@ \
	-I$(top_srcdir)/Utils \
	-I$(top_srcdir)/Tokenize \
	-I$(top_srcdir)/Tokenize/filters \
	-I$(top_srcdir)/Collect \
	-I$(top_srcdir)/IndexSearch/cjkv \
	-I$(top_srcdir)/IndexSearch/Google \
	-I$(top_srcdir)/IndexSearch/xesam \
	@HTTP_CFLAGS@ @XML_CFLAGS@ @DBUS_CFLAGS@ \
	@INDEX_CFLAGS@ @GLIBMM_CFLAGS@

if HAVE_DBUS
AM_CXXFLAGS += -DHAVE_DBUS
endif

if HAVE_BOOST_SPIRIT
AM_CXXFLAGS += -DHAVE_BOOST_SPIRIT
endif

