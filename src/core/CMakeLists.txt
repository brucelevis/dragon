# Project name
PROJECT(core)

# Header directories
INCLUDE_DIRECTORIES(
	)
# Library directories
LINK_DIRECTORIES(
	)

# Other source files
AUX_SOURCE_DIRECTORY(. SRC_LIST)

# Generate library
ADD_LIBRARY(${PROJECT_NAME} STATIC ${SRC_LIST})
# Link libraries
TARGET_LINK_LIBRARIES(${PROJECT_NAME})

# Output library
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES PREFIX ${LIB_PREFIX}) 