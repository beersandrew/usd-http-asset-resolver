# 4. Include/Link everything
set(PLUGIN_NAME URIResolver)

link_directories(${USD_LIBRARY_DIR})


set(
	SRC
		resolver.cpp
)

add_library(${PLUGIN_NAME}
    SHARED
		${SRC}
)

set(PYTHON_ROOT "/Users/andrewbeers/.pyenv/versions/3.9.8")
 # Set the path to the Python executable you want to use
set(PYTHON_EXECUTABLE "${PYTHON_ROOT}/bin/python3.9")
set(PYTHON_INCLUDE_DIR "${PYTHON_ROOT}/include/python3.9")
set(PYTHON_LIBRARY "${PYTHON_ROOT}/lib/libpython3.9.dylib")

# Include the Python include directory
include_directories(${PYTHON_INCLUDE_DIR})

target_link_libraries(${PLUGIN_NAME}
    PUBLIC

		libboost_python39.dylib
		libusd_ar.dylib
		libusd_tf.dylib
		${PYTHON_LIBRARY}
)

target_include_directories(
	${PLUGIN_NAME}
		SYSTEM
			PRIVATE
				${USD_INCLUDE_DIR}

			PUBLIC
				${PYTHON_INCLUDE_PATH}
)

install(
    TARGETS
        ${PLUGIN_NAME}
	DESTINATION
		.
)

install(
    FILES
		plugInfo.json
    DESTINATION
        ./resources
)

# Define the source and destination paths
set(PLUGINFO_SOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/plugInfo.json")
set(PLUGINFO_DESTINATION_DIR "${CMAKE_BINARY_DIR}/build/install/resources")

# Ensure the destination directory exists
file(MAKE_DIRECTORY ${PLUGINFO_DESTINATION_DIR})

# Add a custom command to copy plugInfo.json to the desired directory
add_custom_command(
		OUTPUT ${PLUGINFO_DESTINATION_DIR}/plugInfo.json
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PLUGINFO_SOURCE_FILE} ${PLUGINFO_DESTINATION_DIR}/plugInfo.json
		DEPENDS ${PLUGINFO_SOURCE_FILE}
		COMMENT "Copying plugInfo.json to ${PLUGINFO_DESTINATION_DIR}"
)

# Create a custom target that depends on the copied file
add_custom_target(copy_plugInfo ALL
		DEPENDS ${PLUGINFO_DESTINATION_DIR}/plugInfo.json
)

# Ensure your main target depends on this custom target
# Replace 'main_target' with the actual main target of your project
add_dependencies(${PLUGIN_NAME} copy_plugInfo)

set_target_properties(${PLUGIN_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/install"
	OUTPUT_NAME ${PLUGIN_NAME}
	SUFFIX ".so"
)