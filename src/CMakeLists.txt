add_executable( HTTP-Server
		main.cpp
		server.cpp
		defaults.cpp
)

target_include_directories(HTTP-Server PRIVATE ${CMAKE_SOURCE_DIR}/src/inc)

target_compile_features(HTTP-Server PUBLIC cxx_std_23)
