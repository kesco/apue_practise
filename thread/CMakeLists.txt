include_directories("${PROJECT_SOURCE_DIR}")

file(GLOB SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c)

foreach(src ${SRC_FILES})
  get_filename_component(file_name ${src} NAME_WE)
  add_executable(${file_name} ${src})
  target_link_libraries(${file_name} adk)
  install(TARGETS ${file_name} RUNTIME DESTINATION ${INSTALL_BIN_DIR})
endforeach(src)
