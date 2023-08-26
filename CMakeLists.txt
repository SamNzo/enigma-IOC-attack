cmake_minimum_required(VERSION 3.12)
project(Enigma-IoC-Attack)

set(CMAKE_CXX_STANDARD 11)

# Add the source files
set(SOURCES
    enigma/src/Rotor.cpp
    enigma/src/Reflector.cpp
    enigma/src/Plugboard.cpp
    enigma/src/Enigma.cpp
    src/ioc.cpp
    src/findRotor.cpp
    src/main.cpp
)

# Add the include directories
include_directories(${PROJECT_SOURCE_DIR})

# Create the executable
add_executable(ioc-attack ${SOURCES})
