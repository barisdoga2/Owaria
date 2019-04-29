cmake_minimum_required(VERSION 3.7)

project(Owaria)

include_directories(
	${PROJECT_SOURCE_DIR}/Engine
	${PROJECT_SOURCE_DIR}/Engine/Settings.h

	${PROJECT_SOURCE_DIR}/Engine/Map.h
	${PROJECT_SOURCE_DIR}/Engine/Tile.h
	${PROJECT_SOURCE_DIR}/Engine/Player.h
)

set(sourcefiles
	${PROJECT_SOURCE_DIR}/Engine/Map.cpp
	${PROJECT_SOURCE_DIR}/Engine/Tile.cpp
	${PROJECT_SOURCE_DIR}/Engine/Player.cpp

	${PROJECT_SOURCE_DIR}/Engine/Main.cpp
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
