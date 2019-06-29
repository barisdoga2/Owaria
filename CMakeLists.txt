cmake_minimum_required(VERSION 3.7)

project(Owaria)

include_directories(
	${PROJECT_SOURCE_DIR}/Engine
	${PROJECT_SOURCE_DIR}/Engine/Inventory.h

	${PROJECT_SOURCE_DIR}/Engine/Editors
	${PROJECT_SOURCE_DIR}/Engine/Editors/inc
	${PROJECT_SOURCE_DIR}/Engine/Editors/inc/BuildingEditor.h
	${PROJECT_SOURCE_DIR}/Engine/Editors/inc/ObjectEditor.h
	${PROJECT_SOURCE_DIR}/Engine/Editors/inc/TilemapEditor.h

	${PROJECT_SOURCE_DIR}/Engine/AssetStore
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/inc
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/inc/AssetStore.h
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/inc/AnimationAsset.h
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/inc/AnimationSet.h
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/inc/BuildingAsset.h
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/inc/ObjectAsset.h
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/inc/ObjectSet.h
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/inc/TileAsset.h
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/inc/ItemAsset.h

	${PROJECT_SOURCE_DIR}/Engine/Combat
	${PROJECT_SOURCE_DIR}/Engine/Combat/inc
	${PROJECT_SOURCE_DIR}/Engine/Combat/inc/WeaponData.h
	${PROJECT_SOURCE_DIR}/Engine/Combat/inc/MeleeWeapon.h
	${PROJECT_SOURCE_DIR}/Engine/Combat/inc/Armor.h
	${PROJECT_SOURCE_DIR}/Engine/Combat/inc/Item.h

	${PROJECT_SOURCE_DIR}/Engine/Entities
	${PROJECT_SOURCE_DIR}/Engine/Entities/inc
	${PROJECT_SOURCE_DIR}/Engine/Entities/inc/Building.h
	${PROJECT_SOURCE_DIR}/Engine/Entities/inc/GameObject.h
	${PROJECT_SOURCE_DIR}/Engine/Entities/inc/Player.h

	${PROJECT_SOURCE_DIR}/Engine/Map
	${PROJECT_SOURCE_DIR}/Engine/Map/inc
	${PROJECT_SOURCE_DIR}/Engine/Map/inc/Map.h
	${PROJECT_SOURCE_DIR}/Engine/Map/inc/Tile.h

	${PROJECT_SOURCE_DIR}/Engine/Rendering
	${PROJECT_SOURCE_DIR}/Engine/Rendering/inc
	${PROJECT_SOURCE_DIR}/Engine/Rendering/inc/SFMLDebugDraw.h
	${PROJECT_SOURCE_DIR}/Engine/Rendering/inc/Animation.h
	${PROJECT_SOURCE_DIR}/Engine/Rendering/inc/Background.h
	${PROJECT_SOURCE_DIR}/Engine/Rendering/inc/Camera.h
	${PROJECT_SOURCE_DIR}/Engine/Rendering/inc/Tileset.h

	${PROJECT_SOURCE_DIR}/Engine/Utils
	${PROJECT_SOURCE_DIR}/Engine/Utils/inc
	${PROJECT_SOURCE_DIR}/Engine/Utils/MarchingSquares
	${PROJECT_SOURCE_DIR}/Engine/Utils/MarchingSquares/MarchingSolution.h
	${PROJECT_SOURCE_DIR}/Engine/Utils/MarchingSquares/MarchingSquares.h
	${PROJECT_SOURCE_DIR}/Engine/Utils/inc/b2Utils.h
	${PROJECT_SOURCE_DIR}/Engine/Utils/inc/ContactData.h
	${PROJECT_SOURCE_DIR}/Engine/Utils/inc/ContactListener.h
	${PROJECT_SOURCE_DIR}/Engine/Utils/inc/ioUtils.h
	${PROJECT_SOURCE_DIR}/Engine/Utils/inc/mathUtils.h
	${PROJECT_SOURCE_DIR}/Engine/Utils/inc/XMLUtils.h
)

set(sourcefiles
	${PROJECT_SOURCE_DIR}/Engine/Main.cpp
	${PROJECT_SOURCE_DIR}/Engine/Inventory.cpp

	${PROJECT_SOURCE_DIR}/Engine/Editors
	${PROJECT_SOURCE_DIR}/Engine/Editors/BuildingEditor.cpp
	${PROJECT_SOURCE_DIR}/Engine/Editors/ObjectEditor.cpp
	${PROJECT_SOURCE_DIR}/Engine/Editors/TilemapEditor.cpp

	${PROJECT_SOURCE_DIR}/Engine/AssetStore
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/AssetStore.cpp
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/AnimationAsset.cpp
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/AnimationSet.cpp
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/BuildingAsset.cpp
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/ObjectAsset.cpp
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/ObjectSet.cpp
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/TileAsset.cpp
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/ItemAsset.cpp

	${PROJECT_SOURCE_DIR}/Engine/Combat
	${PROJECT_SOURCE_DIR}/Engine/Combat/WeaponData.cpp
	${PROJECT_SOURCE_DIR}/Engine/Combat/MeleeWeapon.cpp
	${PROJECT_SOURCE_DIR}/Engine/Combat/Armor.cpp

	${PROJECT_SOURCE_DIR}/Engine/Entities
	${PROJECT_SOURCE_DIR}/Engine/Entities/Building.cpp
	${PROJECT_SOURCE_DIR}/Engine/Entities/GameObject.cpp
	${PROJECT_SOURCE_DIR}/Engine/Entities/Player.cpp

	${PROJECT_SOURCE_DIR}/Engine/Map
	${PROJECT_SOURCE_DIR}/Engine/Map/Map.cpp
	${PROJECT_SOURCE_DIR}/Engine/Map/Tile.cpp

	${PROJECT_SOURCE_DIR}/Engine/Rendering
	${PROJECT_SOURCE_DIR}/Engine/Rendering/SFMLDebugDraw.cpp
	${PROJECT_SOURCE_DIR}/Engine/Rendering/Animation.cpp
	${PROJECT_SOURCE_DIR}/Engine/Rendering/Background.cpp
	${PROJECT_SOURCE_DIR}/Engine/Rendering/Camera.cpp
	${PROJECT_SOURCE_DIR}/Engine/Rendering/Tileset.cpp

	${PROJECT_SOURCE_DIR}/Engine/Utils
	${PROJECT_SOURCE_DIR}/Engine/Utils/MarchingSquares
	${PROJECT_SOURCE_DIR}/Engine/Utils/MarchingSquares/MarchingSquares.cpp
)

set(CMAKE_TOOLCHAIN_FILE "${env.VCKG_PATH}\\scripts\\buildsystems\\vcpkg.cmake")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

find_package(Threads REQUIRED)
find_package(SFML 2.5 COMPONENTS system window audio graphics main REQUIRED)
find_package(unofficial-box2d CONFIG REQUIRED)
find_package(tinyxml2 CONFIG REQUIRED)
find_package(TGUI CONFIG REQUIRED)

add_executable(Owaria  ${sourcefiles})

target_compile_features(Owaria PRIVATE cxx_std_17)

target_link_libraries(Owaria ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Owaria sfml-system sfml-window sfml-audio sfml-graphics sfml-main)
target_link_libraries(Owaria unofficial::box2d::Box2D)
target_link_libraries(Owaria tinyxml2::tinyxml2)
target_link_libraries(Owaria tgui)
