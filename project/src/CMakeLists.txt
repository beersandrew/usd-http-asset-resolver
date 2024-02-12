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
