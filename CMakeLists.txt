project( async-data-manager )

set( HEADERS_static
    "include/uni/application/Application.hpp"
)

set( SOURCES_static
    "src/uni/application/Application.cpp"
    "src/uni/application/Config.hpp"
    "src/uni/application/Config.cpp"
    "src/uni/application/DataGenerator.cpp"
    "src/uni/application/DataGenerator.hpp"
    "src/uni/application/DataGeneratorListener.hpp"
    "src/uni/application/Hasher.cpp"
    "src/uni/application/Hasher.hpp"
    "src/uni/application/HasherListener.hpp"
    "src/uni/application/DispatchQueue.cpp"
    "src/uni/application/DispatchQueue.hpp"
)

# treat_all_warnings_as_errors()

add_library( ${PROJECT_NAME}_static STATIC
    ${HEADERS_static}
    ${SOURCES_static}
)

target_include_directories( ${PROJECT_NAME}_static
    PUBLIC
        ${SOURCE_DIR}/async-data-manager/include
    PRIVATE
        ${SOURCE_DIR}/common/public
        ${SOURCE_DIR}/async-data-manager/src
        ${SOURCE_DIR}/external/json/include
)

target_link_libraries( ${PROJECT_NAME}_static
    PRIVATE
        ${CMAKE_THREAD_LIBS_INIT}
        uni-common
        json
)

install( TARGETS ${PROJECT_NAME}_static
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# The trailing slash is important
install( DIRECTORY ${CMAKE_INSTALL_INCLUDEDIR}/
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

######################################################

set( HEADERS
    ""
)

set( SOURCES
    "app/main.cpp"
)

# treat_all_warnings_as_errors()

add_executable( ${PROJECT_NAME}
    ${HEADERS}
    ${SOURCES}
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE
        ${SOURCE_DIR}/async-data-manager/include
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE
        ${PROJECT_NAME}_static
)

# TODO Move it to /etc and resolve dependency from app
configure_file( ${SOURCE_DIR}/async-data-manager/config/Config.json Config.json COPYONLY
)

install( TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install( FILES ${SOURCE_DIR}/async-data-manager/config/Config.json
         DESTINATION ${CMAKE_INSTALL_BINDIR}
)
