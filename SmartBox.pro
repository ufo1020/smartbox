#-------------------------------------------------
#
# Project created by QtCreator 2015-06-08T22:50:52
#
#-------------------------------------------------

CONFIG += ordered

TEMPLATE = subdirs

SUBDIRS += qhttpserver \
           temperaturemonitor

temperaturemonitor.depends = qhttpserver
