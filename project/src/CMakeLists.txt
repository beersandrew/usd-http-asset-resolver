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

include_directories("/Users/andrewbeers/.pyenv/versions/3.9.8/include/python3.9")
set(Python3_ROOT_DIR "/Users/andrewbeers/.pyenv/versions/3.9.8")
find_package(Python3 3.9 EXACT REQUIRED)

target_link_libraries(${PLUGIN_NAME}
    PUBLIC

		libboost_python39.dylib
		libusd_ar.dylib
		libusd_tf.dylib
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
