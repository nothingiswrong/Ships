cmake_minimum_required(VERSION 3.16)

project(Ships2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()


qt_add_executable(appShips2
    main.cpp
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui

    validate.h
    validate.cpp
)

target_link_libraries(appShips2
    PRIVATE Qt6::Widgets
)

set_target_properties(appShips2 PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING
        ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS appShips2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()


add_executable(tests
    tst_validate.cpp
    validate.cpp
    validate.h
)

target_link_libraries(tests
    PRIVATE
    GTest::gtest_main
    GTest::gmock
)

include(GoogleTest)

gtest_discover_tests(tests)