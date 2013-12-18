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

set(PERFS perfs)
set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../data/${GL_PROFILE_GTC}-${GL_VERSION_GTC})

set(GL_SHADER_GTC
	draw-uniform.vert draw-uniform.frag
	draw-range.vert draw-range.frag)

foreach(FILE ${GL_SHADER_GTC})
	set(SHADER_PATH ${SHADER_PATH} ${SHADER_DIR}/${FILE})
endforeach(FILE)

source_group("Shader Files" FILES ${SHADER_PATH})
add_executable(${PERFS}
	main.cpp
	csv.cpp
	csv.hpp
	caps.cpp
	caps.hpp
	test.cpp
	test.hpp
	test_draw_arrays.cpp
	test_draw_arrays.hpp
	test_draw_elements.cpp
	test_draw_elements.hpp
	hz-430-base-vertex.cpp 
	hz-430-draw-range-array.cpp
	hz-430-draw-range-element.cpp
	hz-430-multi-draw-indirect-array.cpp
	hz-430-multi-draw-indirect-element.cpp
	hz-430-vertex-array-object.cpp
	hz-430-vertex-array-objects.cpp
	${SHADER_PATH})
add_custom_command(
	TARGET ${PERFS} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data)

if(WIN32)
	add_dependencies(${PERFS} ${TARGET_DATA_COPY})
endif(WIN32)
target_link_libraries(${PERFS} ${BINARY_FILES})
add_dependencies(${PERFS} GLEW glfw)

endif(NOT APPLE)
