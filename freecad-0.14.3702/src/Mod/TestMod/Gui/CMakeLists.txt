
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${COIN3D_INCLUDE_DIR}
    ${QT_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIR}
    ${SOQT_INCLUDE_DIR}
    ${PYTHON_INCLUDE_PATH}
    ${XERCESC_INCLUDE_DIR}
)

set(TestModGui_LIBS
    TestMod
    FreeCADGui
)

qt4_add_resources(TestMod_QRC_SRCS Resources/TestMod.qrc)

SET(TestModGui_SRCS
    ${TestMod_QRC_SRCS}
    AppTestModGui.cpp
    AppTestModGuiPy.cpp
    Command.cpp
    PreCompiled.cpp
    PreCompiled.h
    Workbench.cpp
    Workbench.h
)

add_library(TestModGui SHARED ${TestModGui_SRCS})
target_link_libraries(TestModGui ${TestModGui_LIBS})

fc_target_copy_resource(TestModGui 
    ${CMAKE_SOURCE_DIR}/src/Mod/TestMod
    ${CMAKE_BINARY_DIR}/Mod/TestMod
    InitGui.py)

SET_BIN_DIR(TestModGui TestModGui /Mod/TestMod)
SET_PYTHON_PREFIX_SUFFIX(TestModGui)
    
install(TARGETS TestModGui DESTINATION ${CMAKE_INSTALL_LIBDIR})
