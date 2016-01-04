include_directories(..)

set(BIN_NAME thread)
aux_source_directory(. SRC_DIR)
add_executable(${BIN_NAME} ${SRC_DIR})

install (TARGETS ${BIN_NAME} RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)
