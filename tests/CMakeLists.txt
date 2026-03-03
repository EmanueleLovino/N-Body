add_executable(test_particles test_particles.cpp)
add_executable(test_system test_system.cpp)
add_executable(test_utils test_utils.cpp)

target_link_libraries(test_particles PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_system PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_utils PRIVATE Catch2::Catch2WithMain)

target_include_directories(test_particles PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(test_system PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(test_utils PRIVATE ${PROJECT_SOURCE_DIR}/include)

catch_discover_tests(test_particles)
catch_discover_tests(test_system)
catch_discover_tests(test_utils)
