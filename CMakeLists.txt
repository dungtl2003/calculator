cmake_minimum_required(VERSION 3.20)

project(Calculator VERSION 0.8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# qt_standard_project_setup() handles enabling MOC and RCC automatically
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(APP_NAME ${CMAKE_PROJECT_NAME})
set(APP_VERSION ${CMAKE_PROJECT_VERSION})

################################################################################

message(STATUS "[CMAKE]   CMAKE_VERSION             = ${CMAKE_VERSION}")
message(STATUS "[CMAKE]   CMAKE_PREFIX_PATH         = ${CMAKE_PREFIX_PATH}")
message(STATUS "[SYSTEM]  (HOST) SYSTEM NAME        = ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "[SYSTEM]  (HOST) SYSTEM VERSION     = ${CMAKE_HOST_SYSTEM_VERSION}")
message(STATUS "[SYSTEM]  (HOST) SYSTEM PROCESSOR   = ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "[SYSTEM]  (TARGET) SYSTEM NAME      = ${CMAKE_SYSTEM_NAME}")
message(STATUS "[SYSTEM]  (TARGET) SYSTEM VERSION   = ${CMAKE_SYSTEM_VERSION}")
message(STATUS "[SYSTEM]  (TARGET) SYSTEM PROCESSOR = ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "[PROJECT] CURRENT_SOURCE_DIR        = ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "[PROJECT] SOURCE_DIR                = ${CMAKE_SOURCE_DIR}")
message(STATUS "[PROJECT] BUILD_TYPE                = ${CMAKE_BUILD_TYPE}")
message(STATUS "[PROJECT] TOOLCHAIN_FILE            = ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "[PROJECT] QT_HOST_PATH              = ${QT_HOST_PATH}")

################################################################################

include(FetchContent)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(spdlog)

################################################################################

find_package(Qt6 REQUIRED COMPONENTS Core Gui Svg Qml Quick QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0002 NEW)
qt_policy(SET QTP0003 NEW)
qt_policy(SET QTP0004 NEW)

qt_add_executable(${CMAKE_PROJECT_NAME}
    src/main.cpp
    # src/CalculatorEngine.h src/CalculatorEngine.cpp
    src/Logger.h src/Logger.cpp
)

qt_add_qml_module(${CMAKE_PROJECT_NAME}
    URI ${CMAKE_PROJECT_NAME}
    NO_PLUGIN

    QML_FILES
    qml/Application.qml
    qml/CalculatorButton.qml
    qml/Display.qml
    qml/NumPad.qml
    qml/DigitButton.qml
    qml/OperatorButton.qml
    qml/ACButton.qml
    qml/BackspaceButton.qml
    qml/FlipSignButton.qml
    qml/DotButton.qml
    qml/ScrollableText.qml

    SOURCES
    src/CalculatorEngine.h
    src/CalculatorEngine.cpp

    RESOURCES
    qml/assets/backspace.svg
)

################################################################################

# Qt libraries
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Svg
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    spdlog::spdlog
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE src/)

################################################################################

## Release build? Set "no debug" macros
if(CMAKE_BUILD_TYPE STREQUAL Release)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
        NDEBUG
        QT_NO_DEBUG
        QT_NO_DEBUG_OUTPUT
    )
endif()

################################################################################

## Linux
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # System install rules
    install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION bin/)
    install(FILES ${CMAKE_SOURCE_DIR}/assets/linux/${CMAKE_PROJECT_NAME}.desktop DESTINATION share/applications/)
    install(FILES ${CMAKE_SOURCE_DIR}/assets/linux/${CMAKE_PROJECT_NAME}.appdata.xml DESTINATION share/appdata/)
    install(FILES ${CMAKE_SOURCE_DIR}/assets/linux/${CMAKE_PROJECT_NAME}.svg DESTINATION share/pixmaps/)
    install(FILES ${CMAKE_SOURCE_DIR}/assets/linux/${CMAKE_PROJECT_NAME}.svg DESTINATION share/icons/hicolor/scalable/apps/)
    install(FILES ${CMAKE_SOURCE_DIR}/LICENSE.md DESTINATION share/licenses/${CMAKE_PROJECT_NAME}/)
endif()

################################################################################

# Output (build)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    #ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
    #LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
