set(APP_SOURCE "main.cc")

add_executable(${MAIN} ${APP_SOURCE})

target_link_libraries(${MAIN} PUBLIC ${LIBRARY_NAME})
target_include_directories(${MAIN} PUBLIC ${PROJECT_BINARY_DIR})
target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_20)