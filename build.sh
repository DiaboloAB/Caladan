#!/bin/bash

# Function to display usage
usage() {
    echo "Usage: $0 [build|rebuild|clean|fullclean|run]"
    exit 1
}

# Function to build the project
build() {
    cd shaders && ./compileShaders.sh && cd ..
    mkdir -p build && cd build && cmake .. && make
}

# Function to rebuild the project
rebuild() {
    mkdir -p build && cd build && cmake .. && make clean && make
}

# Function to clean the build directory
clean() {
    if [ -d "build" ]; then
        cd build && make clean
    else
        echo "Build directory does not exist."
    fi
}

# Function to fully clean the build directory
fullclean() {
    rm -rf build
}

# Function to run the project
run() {
    if [ -f "build/Caladan" ]; then
        ./build/Caladan
    else
        echo "Executable not found. Please build the project first."
    fi
}

# Check the number of arguments
if [ $# -ne 1 ]; then
    usage
fi

# Parse the command
case $1 in
    build)
        build
        ;;
    rebuild)
        rebuild
        ;;
    clean)
        clean
        ;;
    fullclean)
        fullclean
        ;;
    run)
        run
        ;;
    *)
        usage
        ;;
esac
