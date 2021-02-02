# Generate personality.c if it does not exist.
set(PERSONALITY_C_IN ${CMAKE_CURRENT_SOURCE_DIR}/personality.c.in)
set(PERSONALITY_C_OUT ${CMAKE_CURRENT_SOURCE_DIR}/personality.c)
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/personality.c)
  message("Remember to customize ${PERSONALITY_C_OUT}")
endif()
configure_file(${PERSONALITY_C_IN} ${PERSONALITY_C_OUT} @ONLY)

target_sources(app
  PRIVATE
  main.c
  personality.c
  )
