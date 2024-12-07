add_executable(Challenges)
set_target_properties(Challenges PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
target_include_directories(Challenges PUBLIC ".")

file(GLOB_RECURSE sources CONFIGURE_DEPENDS
    "Utility/*.cpp" "Utility/*.hpp"
    "common.cpp" "common.hpp"
    "Y2015/*.cpp" 
    "Y2016/*.cpp" 
    "Y2024/*.cpp" 
    "main.cpp"
)

target_sources(Challenges PRIVATE ${sources})

target_link_libraries(Challenges PUBLIC
    glm::glm
)

