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
find_package(OpenSSL REQUIRED)
target_link_libraries(cerver PRIVATE OpenSSL::SSL)
target_link_libraries(cerver PRIVATE OpenSSL::Crypto)

