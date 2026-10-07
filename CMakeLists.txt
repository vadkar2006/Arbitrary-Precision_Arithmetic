cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MyApp 
    main.cpp
    Library/Int_Number/Int_Number.cpp
    Library/Quotient_Number/Quotient_Number.cpp
    Library/Thread_Pool/Thread_Pool.cpp
    Library/Real_Number/Real_Number.cpp
)

target_link_options(MyApp PRIVATE -static)


target_include_directories(MyApp PRIVATE ${CMAKE_SOURCE_DIR}/include)


target_include_directories(MyApp PRIVATE ${CMAKE_SOURCE_DIR})


target_compile_options(MyApp PRIVATE 
    -O3 
    -march=native 
    -ftree-vectorize
)