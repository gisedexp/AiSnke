cmake_minimum_required(VERSION 3.26)
project(AiSnake)

set(CMAKE_CXX_STANDARD 17)


set(EasyX_INC D:\\Develop\\environment\\easyX\\include) # 你的include地址
set(EasyX_LINK D:\\Develop\\environment\\easyX\\lib64) # 找与自己的环境配置对应的版本
include_directories(${EasyX_INC})
link_directories(${EasyX_LINK})

link_libraries(libeasyx.a)

add_executable(AiSnake main.cpp)

