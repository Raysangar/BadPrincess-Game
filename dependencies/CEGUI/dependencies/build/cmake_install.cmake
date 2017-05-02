# Install script for directory: L:/Estudios/Master/BadPrincess/BadPrincess/trunk/BadPrincess/dependencies/CEGUI/dependencies

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/CEGUI-DEPS")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("L:/Estudios/Master/BadPrincess/BadPrincess/trunk/BadPrincess/dependencies/CEGUI/dependencies/build/src/zlib-1.2.5/CEGUI-BUILD/cmake_install.cmake")
  INCLUDE("L:/Estudios/Master/BadPrincess/BadPrincess/trunk/BadPrincess/dependencies/CEGUI/dependencies/build/src/libpng-1.4.7/CEGUI-BUILD/cmake_install.cmake")
  INCLUDE("L:/Estudios/Master/BadPrincess/BadPrincess/trunk/BadPrincess/dependencies/CEGUI/dependencies/build/src/jpeg-8c/CEGUI-BUILD/cmake_install.cmake")
  INCLUDE("L:/Estudios/Master/BadPrincess/BadPrincess/trunk/BadPrincess/dependencies/CEGUI/dependencies/build/src/freetype-2.4.4/CEGUI-BUILD/cmake_install.cmake")
  INCLUDE("L:/Estudios/Master/BadPrincess/BadPrincess/trunk/BadPrincess/dependencies/CEGUI/dependencies/build/src/pcre-8.12/CEGUI-BUILD/cmake_install.cmake")
  INCLUDE("L:/Estudios/Master/BadPrincess/BadPrincess/trunk/BadPrincess/dependencies/CEGUI/dependencies/build/src/lua-5.1.4/CEGUI-BUILD/cmake_install.cmake")
  INCLUDE("L:/Estudios/Master/BadPrincess/BadPrincess/trunk/BadPrincess/dependencies/CEGUI/dependencies/build/src/expat-2.0.1/CEGUI-BUILD/cmake_install.cmake")
  INCLUDE("L:/Estudios/Master/BadPrincess/BadPrincess/trunk/BadPrincess/dependencies/CEGUI/dependencies/build/src/xerces-c-3.1.1/CEGUI-BUILD/cmake_install.cmake")
  INCLUDE("L:/Estudios/Master/BadPrincess/BadPrincess/trunk/BadPrincess/dependencies/CEGUI/dependencies/build/src/SILLY/CEGUI-BUILD/cmake_install.cmake")
  INCLUDE("L:/Estudios/Master/BadPrincess/BadPrincess/trunk/BadPrincess/dependencies/CEGUI/dependencies/build/src/glew-1.7.0/CEGUI-BUILD/cmake_install.cmake")
  INCLUDE("L:/Estudios/Master/BadPrincess/BadPrincess/trunk/BadPrincess/dependencies/CEGUI/dependencies/build/src/glfw-2.7.2/CEGUI-BUILD/cmake_install.cmake")
  INCLUDE("L:/Estudios/Master/BadPrincess/BadPrincess/trunk/BadPrincess/dependencies/CEGUI/dependencies/build/src/glm-0.9.3.1/CEGUI-BUILD/cmake_install.cmake")
  INCLUDE("L:/Estudios/Master/BadPrincess/BadPrincess/trunk/BadPrincess/dependencies/CEGUI/dependencies/build/src/tolua++-1.0.93/CEGUI-BUILD/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "L:/Estudios/Master/BadPrincess/BadPrincess/trunk/BadPrincess/dependencies/CEGUI/dependencies/build/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "L:/Estudios/Master/BadPrincess/BadPrincess/trunk/BadPrincess/dependencies/CEGUI/dependencies/build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
