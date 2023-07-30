cmake_minimum_required(VERSION 3.5)

# Set the name of the dynamic library as a variable
set(LibraryName TCPFunction)

project(${LibraryName} VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Add the source files for your library here
set(SOURCES
    src/FunctionServer.cpp
)

# Add header files directory if you have header files for your library
set(HEADERS
    include/FunctionServer.h
)

# Create the dynamic library from the source files
add_library(${LibraryName} SHARED ${SOURCES})

# Set include directories for header files (if any)
target_include_directories(${LibraryName} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Install the library
install(TARGETS ${LibraryName}
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin # For Windows shared libraries
)

# Install the header files
install(FILES ${HEADERS} DESTINATION include/${LibraryName})

# Add any other configurations or dependencies as needed

# Optionally, you can add executables and link them against the library
# For example, if you have an executable using your library:
# add_executable(MyExecutable src/main.cpp)
# target_link_libraries(MyExecutable PRIVATE ${LibraryName})