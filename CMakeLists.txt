cmake_minimum_required(VERSION 3.7)

project(Owaria)

include_directories(
	${PROJECT_SOURCE_DIR}/Engine

	${PROJECT_SOURCE_DIR}/Engine/Editors
	${PROJECT_SOURCE_DIR}/Engine/Editors/BuildingEditor.h
	${PROJECT_SOURCE_DIR}/Engine/Editors/ObjectEditor.h
	${PROJECT_SOURCE_DIR}/Engine/Editors/TilemapEditor.h

	${PROJECT_SOURCE_DIR}/Engine/AssetStore
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/AssetStore.h
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/BuildingAsset.h
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/ObjectAsset.h
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/ObjectSet.h
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/TileAsset.h

	${PROJECT_SOURCE_DIR}/Engine/Entities
	${PROJECT_SOURCE_DIR}/Engine/Entities/Building.h
	${PROJECT_SOURCE_DIR}/Engine/Entities/GameObject.h
	${PROJECT_SOURCE_DIR}/Engine/Entities/Player.h

	${PROJECT_SOURCE_DIR}/Engine/Map
	${PROJECT_SOURCE_DIR}/Engine/Map/Map.h
	${PROJECT_SOURCE_DIR}/Engine/Map/Tile.h

	${PROJECT_SOURCE_DIR}/Engine/Rendering
	${PROJECT_SOURCE_DIR}/Engine/Rendering/debugDraw
	${PROJECT_SOURCE_DIR}/Engine/Rendering/debugDraw/SFMLDebugDraw.h
	${PROJECT_SOURCE_DIR}/Engine/Rendering/Animation.h
	${PROJECT_SOURCE_DIR}/Engine/Rendering/Background.h
	${PROJECT_SOURCE_DIR}/Engine/Rendering/Camera.h
	${PROJECT_SOURCE_DIR}/Engine/Rendering/Tileset.h

	${PROJECT_SOURCE_DIR}/Engine/Utils
	${PROJECT_SOURCE_DIR}/Engine/Utils/MarchingSquares
	${PROJECT_SOURCE_DIR}/Engine/Utils/MarchingSquares/MarchingSolution.h
	${PROJECT_SOURCE_DIR}/Engine/Utils/MarchingSquares/MarchingSquares.h
	${PROJECT_SOURCE_DIR}/Engine/Utils/b2Utils.h
	${PROJECT_SOURCE_DIR}/Engine/Utils/ContactData.h
	${PROJECT_SOURCE_DIR}/Engine/Utils/ContactListener.h
	${PROJECT_SOURCE_DIR}/Engine/Utils/ioUtils.h
	${PROJECT_SOURCE_DIR}/Engine/Utils/mathUtils.h
	${PROJECT_SOURCE_DIR}/Engine/Utils/XMLUtils.h
)

set(sourcefiles
	${PROJECT_SOURCE_DIR}/Engine/Main.cpp

	${PROJECT_SOURCE_DIR}/Engine/Editors
	${PROJECT_SOURCE_DIR}/Engine/Editors/BuildingEditor.cpp
	${PROJECT_SOURCE_DIR}/Engine/Editors/ObjectEditor.cpp
	${PROJECT_SOURCE_DIR}/Engine/Editors/TilemapEditor.cpp

	${PROJECT_SOURCE_DIR}/Engine/AssetStore
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/AssetStore.cpp
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/BuildingAsset.cpp
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/ObjectAsset.cpp
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/ObjectSet.cpp
	${PROJECT_SOURCE_DIR}/Engine/AssetStore/TileAsset.cpp

	${PROJECT_SOURCE_DIR}/Engine/Entities
	${PROJECT_SOURCE_DIR}/Engine/Entities/Building.cpp
	${PROJECT_SOURCE_DIR}/Engine/Entities/GameObject.cpp
	${PROJECT_SOURCE_DIR}/Engine/Entities/Player.cpp

	${PROJECT_SOURCE_DIR}/Engine/Map
	${PROJECT_SOURCE_DIR}/Engine/Map/Map.cpp
	${PROJECT_SOURCE_DIR}/Engine/Map/Tile.cpp

	${PROJECT_SOURCE_DIR}/Engine/Rendering
	${PROJECT_SOURCE_DIR}/Engine/Rendering/debugDraw
	${PROJECT_SOURCE_DIR}/Engine/Rendering/debugDraw/SFMLDebugDraw.cpp
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
