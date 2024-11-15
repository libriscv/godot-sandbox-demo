cmake_minimum_required(VERSION 3.10)
project(json_diff)

add_subdirectory(cmake)

set(PROOT ${CMAKE_SOURCE_DIR}/..)

add_sandbox_program_at(json_diff ${PROOT}
	${PROOT}/json_diff/json_diff.cpp
	# Can add more cpp files.
)

# target_add_include_directories(json_diff PRIVATE "include")

include(FetchContent)
target_include_directories(json_diff PRIVATE ${PROOT})
FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG        master
)
FetchContent_MakeAvailable(json)
target_link_libraries(json_diff PUBLIC nlohmann_json::nlohmann_json)
