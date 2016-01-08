include_directories("${PROJECT_SOURCE_DIR}")

add_executable(thread_create thread_create.c)
target_link_libraries(thread_create adk)
install(TARGETS thread_create RUNTIME DESTINATION ${INSTALL_BIN_DIR})

add_executable(thread_cond thread_cond.c)
target_link_libraries(thread_cond adk)
install(TARGETS thread_cond RUNTIME DESTINATION ${INSTALL_BIN_DIR})

add_executable(thread_semaphore thread_semaphore.c)
target_link_libraries(thread_semaphore adk)
install(TARGETS thread_semaphore RUNTIME DESTINATION ${INSTALL_BIN_DIR})
