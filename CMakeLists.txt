set(PROJECT cad)

project(${PROJECT})

cmake_minimum_required(VERSION 2.6.0)

find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})

qt4_add_resources(CAD_RCS cad.qrc)

set(SOURCES
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
	action/fillet.cpp
	action/cut.cpp
	action/fuse.cpp
	filehelper.cpp
	gui/cadapplication.cpp
	gui/childwindow.cpp
	gui/controller.cpp
	gui/command/createprimitivecommand.cpp
	gui/command/createsketchcommand.cpp
	gui/command/removecommand.cpp
	gui/command/boolcommand.cpp
	gui/command/filletcommand.cpp
	gui/interactiveview.cpp
	gui/inventorviewer.cpp
	gui/mainwindow.cpp
	gui/model.cpp
	gui/view.cpp
	gui/shapemodel.cpp
	gui/shape.cpp
	gui/viewprovider.cpp
	gui/viewershape.cpp
	gui/version.cpp
	gui/widget/createprimitivewidget.cpp
	gui/widget/pointsettingswidget.cpp
	gui/widget/positionsettingswidget.cpp
	gui/widget/propertieswidget.cpp
	gui/widget/widgetdialog.cpp
	gui/widget/booloperationwidget.cpp
	gui/widget/shapelistwidget.cpp
	gui/widget/filletoperationwidget.cpp
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

set(HEADERS
	gui/cadapplication.h
	gui/childwindow.h
	gui/controller.h
	gui/interactiveview.h
	gui/mainwindow.h
	gui/model.h
	gui/view.h
	gui/shapemodel.h
	gui/inventorviewer.h
	gui/widget/createprimitivewidget.h
	gui/widget/pointsettingswidget.h
	gui/widget/positionsettingswidget.h
	gui/widget/propertieswidget.h
	gui/widget/widgetdialog.h
	gui/widget/booloperationwidget.h
	gui/widget/shapelistwidget.h
	gui/widget/filletoperationwidget.h
	sketcher/mod/primitive2d.h
	sketcher/gui/sketcherview.h
)

if (UNIX)
	find_package(X11 REQUIRED)

	set(OPENCASCADE_INCLUDE
		/usr/include/opencascade
	)
endif()

find_package(OpenGL REQUIRED)

set(OPENCASCADE_LIB
	TKMath
	TKBRep
	TKIGES
	TKSTL
	TKSTEP
	TKSTEPAttr
	TKSTEP209
	TKSTEPBase
	TKShapeSchema
	TKGeomBase
	TKGeomAlgo
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

set (coin3d_lib
	Coin
	SoQt4
)

set (base_directory .)

if (WIN32)
	set (WINDOWS_INCLUDE
		c:/opencascade/ros/inc
		c:/boost_1_46_1/libs
		c:/SoQt/src
		c:/coin/include)

	add_definitions(-DWNT)
	add_definitions(-DWIN32)
	add_definitions(-D_WINDOWS)
	add_definitions(-DSOQT_DLL)
	add_definitions(-DCOIN_DLL)

	set (OPENCASCADE_LIB_DIR
		c:/opencascade/ros/win32/vc9/lib)

	set (COIN_LIB_DIR
		c:/coin/lib)

	link_directories(${OPENCASCADE_LIB_DIR} ${COIN_LIB_DIR})
endif()

set (CMAKE_BUILD_TYPE "Release")

include_directories(${CMAKE_SOURCE_DIR} ${OPENCASCADE_INCLUDE}
	${base_directory} ${base_directory}/gui
	${base_directory}/gui/widget
	${base_directory}/common ${base_directory}/action
	${base_directory}/sketcher/gui ${base_directory}/sketcher/mod ${base_directory}/gui/command
	${base_directory}/sketcher/command ${base_directory}/sketcher ${WINDOWS_INCLUDE})

qt4_wrap_cpp(SOURCES ${HEADERS})

if (UNIX)
  add_executable(${PROJECT} ${SOURCES})
  target_link_libraries(${PROJECT} ${QT_LIBRARIES} ${OPENCASCADE_LIB} ${OPENGL_LIBRARIES} ${QT_QTOPENGL_LIBRARY} ${coin3d_lib})
elseif(WIN32)
  add_executable(${PROJECT} WIN32 ${SOURCES})
  target_link_libraries(${PROJECT} ${QT_LIBRARIES} ${OPENCASCADE_LIB} ${OPENGL_LIBRARIES} ${QT_QTOPENGL_LIBRARY} ${coin3d_lib} ${QT_QTMAIN_LIBRARY})
endif()

