BUILD_DIR := "./build"
SOURCE_DIR := "./src"
COMPILE_FILES := $(shell find ${SOURCE_DIR} -name '*.c')
COMPILE_FLAGS := -lssl -pthread -O3 -Wall 
build:
	cc -o ${BUILD_DIR}/cerver ${COMPILE_FLAGS} ${COMPILE_FILES} 