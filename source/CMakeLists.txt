add_executable(Challenges)
set_target_properties(Challenges PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
target_include_directories(Challenges PUBLIC ".")

file(GLOB_RECURSE sources CONFIGURE_DEPENDS
    "utility/*.cpp" "utility/*.hpp"
    #"Y2015/*.cpp" 
    #"Y2016/*.cpp" 
    #"Y2017/*.cpp"
    #"Y2018/*.cpp"
    "Y2019/*.cpp"
    #"Y2020/*.cpp"
    #"Y2021/*.cpp"
    #"Y2022/*.cpp"
    #"Y2023/*.cpp"
    #"Y2024/*.cpp" 
    "main.cpp"
)

target_sources(Challenges PRIVATE ${sources})
