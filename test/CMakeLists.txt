include_directories(../src/include)

add_executable(server server.c)
add_executable(clinet client.c)

target_link_libraries(clinet hislip)
target_link_libraries(server hislip)
