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

set(MICRO micro)
set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../data/${GL_PROFILE_GTC}-${GL_VERSION_GTC})

set(GL_SHADER_GTC
	draw.vert draw.frag
	draw-uniform.vert draw-uniform.frag
	draw-uniform2.vert draw-uniform2.frag
	draw-range.vert draw-range.frag
	draw-indexing-id.vert draw-indexing-id.frag
	draw-indexing-attrib.vert draw-indexing-attrib.frag
	draw-indexing-uniform.vert draw-indexing-uniform.frag
	screenspace_coherence.vert screenspace_coherence.frag)

foreach(FILE ${GL_SHADER_GTC})
	set(SHADER_PATH ${SHADER_PATH} ${SHADER_DIR}/${FILE})
endforeach(FILE)

source_group("Shader Files" FILES ${SHADER_PATH})
add_executable(${MICRO}
	main.cpp
	png.cpp
	png.hpp
	csv.cpp
	csv.hpp
	caps.cpp
	caps.hpp
	test.cpp
	test.hpp
	test_compiler.cpp
	test_compiler.hpp
	test_draw_indexing.cpp
	test_draw_indexing.hpp
	test_draw_arrays.cpp
	test_draw_arrays.hpp
	test_draw_elements.cpp
	test_draw_elements.hpp
	test_draw_arrays_vao.cpp
	test_draw_arrays_vao.hpp
	test_screenspace_coherence.cpp
	test_screenspace_coherence.hpp
	buffer.hpp buffer.cpp
	${SHADER_PATH})
add_custom_command(
	TARGET ${MICRO} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data)

if(WIN32)
	add_dependencies(${MICRO} ${TARGET_DATA_COPY})
endif(WIN32)
target_link_libraries(${MICRO} ${BINARY_FILES})
add_dependencies(${MICRO} GLEW glfw)

endif(NOT APPLE)
