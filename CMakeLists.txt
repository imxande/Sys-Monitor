cmake_minimum_required(VERSION 3.28)

project(sys_monitor VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find widgets module
find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

# sources
set(SOURCES 
  src/main.cpp
  src/mainWindow.cpp
  src/processTab.cpp
  src/processManager.cpp
  src/fileSystemsTab.cpp
)
set(HEADERS 
  includes/mainWindow.h
  includes/processTab.h
  includes/processManager.h
  includes/processInfo.h
  includes/fileSystemsTab.h
)

# executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# includes
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/includes)

# link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

