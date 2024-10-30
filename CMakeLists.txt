cmake_minimum_required(VERSION 3.10)
project(behavior_tree_python)

find_package(pybind11 REQUIRED)
find_package(behaviortree_cpp_v3 REQUIRED)

pybind11_add_module(behavior_tree_module binding.cpp)
target_link_libraries(behavior_tree_module PRIVATE behaviortree_cpp_v3)
