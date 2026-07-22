cmake_minimum_required(VERSION 3.19)
project(yt-dlp-GUI LANGUAGES CXX)

find_package(Qt6 6.11.1 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(yt-dlp-GUI
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

target_link_libraries(yt-dlp-GUI
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS yt-dlp-GUI
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET yt-dlp-GUI
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})