cmake_minimum_required(VERSION 3.28)

project(sys_monitor VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find widgets module
find_package(Qt6 REQUIRED COMPONENTS
  Widgets
  QuickWidgets
  Quick
  Qml
  QuickControls2
  Charts
  Core
)

qt_standard_project_setup()

# sources
set(SOURCES 
  src/main.cpp
  src/mainWindow.cpp
  src/processesTab.cpp
  src/processManager.cpp
  src/fileSystemsTab.cpp
  src/resourceMonitor.cpp
  src/resourcesTab.cpp
)

set(HEADERS 
  includes/mainWindow.h
  includes/processesTab.h
  includes/processManager.h
  includes/processInfo.h
  includes/fileSystemsTab.h
  includes/fileSystemInfo.h
  includes/resourceMonitor.h
  includes/resourcesTab.h
)

qt_add_resources(QML_RESOURCES resources/qml/qml.qrc)

# executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${QML_RESOURCES})

# includes
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/includes)

# link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
  Qt6::Widgets
  Qt6::QuickWidgets
  Qt6::Quick
  Qt6::Qml
  Qt6::QuickControls2
  Qt6::Charts
  Qt6::Core
)

