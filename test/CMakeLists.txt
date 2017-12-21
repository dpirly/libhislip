include_directories(../src/include)

add_executable(server server.c)
#add_executable(clinet client.c)

#target_link_libraries(clinet ../src/libhislip.so)
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT} "-lrt -ldl" /home/k6206/Desktop/libhislip/build/src/libhislip.so)
