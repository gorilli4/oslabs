cmake_minimum_required(VERSION 3.22)
project(LabWork1 C)

set(CMAKE_C_STANDARD 99)

# Добавляем исполняемые файлы
add_executable(parent src/parent.c)
add_executable(child src/child.c)