cmake_minimum_required(VERSION 2.8.8)

# project details
# ---------------
project(blogger-dl)

set(${PROJECT_NAME}_qt_modules
    Core
    Network
    WebKit
    WebKitWidgets
    Widgets
)

find_package(Qt5 COMPONENTS ${${PROJECT_NAME}_qt_modules} REQUIRED)

set (${PROJECT_NAME}_headers
    app.hpp
)
set (${PROJECT_NAME}_sources
    app.cpp
    main.cpp
)

QT5_WRAP_CPP(${PROJECT_NAME}_headers_moc ${${PROJECT_NAME}_headers})

add_executable(${PROJECT_NAME}
    ${${PROJECT_NAME}_headers}
    ${${PROJECT_NAME}_headers_moc}
    ${${PROJECT_NAME}_sources}
)

target_link_libraries(${PROJECT_NAME}
    blogger
    twirl
)

qt5_use_modules(${PROJECT_NAME} ${${PROJECT_NAME}_qt_modules})

# Install
# ---------------
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
)
