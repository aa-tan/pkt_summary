
LIBS_DEPTH = ../..
LIB_DEPENDENCIES = \
   momalib \
   gedilib \
   gselib \
   tools \
   nr \
   vacport \
   gse_streams
include(../../custom.pri)

# Input
HEADERS += mainclass.h
SOURCES += main.cpp mainclass.cpp

CONFIG += qt console
CONFIG -= app_bundle
QT -= gui


