
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

set(HomeMakerGui_LIBS
    HomeMaker
    FreeCADGui
)

qt4_add_resources(HomeMaker_QRC_SRCS Resources/HomeMaker.qrc)

SET(HomeMakerGui_SRCS
    ${HomeMaker_QRC_SRCS}
    AppHomeMakerGui.cpp
    AppHomeMakerGuiPy.cpp
    Command.cpp
    PreCompiled.cpp
    PreCompiled.h
    Workbench.cpp
    Workbench.h
)

add_library(HomeMakerGui SHARED ${HomeMakerGui_SRCS})
target_link_libraries(HomeMakerGui ${HomeMakerGui_LIBS})

fc_target_copy_resource(HomeMakerGui 
    ${CMAKE_SOURCE_DIR}/src/Mod/HomeMaker
    ${CMAKE_BINARY_DIR}/Mod/HomeMaker
    InitGui.py)

SET_BIN_DIR(HomeMakerGui HomeMakerGui /Mod/HomeMaker)
SET_PYTHON_PREFIX_SUFFIX(HomeMakerGui)
    
install(TARGETS HomeMakerGui DESTINATION ${CMAKE_INSTALL_LIBDIR})
