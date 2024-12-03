cmake_minimum_required(VERSION 3.16)

project(DED_Controller VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(DED_Controller
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        myfunction.h myfunction.cpp
        axismovethread.h axismovethread.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET DED_Controller APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(DED_Controller SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(DED_Controller
            ${PROJECT_SOURCES}
        )
    endif()
endif()

set(GUGAOINC_DIR "./gugao")
set(GUGAOLINK_DIR "./gugao")


include_directories(${GUGAOINC_DIR})

# link_directories(${GUGAOLINK_DIR})

target_link_directories(DED_Controller PRIVATE ${GUGAOLINK_DIR})

target_link_libraries(DED_Controller PRIVATE gts)


# file(GLOB GUGAO_DLL_FILES ${GUGAOLINK_DIR}/*.dll)


# add_custom_command(TARGET DED_Controller POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#     ${GUGAO_DLL_FILES}
#     $<TARGET_FILE_DIR:DED_Controller>)




target_link_libraries(DED_Controller PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.DED_Controller)
endif()
set_target_properties(DED_Controller PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS DED_Controller
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(DED_Controller)
endif()
