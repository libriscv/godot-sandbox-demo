cmake_minimum_required(VERSION 3.10)
project(demo)

option(MIR_ENABLED "Enable MIR" OFF)

add_subdirectory(cmake)

set(PROOT ${CMAKE_SOURCE_DIR}/..)

add_sandbox_program_at(coin ${PROOT}/scenes/coin
    ${PROOT}/scenes/coin/coin.cpp
)

add_sandbox_program_at(player ${PROOT}/scenes/player
	${PROOT}/scenes/player/player.cpp
)

add_sandbox_program_at(killzone ${PROOT}/scenes/killzone
    ${PROOT}/scenes/killzone/killzone.cpp
)

add_sandbox_program_at(slime ${PROOT}/scenes/slime
    ${PROOT}/scenes/slime/slime.cpp
)



add_sandbox_program_at(test ${PROOT}/test
	${PROOT}/test/test.cpp
	${PROOT}/test/new_script.cpp
)

if (MIR_ENABLED)
	include(FetchContent)
	FetchContent_Declare(libmir
		GIT_REPOSITORY https://github.com/vnmakarov/mir
		GIT_TAG        master
	)
	FetchContent_MakeAvailable(libmir)

	target_link_libraries(test PRIVATE mir_static)
	target_include_directories(test PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/_deps/libmir-src")
endif()
