SET(PROJECT cad)

PROJECT(${PROJECT} CXX)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

FIND_PACKAGE(Qt4 REQUIRED)

INCLUDE(${QT_USE_FILE})

SET(SOURCES
	cadapplication.cpp
	childwindow.cpp
	controller.cpp
	filehelper.cpp
	main.cpp
	mainwindow.cpp
	model.cpp
	view.cpp
)

SET(HEADERS
	cadapplication.h
	childwindow.h
	controller.h
	filehelper.h
	mainwindow.h
	model.h
	view.h
)

if (UNIX)
	FIND_PACKAGE(X11 REQUIRED)
	FIND_PACKAGE(OpenGL REQUIRED)
ENDIF()

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

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} ${OPENCASCADE_INCLUDE})

QT4_WRAP_CPP(SOURCES ${HEADERS})

IF(UNIX)
  ADD_EXECUTABLE(${PROJECT} ${SOURCES})
  TARGET_LINK_LIBRARIES(${PROJECT} ${QT_LIBRARIES} ${OPENCASCADE_LIB} ${OPENGL_LIBRARIES})
ENDIF()

