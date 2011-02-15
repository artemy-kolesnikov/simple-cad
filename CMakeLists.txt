SET(PROJECT cad)

PROJECT(${PROJECT})

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

FIND_PACKAGE(Qt4 REQUIRED)

INCLUDE(${QT_USE_FILE})

#SET (EXECUTABLE_OUTPUT_PATH out_trash)
#SET (CMAKE_EXECUTABLE_OUTPUT_PATH out_trash)
#SET (RUNTIME_OUTPUT_DIRECTORY out_trash)

SET(SOURCES
	cadapplication.cpp
	childwindow.cpp
	controller.cpp
	filehelper.cpp
	main.cpp
	mainwindow.cpp
	model.cpp
	view.cpp
	qshapemodel.cpp
	propertieswidget.cpp
	positionsettingswidget.cpp
	pointsettingswidget.cpp
	widgetdialog.cpp
	primitivesettingswidget.cpp
)

SET(HEADERS
	cadapplication.h
	childwindow.h
	controller.h
	mainwindow.h
	model.h
	view.h
	qshapemodel.h
	propertieswidget.h
	positionsettingswidget.h
	pointsettingswidget.h
	widgetdialog.h
	primitivesettingswidget.h
)

if (UNIX)
	FIND_PACKAGE(X11 REQUIRED)
ENDIF()

FIND_PACKAGE(OpenGL REQUIRED)

SET(OPENCASCADE_LIB
	TKernel
	PTKernel
	TKMath
	TKService
	TKV3d
	TKV2d
	TKBRep
	TKIGES
	TKSTL
	TKVRML
	TKSTEP
	TKSTEPAttr
	TKSTEP209
	TKSTEPBase
	TKShapeSchema
	TKGeomBase
	TKGeomAlgo
	TKG3d
	TKG2d
	TKXSBase
	TKPShape
	TKShHealing
	TKHLR
	TKTopAlgo
	TKMesh
	TKPrim
	TKCDF
	TKBool
	TKBO
	TKFillet
	TKOffset
)

SET(OPENCASCADE_INCLUDE
	/usr/include/opencascade
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} ${OPENCASCADE_INCLUDE} ${QT_QTOPENGL_INCLUDE_DIR})

QT4_WRAP_CPP(SOURCES ${HEADERS})

#IF(UNIX)
  ADD_EXECUTABLE(${PROJECT} ${SOURCES})
  TARGET_LINK_LIBRARIES(${PROJECT} ${QT_LIBRARIES} ${OPENCASCADE_LIB} ${OPENGL_LIBRARIES} ${QT_QTOPENGL_LIBRARY})
#ENDIF()

