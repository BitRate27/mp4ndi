cmake_minimum_required(VERSION 3.10)
project(mp4ndi)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include NDI SDK and OpenCV directories
include_directories(/Users/bgillock/Documents/DistroAV/lib/ndi/)
include_directories(/Users/bgillock/Documents/opencv/install/include/opencv4/)

# Add executable
add_executable(mp4ndi ndi-video-sender.cpp)

# Find required frameworks
find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)
find_library(AVFOUNDATION_FRAMEWORK AVFoundation)
find_library(COREMEDIA_FRAMEWORK CoreMedia)
find_library(COREVIDEO_FRAMEWORK CoreVideo)

# Find OpenCV components
find_package(OpenCV REQUIRED PATHS /Users/bgillock/Documents/opencv/install NO_DEFAULT_PATH)

# Link libraries
target_link_libraries(mp4ndi 
    ${OpenCV_LIBS}
    /usr/local/lib/libndi.dylib
    -lpthread
    -ldl
    -lz
    ${COREFOUNDATION_FRAMEWORK}
    ${AVFOUNDATION_FRAMEWORK}
    ${COREMEDIA_FRAMEWOpRK}
    ${COREVIDEO_FRAMEWORK}
)