# Сюда складывается весь компиляторный треш
OBJECTS_DIR = ./obj
MOC_DIR = $$OBJECTS_DIR
UI_DIR = $$OBJECTS_DIR

# Опции проекта (тип приложения, имя бинарника, доп.либы для qt, соответственно)
TEMPLATE = app
QT += 
CONFIG += link_pkgconfig
PKGCONFIG += gl 

INCLUDEPATH = /usr/include/opencascade

LIBS = -lTKernel -lPTKernel -lTKMath -lTKService -lTKV3d -lTKV2d \
       -lTKBRep -lTKIGES -lTKSTL -lTKVRML -lTKSTEP -lTKSTEPAttr -lTKSTEP209 \
       -lTKSTEPBase -lTKShapeSchema -lTKGeomBase -lTKGeomAlgo -lTKG3d -lTKG2d \
       -lTKXSBase -lTKPShape -lTKShHealing -lTKHLR -lTKTopAlgo -lTKMesh -lTKPrim \
       -lTKCDF -lTKBool -lTKBO -lTKFillet -lTKOffset\ 

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

