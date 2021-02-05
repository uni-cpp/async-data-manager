project( async-data-manager )

set( HEADERS
    ""
)

set( SOURCES
    "app/main.cpp"
    "src/uni/application/Application.cpp"
    "src/uni/application/Application.hpp"
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

add_executable( ${PROJECT_NAME}
    ${HEADERS}
    ${SOURCES}
)

target_include_directories( ${PROJECT_NAME}
    PUBLIC
        ${SOURCE_DIR}/common/public
    PRIVATE
        ${SOURCE_DIR}/async-data-manager/src
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE
        ${CMAKE_THREAD_LIBS_INIT}
        uni-common
)

install( TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
