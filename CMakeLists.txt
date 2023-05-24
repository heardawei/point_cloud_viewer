# set(VCPKG_ROOT "D:/vcpkg")

if(VCPKG_ROOT)
    set(VCPKG_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    if(EXISTS ${VCPKG_TOOLCHAIN_FILE})
        set(CMAKE_TOOLCHAIN_FILE ${VCPKG_TOOLCHAIN_FILE})
    endif()
endif()

cmake_minimum_required(VERSION 3.5)
project(point_cloud_viewer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PCL CONFIG REQUIRED)
find_package(Qt6Widgets CONFIG REQUIRED)

qt_standard_project_setup()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(PROJECT_SOURCES
    main.cpp
    pclviewer.cpp
    pclviewer.h
)

qt_add_executable(point_cloud_viewer
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(point_cloud_viewer PRIVATE ${PCL_LIBRARIES} Qt6::Widgets)

set_target_properties(point_cloud_viewer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS point_cloud_viewer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(point_cloud_viewer)
endif()
