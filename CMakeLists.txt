# See http://qt-project.org/quarterly/view/using_cmake_to_build_qt_projects
# for more information on building a Qt project with CMake.

project(glextension)

cmake_minimum_required(VERSION 2.8)

find_package(Qt4 REQUIRED QtCore QtGui QtWebKit QtOpenGL)

set(src_dir ${CMAKE_CURRENT_SOURCE_DIR}/source/glxtension)
include_directories(${src_dir})

set(glxtension_SOURCES
    ${src_dir}/main.cpp
    ${src_dir}/main_window.cpp)

set(glxtension_HEADERS
    ${src_dir}/gl_helper_widget.h
    ${src_dir}/main_window.h)
    
set(glxtension_FORMS
    ${src_dir}/main_window.ui)

set(glxtension_RESOURCES
    resources.qrc)

# Call Qt's MOC, UIC and RCC
qt4_wrap_cpp(glxtension_HEADERS_MOC ${glxtension_HEADERS})
qt4_wrap_ui(glxtension_FORMS_HEADERS ${glxtension_FORMS})
qt4_add_resources(glxtension_RESOURCES_RCC ${glxtension_RESOURCES})

# Help CMake find Qt's intermediate header files
include_directories(${CMAKE_CURRENT_BINARY_DIR})
link_directories(${CMAKE_CURRENT_BINARY_DIR})

# Include qt include directories and definitions
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITONS})

add_executable(glxtension
               ${glxtension_SOURCES}
               ${glxtension_HEADERS_MOC}
               ${glxtension_FORMS_HEADERS}
               ${glxtension_RESOURCES_RCC})

target_link_libraries(glxtension ${QT_LIBRARIES})
