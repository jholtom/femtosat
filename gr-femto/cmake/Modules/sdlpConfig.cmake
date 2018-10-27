INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_SDLP sdlp)

FIND_PATH(
    SDLP_INCLUDE_DIRS
    NAMES sdlp/api.h
    HINTS $ENV{SDLP_DIR}/include
        ${PC_SDLP_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    SDLP_LIBRARIES
    NAMES gnuradio-sdlp
    HINTS $ENV{SDLP_DIR}/lib
        ${PC_SDLP_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDLP DEFAULT_MSG SDLP_LIBRARIES SDLP_INCLUDE_DIRS)
MARK_AS_ADVANCED(SDLP_LIBRARIES SDLP_INCLUDE_DIRS)

