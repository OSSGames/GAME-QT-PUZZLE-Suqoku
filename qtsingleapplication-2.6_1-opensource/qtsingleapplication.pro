TEMPLATE=subdirs
CONFIG += ordered
include(common.pri)
qtsingleapplication-uselib:SUBDIRS=buildlib
SUBDIRS+=examples

CONFIG += x86 x86_64
