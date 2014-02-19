AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

CPPFLAGS += -Iinclude
#CPPFLAGS += -fopenmp

#lib_LTLIBRARIES = libTHIS.la
#libArgumentParser_la_SOURCES = src/....cpp
#libArgumentParser_la_LDFLAGS = -version-info 0:0:0
#include_HEADERS = include/THIS.h

bin_PROGRAMS = xyzeval
xyzeval_SOURCES = src/Bond.cpp src/BondMap.cpp src/CoordinationCutoffs.cpp src/CoordinationHistogramData.cpp src/Type.cpp src/convert.cpp src/excr.cpp src/main.cpp src/utils.cpp
xyzeval_LDFLAGS = -lArgumentParser

#bin_SCRIPTS = src/*.sh

