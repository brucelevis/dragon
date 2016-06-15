# Project name
PROJECT(gate)

# Header directories
INCLUDE_DIRECTORIES(
	../core
	${CMAKE_BINARY_DIR}/src
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/src/protocol
	)

# All source files
FILE(GLOB_RECURSE SRC_LIST . "*.cpp" "*.c" "*.h" "*.hpp")  
AUX_SOURCE_DIRECTORY(. SRC_LIST) 

# Generate binary
ADD_EXECUTABLE(${PROJECT_NAME} ${SRC_LIST})
# Link libraries
TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
	core 
	proto
	)

# Install binary
INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${BINARY_OUTPUTPATH})
