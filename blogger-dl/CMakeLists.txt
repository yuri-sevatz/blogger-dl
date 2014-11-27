cmake_minimum_required(VERSION 2.8.8)

# project details
# ---------------
project(blogger-dl)

set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# required dependencies
find_package(Qt5 COMPONENTS Core Network WebKit WebKitWidgets Widgets REQUIRED)

# add your headers and source files here
# ---------------

set (${PROJECT_NAME}_headers
    bootstrap.hpp
    app.hpp
)
set (${PROJECT_NAME}_sources
    bootstrap.cpp
    app.cpp
    main.cpp
)

QT5_WRAP_CPP(${PROJECT_NAME}_headers_moc ${${PROJECT_NAME}_headers})

# Create the binary
# ---------------
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_headers_moc} ${${PROJECT_NAME}_sources})

target_link_libraries(${PROJECT_NAME} twirl blogger)

qt5_use_modules(${PROJECT_NAME} Core Network WebKit WebKitWidgets Widgets)

# Install
# ---------------
install(TARGETS ${PROJECT_NAME}
                RUNTIME DESTINATION bin
                LIBRARY DESTINATION lib
                ARCHIVE DESTINATION lib/static)

