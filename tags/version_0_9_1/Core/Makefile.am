# Process this file with automake to produce Makefile.in

noinst_HEADERS = \
	DaemonState.h \
	OnDiskHandler.h \
	PinotSettings.h \
	PinotUtils.h \
	ServerThreads.h \
	UniqueApplication.h \
	WorkerThreads.h

noinst_LTLIBRARIES = libCore.la

libCore_la_SOURCES = \
	PinotSettings.cpp \
	PinotUtils.cpp \
	UniqueApplication.cpp \
	WorkerThreads.cpp

if HAVE_DBUS
bin_PROGRAMS = pinot-index pinot-search pinot-dbus-daemon
else
bin_PROGRAMS = pinot-index pinot-search
endif

AM_CXXFLAGS = \
	@MISC_CFLAGS@ \
	-I$(top_srcdir)/Utils \
	-I$(top_srcdir)/Tokenize \
	-I$(top_srcdir)/Tokenize/filters \
	-I$(top_srcdir)/SQL \
	-I$(top_srcdir)/Monitor \
	-I$(top_srcdir)/Collect \
	-I$(top_srcdir)/IndexSearch \
	@SQL_CFLAGS@ @HTTP_CFLAGS@ @XML_CFLAGS@ @DBUS_CFLAGS@ \
	@INDEX_CFLAGS@ @GTHREAD_CFLAGS@ @GTKMM_CFLAGS@

if HAVE_DBUS
AM_CXXFLAGS += -DHAVE_DBUS
endif

pinot_index_LDFLAGS = \
	-rdynamic

pinot_index_LDADD = \
	-L$(top_builddir)/Utils \
	-L$(top_builddir)/Tokenize \
	-L$(top_builddir)/SQL \
	-L$(top_builddir)/Monitor \
	-L$(top_builddir)/Collect \
	-L$(top_builddir)/IndexSearch \
	-lCore -lIndexSearch -lMonitor -lCollect -lSQL \
	-lTokenize -lUtils -lBasicUtils @LIBS@ \
	@GLIBMM_LIBS@ @DBUS_LIBS@ @GTHREAD_LIBS@ @XML_LIBS@ \
	@HTTP_LIBS@ @SQL_LIBS@ @MISC_LIBS@

pinot_index_SOURCES = pinot-index.cpp

pinot_index_DEPENDENCIES = libCore.la

pinot_search_LDFLAGS = \
	-rdynamic

pinot_search_LDADD = \
	-L$(top_builddir)/Utils \
	-L$(top_builddir)/Tokenize \
	-L$(top_builddir)/Collect \
	-L$(top_builddir)/IndexSearch \
	-lCore -lIndexSearch -lCollect -lTokenize \
	-lUtils -lBasicUtils @LIBS@ \
	@GLIBMM_LIBS@ @DBUS_LIBS@ \
	@XML_LIBS@ @HTTP_LIBS@ @MISC_LIBS@

pinot_search_SOURCES = \
	pinot-search.cpp

pinot_search_DEPENDENCIES = libCore.la

pinot_dbus_daemon_LDFLAGS = \
	-rdynamic

pinot_dbus_daemon_LDADD = \
	-L$(top_builddir)/Utils \
	-L$(top_builddir)/Tokenize \
	-L$(top_builddir)/SQL \
	-L$(top_builddir)/Monitor \
	-L$(top_builddir)/Collect \
	-L$(top_builddir)/IndexSearch \
	-lCore -lIndexSearch -lMonitor -lCollect -lSQL \
	-lTokenize -lUtils -lBasicUtils @LIBS@ \
	@GLIBMM_LIBS@ @DBUS_LIBS@ @GTHREAD_LIBS@ @XML_LIBS@ \
	@HTTP_LIBS@ @SQL_LIBS@ @MISC_LIBS@

pinot_dbus_daemon_SOURCES = \
	DaemonState.cpp \
	OnDiskHandler.cpp \
	ServerThreads.cpp \
	pinot-dbus-daemon.cpp

pinot_dbus_daemon_DEPENDENCIES = libCore.la

