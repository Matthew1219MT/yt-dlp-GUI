cmake_minimum_required(VERSION 3.19)
project(yt-dlp-GUI LANGUAGES CXX)

find_package(Qt6 6.11.1 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(yt-dlp-GUI
    WIN32 MACOSX_BUNDLE
    src/main.cpp

    src/model.h
    src/model.cpp

    src/view.h
    src/view.cpp

    src/controller.h
    src/controller.cpp

    src/ElidedLabel.h
    src/ElidedLabel.cpp

    src/downloadmodel.h
    src/downloadmodel.cpp

    src/mainwindow.ui
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

add_custom_command(TARGET yt-dlp-GUI POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/yt-dlp/yt-dlp.exe"
            "$<TARGET_FILE_DIR:yt-dlp-GUI>")