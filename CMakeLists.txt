cmake_minimum_required(VERSION 3.10)

project(MatchingEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Qt confing
set(CMAKE_AUTOMOC ON) # Active la génération de MOC (Meta-Object Compiler)
set(CMAKE_AUTORCC ON) # Active la compilation automatique des fichiers de ressources (.qrc)
set(CMAKE_AUTOUIC ON) # Active la compilation automatique des fichiers d'interface utilisateur (.ui)

# personnel remplacer par votre Qt
set(CMAKE_PREFIX_PATH "C:/Qt/6.10.0/mingw_64")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Charts
        REQUIRED)

include_directories(MatchingEngine/include)

add_executable(MatchingEngine
        MatchingEngine/src/Main.cpp
        MatchingEngine/src/Order.cpp
        MatchingEngine/src/Instrument.cpp
        MatchingEngine/src/OrderBook.cpp
        MatchingEngine/src/Utils.cpp
        MatchingEngine/src/InstrumentManager.cpp
        MatchingEngine/src/MatchingEngine.cpp
        MatchingEngine/include/MainWindow.h
        MatchingEngine/src/MainWindow.cpp
        MatchingEngine/include/CreateInstrumentWidget.h
        MatchingEngine/src/CreateInstrumentWidget.cpp
)


target_link_libraries(MatchingEngine
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Charts
)