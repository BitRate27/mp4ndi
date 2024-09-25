# Play a MP4 file over NDI
##Requirements 
###Running: 
NDI Tools downloaded and installed on your computer
Download macos exe from release page of this repo
Usage: mp4ndi <mp4 file name> [ndi name]
###Building: 
git clone https://github.com/opencv/opencv.git
cd opencv
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_SHARED_LIBS=OFF \
      -DBUILD_EXAMPLES=OFF \
      -DBUILD_TESTS=OFF \
      -DBUILD_PERF_TESTS=OFF \
      -DWITH_FFMPEG=ON \
      -DBUILD_opencv_apps=OFF \
      -DBUILD_opencv_js=OFF \
      -DBUILD_JAVA=OFF \
      -DBUILD_opencv_python2=OFF \
      -DBUILD_opencv_python3=OFF \
      -DCMAKE_INSTALL_PREFIX=/path/to/opencv/install \
      ..
In cmake_install.cmake change CMAKE_INSTALL_PREFIX to where opencv install is
make -j$(nproc)
make install



