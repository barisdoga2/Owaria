cmake_minimum_required(VERSION 3.7)

project(Owaria)

include_directories(
	${PROJECT_SOURCE_DIR}/Engine

	${PROJECT_SOURCE_DIR}/Engine/Entities
	${PROJECT_SOURCE_DIR}/Engine/Entities/Player.h

	${PROJECT_SOURCE_DIR}/Engine/Map
	${PROJECT_SOURCE_DIR}/Engine/Map/Map.h
	${PROJECT_SOURCE_DIR}/Engine/Map/Tile.h

	${PROJECT_SOURCE_DIR}/Engine/Rendering
	${PROJECT_SOURCE_DIR}/Engine/Rendering/Animation.h
	${PROJECT_SOURCE_DIR}/Engine/Rendering/Camera.h

	${PROJECT_SOURCE_DIR}/Engine/Utils
	${PROJECT_SOURCE_DIR}/Engine/Utils/b2Utils.h
	${PROJECT_SOURCE_DIR}/Engine/Utils/MyContactListener.h
	${PROJECT_SOURCE_DIR}/Engine/Utils/Settings.h
	${PROJECT_SOURCE_DIR}/Engine/Utils/MarchingSquares
	${PROJECT_SOURCE_DIR}/Engine/Utils/MarchingSquares/MarchingSolution.h
	${PROJECT_SOURCE_DIR}/Engine/Utils/MarchingSquares/MarchingSquares.h
)

set(sourcefiles
	${PROJECT_SOURCE_DIR}/Engine/Main.cpp

	${PROJECT_SOURCE_DIR}/Engine/Entities
	${PROJECT_SOURCE_DIR}/Engine/Entities/Player.cpp

	${PROJECT_SOURCE_DIR}/Engine/Map
	${PROJECT_SOURCE_DIR}/Engine/Map/Map.cpp
	${PROJECT_SOURCE_DIR}/Engine/Map/Tile.cpp

	${PROJECT_SOURCE_DIR}/Engine/Rendering
	${PROJECT_SOURCE_DIR}/Engine/Rendering/Animation.cpp
	${PROJECT_SOURCE_DIR}/Engine/Rendering/Camera.cpp

	${PROJECT_SOURCE_DIR}/Engine/Utils
	${PROJECT_SOURCE_DIR}/Engine/Utils/b2Utils.cpp
	${PROJECT_SOURCE_DIR}/Engine/Utils/MyContactListener.cpp
	${PROJECT_SOURCE_DIR}/Engine/Utils/MarchingSquares
	${PROJECT_SOURCE_DIR}/Engine/Utils/MarchingSquares/MarchingSquares.cpp
)

set(CMAKE_TOOLCHAIN_FILE "C:\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake")
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
find_package(unofficial-box2d CONFIG REQUIRED)

set(
	CMAKE_MODULE_PATH
	"${CMAKE_SOURCE_DIR}/cmake_modules"
	${CMAKE_MODULE_PATH}
)
find_package(SFML 2.5 COMPONENTS system window graphics main REQUIRED)

add_executable(Owaria  ${sourcefiles})
target_compile_features(
	Owaria
	PRIVATE
	cxx_std_17)
target_link_libraries(Owaria sfml-system sfml-window sfml-graphics sfml-main ${CMAKE_THREAD_LIBS_INIT} unofficial::box2d::Box2D)
