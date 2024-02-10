cmake_minimum_required(VERSION 3.10)

project (SEA_BATTLE VERSION 1.0.0)

find_package(Boost COMPONENTS locale REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics audio window system REQUIRED)

set(SOURCE_FILE
    GameFiles/source/Elements/Field.cpp
    GameFiles/source/Elements/Ship.cpp
    GameFiles/source/Interface/Gameplay.cpp
    GameFiles/source/Interface/Interface.cpp
    GameFiles/source/Interface/settings.cpp
    GameFiles/source/Managers/FileIOManager.cpp
    GameFiles/source/Managers/TextureManager.cpp
    GameFiles/source/Managers/Timer.cpp
    GameFiles/source/Widgets/Button.cpp
    GameFiles/main.cpp
)

file(COPY GameFiles/app_data/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/app_data)
file(COPY GameFiles/app_data/textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/app_data)

add_executable(sea_battle ${SOURCE_FILE})

target_link_libraries(sea_battle PRIVATE Boost::locale)
target_link_libraries(sea_battle PRIVATE sfml-graphics sfml-audio sfml-window sfml-system)
target_include_directories(sea_battle PRIVATE ${CMAKE_BINARY_DIR})

set_target_properties(sea_battle PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
