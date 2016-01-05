aux_source_directory(. SRC_DIR)
add_library(adk SHARED ${SRC_DIR})
install(TARGETS adk LIBRARY DESTINATION ${INSTALL_LIB_DIR})
