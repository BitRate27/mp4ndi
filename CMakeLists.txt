cmake_minimum_required(VERSION 3.10)
project(mp4ndi)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include NDI SDK and OpenCV directories
include_directories(/Users/bgillock/Documents/DistroAV/lib/ndi/)
include_directories(/Users/bgillock/Documents/source/opencv/install/include/opencv4/)

# Add executable
add_executable(mp4ndi ndi-video-sender.cpp)

# Link libraries

target_link_libraries(mp4ndi /Users/bgillock/Documents/source/opencv/install/lib/libopencv_core.dylib)
target_link_libraries(mp4ndi /Users/bgillock/Documents/source/opencv/install/lib/libopencv_videoio.dylib)
target_link_libraries(mp4ndi /Users/bgillock/Documents/source/opencv/install/lib/libopencv_imgproc.dylib)
target_link_libraries(mp4ndi /Users/bgillock/Documents/source/opencv/install/lib/libopencv_highgui.dylib)
target_link_libraries(mp4ndi /usr/local/lib/libndi.dylib)