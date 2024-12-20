cmake_minimum_required(VERSION 3.27)
project(Starter CXX)


build_application(
    SOURCES Application.cpp
    PACKAGES glfw3 imguidocking
    LINK_PACKAGES glfw imguidocking::imguidocking
)
