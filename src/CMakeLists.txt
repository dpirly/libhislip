include_directories(
  ../src/include
)

add_library(hislip SHARED
 client.c
 message.c
 server.c
 session.c
 tcp.c)

target_link_libraries(hislip ${CMAKE_THREAD_LIBS_INIT})