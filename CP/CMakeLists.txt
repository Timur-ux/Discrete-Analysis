add_executable(CP_main main.cpp)
target_link_libraries(CP_main strings_lib)


file(GLOB_RECURSE PROJ_SRC LIST_DIRECTORIES FALSE ./src/*.cpp)

add_library(CP_proj_lib ${PROJ_SRC})
target_link_libraries(CP_proj_lib strings_lib)
target_include_directories(CP_proj_lib PUBLIC "./include/")

add_executable(CP_main_proj main_proj.cpp)
target_link_libraries(CP_main_proj strings_lib CP_proj_lib)
target_include_directories(CP_main_proj PUBLIC "./include/")

target_compile_definitions(CP_proj_lib PUBLIC "DEBUG")
