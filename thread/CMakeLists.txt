include_directories("${PROJECT_SOURCE_DIR}")

add_executable(thread thread_test.c)
target_link_libraries(thread adk)
install(TARGETS thread RUNTIME DESTINATION ${INSTALL_BIN_DIR})
