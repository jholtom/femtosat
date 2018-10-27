# Install script for directory: /home/jacob/cygnus/gnuradio/gr-sdlp/python

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/python2.7/site-packages/sdlp" TYPE FILE FILES
    "/home/jacob/cygnus/gnuradio/gr-sdlp/python/__init__.py"
    "/home/jacob/cygnus/gnuradio/gr-sdlp/python/create_frame.py"
    "/home/jacob/cygnus/gnuradio/gr-sdlp/python/destruct_frame.py"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/python2.7/site-packages/sdlp" TYPE FILE FILES
    "/home/jacob/cygnus/gnuradio/gr-sdlp/cmake/python/__init__.pyc"
    "/home/jacob/cygnus/gnuradio/gr-sdlp/cmake/python/create_frame.pyc"
    "/home/jacob/cygnus/gnuradio/gr-sdlp/cmake/python/destruct_frame.pyc"
    "/home/jacob/cygnus/gnuradio/gr-sdlp/cmake/python/__init__.pyo"
    "/home/jacob/cygnus/gnuradio/gr-sdlp/cmake/python/create_frame.pyo"
    "/home/jacob/cygnus/gnuradio/gr-sdlp/cmake/python/destruct_frame.pyo"
    )
endif()

