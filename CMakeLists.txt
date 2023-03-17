cmake_minimum_required(VERSION 3.1.0)

project(CERVER)

set(SOURCE_FILES
        src/main.c

        src/config/commands.c
        src/config/config.c
        src/config/config_t.c

        src/dir/dir.c
        src/dir/filereader.c

        src/hashmap/hash.c
        src/hashmap/hashmap.c

        src/http/http.c

        src/mime/mime.c

        src/server/handler.c
        src/server/response.c
        src/server/server.c

        src/threads/thread_pool.c)

add_executable(cerver ${SOURCE_FILES})

# Pthreads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(cerver PRIVATE Threads::Threads)

# Openssl
target_include_directories(cerver PRIVATE ${PROJECT_SOURCE_DIR}/openssl/include)
#link_directories(${PROJECT_SOURCE_DIR}/lib)
find_library(OPENSSL ssl HINTS ${PROJECT_SOURCE_DIR}/lib)
find_library(CRYPTO crypto HINTS ${PROJECT_SOURCE_DIR}/lib)
if (${CMAKE_SYSTEM_NAME} STREQUAL Windows)
	target_link_libraries(cerver PRIVATE ${PROJECT_SOURCE_DIR}/lib/libssl-3-x64.dll ${PROJECT_SOURCE_DIR}/lib/libcrypto-3-x64.dll)
else()
	message(Openssl location: ${OPENSSL})
	target_link_libraries(cerver PRIVATE ${OPENSSL} ${CRYPTO})
endif ()



# Winsock
if (${CMAKE_SYSTEM_NAME} STREQUAL Windows)
    message("Linking windows libs")
    target_link_libraries(cerver PRIVATE ws2_32)
    target_link_libraries(cerver PRIVATE wsock32)
endif ()
