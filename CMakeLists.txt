cmake_minimum_required(VERSION 3.15.3)
project(NeuronNetwork)

set(CMAKE_CXX_STANDARD 17)

add_executable(NeuronNetwork main.cpp EasyBmp/EasyBMP.h EasyBmp/EasyBMP_BMP.h EasyBmp/EasyBMP.cpp
        EasyBmp/EasyBMP_DataStructures.h EasyBmp/EasyBMP_VariousBMPutilities.h
)
