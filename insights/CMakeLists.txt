function(glCreateSampleGTC NAME)
	set(SAMPLE_NAME ${GL_PROFILE_GTC}-${GL_VERSION_GTC}-${NAME})
	set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../data/${GL_PROFILE_GTC}-${GL_VERSION_GTC})

	foreach(FILE ${GL_SHADER_GTC})
 		set(SHADER_PATH ${SHADER_PATH} ${SHADER_DIR}/${FILE})
	endforeach(FILE)

	source_group("Shader Files" FILES ${SHADER_PATH})
	add_executable(${SAMPLE_NAME} ${GL_PROFILE_GTC}-${GL_VERSION_GTC}-${NAME}.cpp ${SHADER_PATH})
	add_custom_command(
		TARGET ${NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data)
	add_custom_command(
		TARGET ${NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${ASSIMP_BINARY} ${CMAKE_CURRENT_BINARY_DIR})

	add_test(NAME ${SAMPLE_NAME} COMMAND $<TARGET_FILE:${SAMPLE_NAME}>)
	add_dependencies(${SAMPLE_NAME} ${TARGET_DATA_COPY})
if(MSVC)
	target_link_libraries(${SAMPLE_NAME} ${BINARY_FILES})
else(MSVC)
	target_link_libraries(${SAMPLE_NAME} ${BINARY_FILES} GLEW glut)
endif(MSVC)
endfunction(glCreateSampleGTC)

set(GL_PROFILE_GTC gl)
set(GL_VERSION_GTC XXX)

set(GL_SHADER_GTC matching-by-block.vert matching-by-block.geom matching-by-block.cont matching-by-block.eval matching-by-block.frag)
glCreateSampleGTC(5.3-matching-by-block)

set(GL_SHADER_GTC matching-by-location.vert matching-by-location.geom matching-by-location.cont matching-by-location.eval matching-by-location.frag)
glCreateSampleGTC(5.6-matching-by-location)

set(GL_SHADER_GTC matching-by-variable.vert matching-by-variable.geom matching-by-variable.cont matching-by-variable.eval matching-by-variable.frag)
glCreateSampleGTC(5.2-matching-by-variable)

