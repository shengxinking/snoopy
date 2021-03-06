### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### First process all required subdirs
#
SUBDIRS = datasource output



### Create main library, but without execve() wrappers
#
noinst_LTLIBRARIES = \
	libsnoopy_no_execve.la

libsnoopy_no_execve_la_SOURCES = \
	configuration.c \
	configuration.h \
	error.c \
	error.h \
	inputdatastorage.c \
	inputdatastorage.h \
	datasourceregistry.c \
	datasourceregistry.h \
	log.c \
	log.h \
	message.c \
	message.h \
	misc.c \
	misc.h \
	outputregistry.c \
	outputregistry.h \
	snoopy.h

libsnoopy_no_execve_la_LIBADD = \
	datasource/libsnoopy_datasources_all.la \
	output/libsnoopy_outputs_all.la

# If config file is enabled, build and link these too
if CONFIG_FILE_ENABLED
libsnoopy_no_execve_la_SOURCES += \
	configfile.c \
	configfile.h
libsnoopy_no_execve_la_LIBADD += \
	../lib/libiniparser.la
endif

# If filtering is enabled, add these too
if FILTERING_ENABLED
SUBDIRS += \
	filter
libsnoopy_no_execve_la_SOURCES += \
	filtering.c \
	filtering.h \
	filterregistry.c \
	filterregistry.h
libsnoopy_no_execve_la_LIBADD += \
	filter/libsnoopy_filters_all.la
endif



### Create main library, whole
#
# Exclude -pedantic from CFLAGS because casing object pointer received from
# dlsym to function pointer is not allowed by ISO C.
#
lib_LTLIBRARIES      = libsnoopy.la
libsnoopy_la_SOURCES = libsnoopy.c
libsnoopy_la_CFLAGS  = `echo $(AM_CFLAGS) | sed -e 's/-pedantic//'`
libsnoopy_la_LIBADD  = libsnoopy_no_execve.la
