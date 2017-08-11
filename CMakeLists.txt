cmake_minimum_required(VERSION 2.8.3)
project(jpg2bmp)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(${CMAKE_PROJECT_NAME}lib src/BitReader.cpp
                                     src/BitWriter.cpp
                                     src/BmpFileHeader.cpp
                                     src/BmpImage.cpp
                                     src/BmpInfoHeader.cpp
                                     src/ByteHandler.cpp
                                     src/ColorSpaceConverter.cpp
                                     src/Dht.cpp
                                     src/EXIFextractor.cpp
                                     src/EXIFtag.cpp
                                     src/IFD0tag.cpp
                                     src/IFD1tag.cpp
                                     src/JFIFextractor.cpp
                                     src/JpegHeader.cpp
                                     src/RgbQuad.cpp)


add_executable(${CMAKE_PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/main.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}lib)


