include_directories(..)

set(BIN_NAME thread)
aux_source_directory(. SRC_DIR)
add_executable(${BIN_NAME} ${SRC_DIR})
link_directories(${PROJECT_BINARY_DIR}/adk)
# set_target_properties(${BIN_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath,${PROJECT_BINARY_DIR}/lib")
target_link_libraries(${BIN_NAME} adk)
install(TARGETS ${BIN_NAME} RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)


