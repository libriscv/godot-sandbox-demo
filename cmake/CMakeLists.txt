cmake_minimum_required(VERSION 3.10)
project(thp)

add_subdirectory(cmake)

set(PROOT ${CMAKE_SOURCE_DIR}/..)

add_sandbox_program_at(coin ${PROOT}/scenes/scenes
    ${PROOT}/scenes/coin.cpp
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
