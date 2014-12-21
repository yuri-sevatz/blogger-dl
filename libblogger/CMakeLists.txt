cmake_minimum_required(VERSION 2.8.8)

# project details
# ---------------
project(blogger)

set(${PROJECT_NAME}_qt_modules
    Core
    Network
    WebKit
    WebKitWidgets
    Xml
)

find_package(Qt5 COMPONENTS ${${PROJECT_NAME}_qt_modules} REQUIRED)

OPTION(MORE_VERBOSE "Enable more verbose output messages" OFF)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/libblogger/include/config.h.cmake ${CMAKE_SOURCE_DIR}/libblogger/include/config.h)

set(${PROJECT_NAME}_pub_headers
    client.hpp
    blog.hpp
)

set(${PROJECT_NAME}_pub_sources

)

set(${PROJECT_NAME}_pri_headers
    blogdata.hpp
    clientprivate.hpp
)

set(${PROJECT_NAME}_pri_sources
    blog.cpp
    client.cpp
    clientprivate.cpp
)

set(${PROJECT_NAME}_headers
    ${${PROJECT_NAME}_pub_headers}
    ${${PROJECT_NAME}_pri_headers}
)

set(${PROJECT_NAME}_sources
    ${${PROJECT_NAME}_pub_sources}
    ${${PROJECT_NAME}_pri_sources}
)

QT5_WRAP_CPP(${PROJECT_NAME}_headers_moc ${${PROJECT_NAME}_headers})

# Create the library
# ---------------
add_library(${PROJECT_NAME} SHARED
    ${${PROJECT_NAME}_headers}
    ${${PROJECT_NAME}_headers_moc}
    ${${PROJECT_NAME}_sources}
)

target_link_libraries(${PROJECT_NAME}
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

foreach(header ${${PROJECT_NAME}_pub_headers})
	string(REGEX MATCH "(.*)[/\\]" dir ${header})
	install(FILES ${header} DESTINATION include/lib${PROJECT_NAME}/${dir})
endforeach(header ${${PROJECT_NAME}_pub_headers})
