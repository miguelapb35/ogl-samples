function(glCreatePerfGTC NAME)
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
	if(WIN32)
		add_dependencies(${SAMPLE_NAME} ${TARGET_DATA_COPY})
	endif(WIN32)
	target_link_libraries(${SAMPLE_NAME} ${BINARY_FILES})
	add_dependencies(${SAMPLE_NAME} GLEW glfw)
	install ( TARGETS ${SAMPLE_NAME} DESTINATION .)
endfunction(glCreatePerfGTC)

set(GL_PROFILE_GTC hz)
set(GL_VERSION_GTC 430)

if(NOT APPLE)

set(GL_SHADER_GTC multi-draw-indirect.vert multi-draw-indirect.frag)
glCreatePerfGTC(thread)
set(GL_SHADER_GTC multi-draw-indirect.vert multi-draw-indirect.frag)
glCreatePerfGTC(multi-draw-indirect-array)
set(GL_SHADER_GTC multi-draw-indirect.vert multi-draw-indirect.frag)
glCreatePerfGTC(multi-draw-indirect-element)
set(GL_SHADER_GTC vertex-array-object.vert vertex-array-object.frag)
glCreatePerfGTC(vertex-array-object)
set(GL_SHADER_GTC vertex-array-object.vert vertex-array-object.frag)
glCreatePerfGTC(vertex-array-objects)
set(GL_SHADER_GTC vertex-array-object.vert vertex-array-object.frag)
glCreatePerfGTC(base-vertex)

endif(NOT APPLE)
