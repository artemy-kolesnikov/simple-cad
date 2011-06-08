SET(PROJECT cad)

PROJECT(${PROJECT})

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

FIND_PACKAGE(Qt4 REQUIRED)

INCLUDE(${QT_USE_FILE})

QT4_ADD_RESOURCES(CAD_RCS cad.qrc)

SET(SOURCES
	action/boolean.cpp
	action/common.cpp
	action/create3dprimitive.cpp
	action/createbox.cpp
	action/createcone.cpp
	action/createcylinder.cpp
	action/createellipsoid.cpp
	action/createplane.cpp
	action/createrectangle.cpp
	action/createsphere.cpp
	action/createtorus.cpp
	action/cut.cpp
	action/fuse.cpp
	filehelper.cpp
	gui/cadapplication.cpp
	gui/childwindow.cpp
	gui/controller.cpp
	gui/command/createprimitivecommand.cpp
	gui/command/createsketchcommand.cpp
	gui/interactiveview.cpp
	gui/inventorviewer.cpp
	gui/mainwindow.cpp
	gui/model.cpp
	gui/view.cpp
	gui/viewprovider.cpp
	gui/viewershape.cpp
	gui/widget/createprimitivewidget.cpp
	gui/widget/pointsettingswidget.cpp
	gui/widget/positionsettingswidget.cpp
	gui/widget/propertieswidget.cpp
	gui/widget/widgetdialog.cpp
	main.cpp
	sketcher/gui/sketcherview.cpp
	sketcher/mod/primitive2d.cpp
	sketcher/mod/rectangle.cpp
	sketcher/mod/polyline.cpp
	sketcher/mod/circle.cpp
	sketcher/mod/resizepolygon.cpp
	common/sendmessagecommand.cpp
	${CAD_RCS}
)

SET(HEADERS
	gui/cadapplication.h
	gui/childwindow.h
	gui/controller.h
	gui/interactiveview.h
	gui/mainwindow.h
	gui/model.h
	gui/view.h
	gui/widget/createprimitivewidget.h
	gui/widget/pointsettingswidget.h
	gui/widget/positionsettingswidget.h
	gui/widget/propertieswidget.h
	gui/widget/widgetdialog.h
	sketcher/mod/primitive2d.h
	sketcher/gui/sketcherview.h
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

set (coin3d_lib
	Coin
	SoQt4
)

set (base_directory .)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} ${OPENCASCADE_INCLUDE}
	${QT_QTOPENGL_INCLUDE_DIR} ${base_directory} ${base_directory}/gui
	${base_directory}/gui/widget ${base_directory}/operation
	${base_directory}/common ${base_directory}/action
	${base_directory}/sketcher/gui ${base_directory}/sketcher/mod ${base_directory}/gui/command
	${base_directory}/sketcher/command ${base_directory}/sketcher)

QT4_WRAP_CPP(SOURCES ${HEADERS})

#IF(UNIX)
  ADD_EXECUTABLE(${PROJECT} ${SOURCES})
  TARGET_LINK_LIBRARIES(${PROJECT} ${QT_LIBRARIES} ${OPENCASCADE_LIB} ${OPENGL_LIBRARIES} ${QT_QTOPENGL_LIBRARY} ${coin3d_lib})
#ENDIF()

