add_executable(test_particles test_particles.cpp)

target_link_libraries(test_particles PRIVATE
    Catch2::Catch2WithMain
)

target_include_directories(test_particles PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

catch_discover_tests(test_particles)
