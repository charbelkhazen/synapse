cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories for header files
include_directories(
  "C:/libraries_charbel/libjpeg-turbo64/include"
  "C:/libraries_charbel/libwebp-1.4.0-windows-x64/include"
  "C:/libraries_charbel/sqlite_lib"

)

# Library directories for .lib files
link_directories(
  "C:/libraries_charbel/libjpeg-turbo64/lib"
  "C:/libraries_charbel/libwebp-1.4.0-windows-x64/lib"
)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Include OpenCV directories
include_directories(${OpenCV_INCLUDE_DIRS})



# Add the compress_jpeg library
#add_library(compress_jpeg_lib compress_jpeg.cpp)

# Link jpeg library to compress_jpeg_lib
#target_link_libraries(compress_jpeg_lib jpeg-static)


# Add console application
#add_executable(console_app console_app.cpp)


# Add the executable with all source files
add_executable(console_app
  console_app.cpp
  compress_jpeg.cpp
  display_img.cpp
  file_mngmt.cpp
  sqlite.cpp
)

# Link against the required libraries
target_link_libraries(console_app
  jpeg-static          # For libjpeg-turbo
  libwebp              # For libwebp
  ${OpenCV_LIBS}       # For OpenCV
  "C:/libraries_charbel/sqlite_lib/sqlite3.lib"
)