cmake_minimum_required(VERSION 3.16)
project(QMentat VERSION 1.0.2 LANGUAGES CXX)
# Do not move the above lines from the top, or put things in front.
# CPack pulls versioning from there.

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# This ensures generated UI headers can find your source headers
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Make a version.h with the version in it
configure_file(
    "${CMAKE_SOURCE_DIR}/version.h.in"
    "${CMAKE_BINARY_DIR}/version.h"
)

# 1. Find Qt Components (LinguistTools handles the translations)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Help TextToSpeech LinguistTools)

# 2. Handle Dependencies (GMP/MPIR)
if(WIN32)
    find_library(GMP_LIB NAMES gmp libgmp REQUIRED)
    find_library(GMPXX_LIB NAMES gmpxx libgmpxx REQUIRED)
    set(EXTRA_LIBS ${GMPXX_LIB} ${GMP_LIB})
    get_filename_component(GMP_LIB_DIR ${GMP_LIB} DIRECTORY)
    include_directories("${GMP_LIB_DIR}/../include")
else()
    find_library(GMP_LIB gmp)
    set(EXTRA_LIBS ${GMP_LIB})
endif()

# 3. Define the Executable and Source Files
qt_add_executable(QMentat
    main.cpp
    mainwindow.cpp
    additionmodule.cpp
    additionconfigframe.cpp
    multiplicationmodule.cpp
    multiplicationconfigframe.cpp
    subtractionmodule.cpp
    subtractionconfigframe.cpp
    divisionmodule.cpp
    divisionconfigframe.cpp
    questiondisplayform.cpp
    powersmodule.cpp
    powersconfigframe.cpp
    rootsmodule.cpp
    rootsconfigframe.cpp
    questiondisplay.cpp
    qulonglongvalidator.cpp
    pieview.cpp
    statisticsdialog.cpp
    bigfixedpoint.cpp
    qbigfixedvalidator.cpp
    preferencesdialog.cpp
    preferences.cpp
    helpbrowser.cpp
    helpdialog.cpp
    mainwindow.ui
    additionconfigframe.ui
    multiplicationconfigframe.ui
    subtractionconfigframe.ui
    divisionconfigframe.ui
    questiondisplayform.ui
    powersconfigframe.ui
    rootsconfigframe.ui
    statisticsdialog.ui
    preferencesdialog.ui
    helpdialog.ui
    qmentat.qrc
)

# Or, to be more explicit and modern:
target_include_directories(QMentat PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# 4. Handle Translations (Replaces the custom TSQM compiler) 
find_package(Qt6 REQUIRED COMPONENTS LinguistTools)

# Define the translations and ensure they are generated
# This command finds lrelease and automates the .ts -> .qm conversion
qt_add_translations(QMentat
    TS_FILES qmentat_fr.ts qmentat_es.ts qmentat_it.ts qmentat_de.ts
    RESOURCE_PREFIX "/translations"
)

# If your .qrc file explicitly lists the .qm files, 
# CMake needs to know they are dependencies of the executable.
target_sources(QMentat PRIVATE ${qm_files})

target_compile_definitions(QMentat PRIVATE 
    QMENTAT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

# 5. Link everything
target_link_libraries(QMentat PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Help
    Qt6::TextToSpeech
    ${EXTRA_LIBS}
)

# Windows stuff
if(WIN32)
    find_program(WINDEPLOYQT windeployqt6 REQUIRED)
    add_custom_command(TARGET QMentat POST_BUILD
        COMMAND ${WINDEPLOYQT}
            --no-translations
            --no-system-d3d-compiler
            --no-opengl-sw
            $<TARGET_FILE:QMentat>
        COMMENT "Deploying Qt dependencies..."
    )
endif()

# 6. Installation Logic
if(WIN32)
    install(TARGETS QMentat RUNTIME DESTINATION . COMPONENT Application)
    install(FILES QMentat.png DESTINATION . COMPONENT Application)
    install(DIRECTORY documentation/ DESTINATION documentation COMPONENT Application)
    install(FILES LICENSE README.md DESTINATION . COMPONENT Application)
else()
    install(TARGETS QMentat DESTINATION bin)
    install(FILES QMentat.png DESTINATION share/icons)
    install(FILES qmentat.desktop DESTINATION share/applications)
    # Docs and license installation
    install(DIRECTORY documentation/ DESTINATION share/QMentat/documentation)
    install(FILES LICENSE README.md DESTINATION share/QMentat)
endif()

# --- CPack Packaging Configuration ---
set(CPACK_PACKAGE_NAME "QMentat")
set(CPACK_PACKAGE_VENDOR "Mike Dusseault")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Mental Arithmetic Training Software")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

if(WIN32)
    set(CPACK_GENERATOR "NSIS;ZIP")
    set(CPACK_NSIS_EXECUTABLES "QMentat" "QMentat") 
    set(CPACK_NSIS_MENU_LINKS "documentation/index.html" "Documentation")
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_NSIS_DISPLAY_NAME "QMentat Mental Math")
    set(CPACK_NSIS_HELP_LINK "https://github.com/RealGrep/QMentat")

    # Correct way to bundle DLLs: Use install() so CPack sees them
    install(FILES "${GMP_LIB_DIR}/libgmp-10.dll" 
            DESTINATION "." 
            COMPONENT Application)
    install(FILES "${GMP_LIB_DIR}/libgmpxx-4.dll" 
            DESTINATION "." 
            COMPONENT Application)
elseif(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "DEB;RPM;TGZ")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Mike Dusseault")
    set(CPACK_DEBIAN_PACKAGE_SECTION "education")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt6widgets6, libqt6help6, libqt6texttospeech6, libgmp10")
endif()
# CHECK: Looking at the above, I wonder if we can't also make the .zip file here, move stuff from the cmake-multi-platform.yml

include(CPack)

# Translation workflow (targets provided automatically by qt_add_translations above):
#   cmake --build build --target update_translations   # runs lupdate, syncs .ts files
#   cmake --build build                                # runs lrelease, recompiles .qm files