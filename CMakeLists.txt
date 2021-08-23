cmake_minimum_required(VERSION 3.5)

project(QtVulkan VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

# Find External Package
find_package(Vulkan REQUIRED)

file(GLOB_RECURSE VulkanHeader Vulkan*.h)
file(GLOB_RECURSE VulkanSource Vulkan*.cpp)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ${VulkanSource}
    ${VulkanHeader}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QtVulkan
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QtVulkan APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QtVulkan SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QtVulkan
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# vulkan define
if(WIN32)
    target_compile_definitions(QtVulkan PUBLIC VK_USE_PLATFORM_WIN32_KHR)
elseif(APPLE)
    target_compile_definitions(QtVulkan PUBLIC VK_USE_PLATFORM_MACOS_MVK)
endif()

# windows warnning remove
if(WIN32)
    target_compile_definitions(QtVulkan PUBLIC _CRT_SECURE_NO_WARNINGS)
endif()

target_link_libraries(QtVulkan PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(QtVulkan PRIVATE Vulkan::Vulkan)

set_target_properties(QtVulkan PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QtVulkan)
endif()
