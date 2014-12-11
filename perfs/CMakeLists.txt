function(glCreateMicroGTC NAME)
	set(SAMPLE_NAME ${GL_PROFILE_GTC}-${GL_VERSION_GTC}-${NAME})

	set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../data/perfs)

	foreach(FILE ${GL_SHADER_GTC})
		set(SHADER_PATH ${SHADER_PATH} ${SHADER_DIR}/${FILE})
	endforeach(FILE)

	source_group("Shader Files" FILES ${SHADER_PATH})

	add_executable(${SAMPLE_NAME} ${NAME}.cpp ${SHADER_PATH})

	target_link_libraries(${SAMPLE_NAME} ${FRAMEWORK_NAME} ${BINARY_FILES})
	add_dependencies(${SAMPLE_NAME} ${FRAMEWORK_NAME} glfw)
endfunction(glCreateMicroGTC)

# OpenGL 3.2 samples
set(GL_PROFILE_GTC bk)
set(GL_VERSION_GTC 320)

set(GL_SHADER_GTC test-buffer.vert test-buffer.frag)
glCreateMicroGTC(test-buffer)
set(GL_SHADER_GTC test-buffer-multi-attrib.vert test-buffer-multi-attrib.frag)
glCreateMicroGTC(test-buffer-multi-attrib)
set(GL_SHADER_GTC test-draw.vert test-draw.frag)
glCreateMicroGTC(test-draw)


