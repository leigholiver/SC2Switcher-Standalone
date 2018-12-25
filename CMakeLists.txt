cmake_minimum_required(VERSION 3.7)
set(CMAKE_AUTOMOC ON)
project(webhook)

list(APPEND CMAKE_MODULE_PATH "${OBS_CMAKE_MODULES_PATH}/")
find_package(Libcurl REQUIRED)
include_directories(${LIBCURL_INCLUDE_DIRS})

add_subdirectory(jansson)

set(OBS_JANSSON_IMPORT "jansson" CACHE INTERNAL "Internal var")
set(OBS_JANSSON_INCLUDE_DIRS "" CACHE INTERNAL "Internal var")
include_directories(${JANSSON_INCLUDE_DIRS})


set(webhook_HEADERS
	${webhook_HEADERS}
	SC2Data.h
	Observer.h
	SC2State.h
	Constants.h
	Webhook.h
	Config.h
	ScoreTracker.h
	MainWindow.h
	FileScoreWriter.h
	ScoreTracker.h
	ScoreWriter.h
	)
set(webhook_SOURCES
	${webhook_SOURCES}
	main.cpp
	SC2Data.cpp
	SC2State.cpp
	Webhook.cpp
	Config.cpp
	ScoreTracker.cpp
	MainWindow.cpp
	FileScoreWriter.cpp
	)
set(webhook_UI
	${webhook_UI}
	MainWindow.ui
	)

set(webhook_PLATFORM_LIBS
	${OBS_JANSSON_IMPORT} 
	)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
qt5_wrap_ui(webhook_UI_HEADERS
	${webhook_UI})

add_executable(webhook WIN32 
	${webhook_HEADERS}
	${webhook_SOURCES}
	${webhook_UI_HEADERS}
	${webhook_PLATFORM_SOURCES}
	${webhook_PLATFORM_HEADERS}
	)


target_link_libraries(webhook
	${webhook_PLATFORM_LIBS}
	Qt5::Widgets
	${LIBCURL_LIBRARIES})
