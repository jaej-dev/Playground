# Title     :   deployment.cmake 
# Author    :   Jaewoong Jang
# Date      :   Jan 27th 2021

if (NOT deployment_included)
  set (deployment_included TRUE)
  set (REMOTE_PATH "/root/tesla/")

  file(WRITE "${CMAKE_SOURCE_DIR}/deployment.txt" "${REMOTE_PATH}\n")

  function(add_deployment_file _src _dest)
    get_filename_component(src_path ${_src} PATH)
    file(RELATIVE_PATH path ${CMAKE_SOURCE_DIR} ${src_path})
    get_filename_component(filename ${_src} NAME)
    file(APPEND "${CMAKE_SOURCE_DIR}/deployment.txt" "${path}/${filename}:${_dest}\n")
  endfunction()

  MACRO(ARGC var dummy)
    SET(${var} ${ARGV1})
  ENDMACRO(ARGC)

  MACRO(ARGV var dummy)
    SET(${var} ${ARGN})
  ENDMACRO(ARGV)

  function(add_deployment_file_list _src _dest)
    list(LENGTH _src _len)
    #message("_src: ${_src}, _dest: ${_dest}, _len: ${_len}")
    if (${_len} GREATER 0)
      ARGC(argc ${_src})
      ARGV(argv ${_src})
      add_deployment_file(${argc} ${_dest})
      add_deployment_file_list("${argv}" ${_dest})
    endif ()
  endfunction()

  function(add_deployment_directory _src _dest)
    file(GLOB_RECURSE files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${_src}/*")
    foreach(filename ${files})
      get_filename_component(path ${filename} PATH)
      add_deployment_file("${filename}" "${_dest}/${path}")
    endforeach(filename)
  endfunction()
endif (NOT deployment_included)
