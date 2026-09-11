# CMakeLists.txt for tests

find_package(GTest REQUIRED)

add_executable(Tests 
    "Math/Vector2Tests.cpp"
 "Math/Vector3Tests.cpp" "Math/Vector4Tests.cpp" "Math/MathTests.cpp" "Math/QuaternionTests.cpp" "Math/Matrix4x4FTests.cpp" "Math/RotationTests.cpp" "Math/ColorTests.cpp"
)

target_link_libraries(Tests PRIVATE GTest::gtest GTest::gtest_main CoreRuntime)

include(GoogleTest)
gtest_discover_tests(Tests)