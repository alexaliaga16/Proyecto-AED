cmake_minimum_required(VERSION 3.10)
project(ProyectoAED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(main
    main.cpp
    SparseMatrix.cpp
    MainWindow.cpp
)

target_link_libraries(main Qt6::Widgets)