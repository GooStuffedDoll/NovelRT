add_subdirectory(Fabulist)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(GSL)
add_subdirectory(jsoncons)
add_subdirectory(libpng)
add_subdirectory(oneTBB)
add_subdirectory(spdlog)

if(NOVELRT_BUILD_TESTS)
  add_subdirectory(gtest)
endif()
