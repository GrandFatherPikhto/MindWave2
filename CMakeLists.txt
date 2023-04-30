cmake_minimum_required(VERSION 3.23)
set(PROJECT_NAME MindWave2)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(Boost_NO_WARN_NEW_VERSIONS 1)
set(Boost_DEBUG "ON")
set(Boost_VERBOSE "ON")
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS system filesystem)


set(CMAKE_PREFIX_PATH "D:/Qt/6.4.2/msvc2019_64/")
#set(CMAKE_PREFIX_PATH "D:/QtSource/Qt6/msvc2022_64")

# set(BOOST_ROOT "D:/Libraries/boost_1_81_0")
# find_package(Boost COMPONENTS program_options REQUIRED)

find_package(Qt6 REQUIRED COMPONENTS
        Linguist
        Core
        Gui
        Widgets
        Sql
        Bluetooth
        SerialPort
        OpenGL
        Charts
        Svg
        Sql
        SvgWidgets
        LinguistTools)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}
        src/res/${PROJECT_NAME}.qrc
        src/Application/MindWave2.cpp
        src/MainWindow/MainWindow.cpp
        src/TGParser/PacketParser.cpp
        src/TGParser/TGParser.cpp
        src/MainWindow/MainWindow.h
        src/Application/MindWave2.h
        src/TGParser/TGParser.h
        src/TGParser/PacketParser.h
        src/DocWidgets/EEGBar/EEGBar.cpp
        src/DocWidgets/EEGBar/EEGBar.h
        src/Connectors/Generator/Generator.cpp
        src/Connectors/Generator/Generator.h
        src/DocWidgets/EEGBar/EEGBarWidget.cpp
        src/DocWidgets/EEGBar/EEGBarWidget.h
        src/DocWidgets/EEGWave/EEGWaveWidget.cpp
        src/DocWidgets/EEGWave/EEGWaveWidget.h
        src/DocWidgets/EEGWave/EEGWave.cpp
        src/DocWidgets/EEGWave/EEGWave.h
        src/Connectors/Bluetooth/BTServices.cpp
        src/Connectors/Bluetooth/BTServices.h
        src/Connectors/Bluetooth/Bluetooth.cpp
        src/Connectors/Bluetooth/Bluetooth.h
        src/Connectors/Bluetooth/DialogBT.cpp
        src/Connectors/Bluetooth/DialogBT.h
        src/Models/Database.cpp
        src/Models/Database.h
        src/Models/BTDevicesModel.cpp
        src/Models/BTDevicesModel.h
        src/Models/Proxy/ModelProxy.cpp
        src/Models/Proxy/ModelProxy.h
        src/Models/Proxy/ProxyColumns.h
        src/Models/Proxy/ProxyColumn.h
        src/Models/Delegates/TimeDelegate.cpp
        src/Models/Delegates/TimeDelegate.h
        src/Connectors/Bluetooth/BTReader.cpp
        src/Connectors/Bluetooth/BTReader.h
        src/SerialPort/SerialPort.h
        src/SerialPort/SerialPort.cpp
        src/SerialPort/DialogSP.h
        src/SerialPort/DialogSP.cpp
        src/DocWidgets/EEGLog/EEGLogWidget.cpp
        src/DocWidgets/EEGLog/EEGLogWidget.h
        src/DocWidgets/EEGLog/EEGLog.cpp
        src/DocWidgets/EEGLog/EEGLog.h
        src/TGParser/EEGData.h
        src/TGParser/EEGData.cpp
        src/DocWidgets/EEGStatus/EEGStatus.h
        src/DocWidgets/EEGStatus/EEGStatus.cpp
        src/DocWidgets/EEGStatus/EEGStatusWidget.cpp
        src/DocWidgets/EEGStatus/EEGStatusWidget.h
        src/Models/EEGNormDataModel.cpp
        src/Models/EEGNormDataModel.h
        src/DocWidgets/EEGSessions/EEGSessionWidget.cpp
        src/DocWidgets/EEGSessions/EEGSessionWidget.h
        src/DocWidgets/EEGSessions/EEGSessionWidget.h
        src/DocWidgets/EEGSessions/EEGSession.cpp
        src/DocWidgets/EEGSessions/EEGSession.h
        src/Models/EEGSessionModel.cpp
        src/Models/EEGSessionModel.h
        src/Models/EEGSessionData.cpp
        src/Models/EEGSessionData.h src/MainWindow/DialogSession.cpp src/MainWindow/DialogSession.h)

target_link_libraries( ${PROJECT_NAME}
        Qt6::Linguist
        Qt6::Core
        Qt6::Gui
        Qt6::Sql
        Qt6::Widgets
        Qt6::Sql
        Qt6::Bluetooth
        Qt6::SerialPort
        Qt6::Svg
        Qt6::SvgWidgets
        Qt6::OpenGL
        Qt6::Charts
        ${Boost_LIBRARIES})


#install(TARGETS ${PROJECT_NAME}
#        BUNDLE  DESTINATION .
#        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#        )

set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION "l10n")
qt6_add_translations(${PROJECT_NAME}
        TS_FILES src/res/${PROJECT_NAME}_ru_RU.xml
        QM_FILES_OUTPUT_VARIABLE QM_FILES)
# install(FILES ${QM_FILES} DESTINATION QM_FILES)


#qt_add_lupdate(${PROJECT_NAME} TS_FILES ${PROJECT_NAME}_ru_RU.xml)
#qt_add_lrelease(${PROJECT_NAME}
#        TS_FILES src/res/${PROJECT_NAME}_ru_RU.xml
#        QM_FILES_OUTPUT_VARIABLE QM_FILES)
#qt_add_resources(${PROJECT_NAME} translations
#        PREFIX /i18n
#        BASE ${CMAKE_CURRENT_BINARY_DIR}
#        FILES "${QM_FILES}")

if (WIN32)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
