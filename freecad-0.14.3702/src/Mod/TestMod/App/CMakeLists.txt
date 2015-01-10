
include_directories(
    ${Boost_INCLUDE_DIRS}
    ${OCC_INCLUDE_DIR}
    ${PYTHON_INCLUDE_PATH}
    ${ZLIB_INCLUDE_DIR}
    ${XERCESC_INCLUDE_DIR}
)

set(TestMod_LIBS
    FreeCADApp
)

SET(TestMod_SRCS
    AppTestMod.cpp
    AppTestModPy.cpp
    PreCompiled.cpp
    PreCompiled.h
)

add_library(TestMod SHARED ${TestMod_SRCS})
target_link_libraries(TestMod ${TestMod_LIBS})


fc_target_copy_resource(TestMod 
    ${CMAKE_SOURCE_DIR}/src/Mod/TestMod
    ${CMAKE_BINARY_DIR}/Mod/TestMod
    Init.py)

SET_BIN_DIR(TestMod TestMod /Mod/TestMod)
SET_PYTHON_PREFIX_SUFFIX(TestMod)

install(TARGETS TestMod DESTINATION ${CMAKE_INSTALL_LIBDIR})
