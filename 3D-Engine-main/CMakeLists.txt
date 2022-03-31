add_library(pico_3d main.c)

target_link_libraries(pico_3d pico_stdlib hardware_pio)

target_include_directories(pico_lcd PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")