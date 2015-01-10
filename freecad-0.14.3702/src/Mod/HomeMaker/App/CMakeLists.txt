
include_directories(
    ${Boost_INCLUDE_DIRS}
    ${OCC_INCLUDE_DIR}
    ${PYTHON_INCLUDE_PATH}
    ${ZLIB_INCLUDE_DIR}
    ${XERCESC_INCLUDE_DIR}
)

set(HomeMaker_LIBS
    FreeCADApp
)

SET(HomeMaker_SRCS
    AppHomeMaker.cpp
    AppHomeMakerPy.cpp
    PreCompiled.cpp
    PreCompiled.h
)

add_library(HomeMaker SHARED ${HomeMaker_SRCS})
target_link_libraries(HomeMaker ${HomeMaker_LIBS})


fc_target_copy_resource(HomeMaker 
    ${CMAKE_SOURCE_DIR}/src/Mod/HomeMaker
    ${CMAKE_BINARY_DIR}/Mod/HomeMaker
    Init.py)

SET_BIN_DIR(HomeMaker HomeMaker /Mod/HomeMaker)
SET_PYTHON_PREFIX_SUFFIX(HomeMaker)

install(TARGETS HomeMaker DESTINATION ${CMAKE_INSTALL_LIBDIR})
