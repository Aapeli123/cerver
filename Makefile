BUILD_DIR := ./out
SOURCE_DIR := ./src
COMPILE_FILES := $(shell find ${SOURCE_DIR} -name '*.c')
COMPILE_FLAGS := -lssl -pthread -Wall
COMPILE_FLAGS_DBG := -g
COMPILE_FLAGS_PROD := -O3
CC := clang
build:
	mkdir -p ${BUILD_DIR}/release
	${CC} -o ${BUILD_DIR}/release/cerver ${COMPILE_FILES} ${COMPILE_FLAGS} ${COMPILE_FLAGS_PROD}
clean:
	rm -rf ${BUILD_DIR}
debug:
	mkdir -p ${BUILD_DIR}/debug
	${CC} -o ${BUILD_DIR}/debug/cerver ${COMPILE_FILES} ${COMPILE_FLAGS} ${COMPILE_FLAGS_DBG}