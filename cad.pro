# Простая CAD система, основана на opencascade
# Copyright (C) 2010 Artemy Kolesnikov <artemy.kolesnikov@gmail.com>

# Сюда складывается весь компиляторный мусор
OBJECTS_DIR = ./obj
MOC_DIR = $$OBJECTS_DIR
UI_DIR = $$OBJECTS_DIR

include(QtOpenCascadeWin32.pri)

# Опции проекта (тип приложения, имя бинарника, доп.либы для qt, соответственно)
TEMPLATE = app
QT += 
CONFIG  += opengl

DEFINES += WNT

# Файлы исходного кода
SOURCES += main.cpp \
	mainwindow.cpp \
	view.cpp \
	childwindow.cpp \
	model.cpp \
	controller.cpp \
	cadapplication.cpp \
	filehelper.cpp

HEADERS += mainwindow.h \
	view.h \
	childwindow.h \
	model.h \
	controller.h \
	cadapplication.h \
	filehelper.h

