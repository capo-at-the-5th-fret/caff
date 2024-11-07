cmake_minimum_required(VERSION 3.25.0)

project(caff
    VERSION 0.1.0
    LANGUAGES CXX
)

add_subdirectory(include)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)

    option(CAFF_BUILD_TESTS "Enable or disable the building of tests" ON)
    option(CAFF_ENABLE_INSTALL "Enable or disable the install rule" ON)

    if (CAFF_BUILD_TESTS)
        enable_testing()
        add_subdirectory(tests)
    endif()

    if (CAFF_ENABLE_INSTALL)

        include(GNUInstallDirs)
        include(CMakePackageConfigHelpers)

        # NOTE 1: The XXX_PACKAGE_TARGETS variable is referenced inside of the
        # xxxConfig.cmake.in file, so if the name of this variable changes, it
        # must be updated inside of the xxxConfig.cmake.in file.
        #
        # NOTE 2: If value of this variable changes, the xxxConfig.cmake.in file
        # name must be renamed to match the new value.
        set(CAFF_PACKAGE_NAME ${PROJECT_NAME})
        set(CAFF_PACKAGE_TARGETS ${CAFF_PACKAGE_NAME}Targets)

        set(CAFF_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR})
        set(CAFF_INSTALL_CMAKE_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/${CAFF_PACKAGE_NAME})

        set(CAFF_CONFIG_DIR ${PROJECT_BINARY_DIR}/cmake)
        set(CAFF_CONFIG_IN cmake/${CAFF_PACKAGE_NAME}Config.cmake.in)
        set(CAFF_CONFIG ${CAFF_CONFIG_DIR}/${CAFF_PACKAGE_NAME}Config.cmake)
        set(CAFF_CONFIG_VERSION ${CAFF_CONFIG_DIR}/${CAFF_PACKAGE_NAME}ConfigVersion.cmake)

        configure_package_config_file(
            ${CAFF_CONFIG_IN}
            ${CAFF_CONFIG}
            INSTALL_DESTINATION ${CAFF_INSTALL_CMAKE_DIR}
        )

        # major.minor.patch1 is compatible with major.minor.patch2..n
        # if no VERSION is specified, the ${PROJECT_VERSION} variable will be
        # used. if the ${PROJECT_VERSION} is not set, CMake will output an
        # error
        write_basic_package_version_file(
            ${CAFF_CONFIG_VERSION}
            COMPATIBILITY SameMinorVersion
        )

        install(
            FILES
                ${CAFF_CONFIG}
                ${CAFF_CONFIG_VERSION}
            DESTINATION ${CAFF_INSTALL_CMAKE_DIR}
        )

        install(TARGETS ${CAFF_PACKAGE_NAME}
            EXPORT ${CAFF_PACKAGE_TARGETS}
            FILE_SET HEADERS
        )

        install(EXPORT ${CAFF_PACKAGE_TARGETS}
            NAMESPACE ${CAFF_PACKAGE_NAME}::
            DESTINATION ${CAFF_INSTALL_CMAKE_DIR}
        )

    endif()

endif()
