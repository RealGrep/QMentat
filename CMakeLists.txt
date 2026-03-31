cmake_minimum_required(VERSION 3.16)
project(QMentat VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# This ensures generated UI headers can find your source headers
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 1. Find Qt Components (LinguistTools handles the translations)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Help LinguistTools)

# 2. Handle Dependencies (GMP/MPIR)
if(WIN32)
    # Make sure to edit these to point to your MPIR and Boost 
    target_include_directories(QMentat PRIVATE "C:/mpir-2.6.0/lib/Win32/Release" "C:/boost_1_53_0")
    link_directories("C:/mpir-2.6.0/dll/Win32/Release" "C:/mpir-2.6.0/lib/Win32/Release")
    set(EXTRA_LIBS mpirxx mpir)
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
    TS_FILES qmentat_fr.ts qmentat_es.ts
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
    ${EXTRA_LIBS}
)

# 6. Installation Logic 
install(TARGETS QMentat DESTINATION bin)
install(FILES QMentat.png DESTINATION share/icons)
install(FILES qmentat.desktop DESTINATION share/applications)
# Docs and license installation
install(DIRECTORY documentation/ DESTINATION share/QMentat/documentation)
install(FILES LICENSE README.md DESTINATION share/QMentat)
