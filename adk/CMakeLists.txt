set(LIB_NAME adk)
aux_source_directory(. SRC_DIR)
add_library(${LIB_NAME} SHARED ${SRC_DIR})
install(TARGETS ${LIB_NAME} LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/lib)
