cmake_minimum_required(VERSION 3.13)
project(sfml_test)

#Make sure that custom modules like FindENet are found
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--stack,10000000")
set(SFML_STATIC_LIBRARIES TRUE)

find_package(
        SFML 2.5.1
        COMPONENTS
            network
            audio
            graphics
            window
            system
        REQUIRED
)

find_package(ENet REQUIRED MODULE)


add_executable(sfml_test
        main.cpp
        server.cpp
        network.cpp
        )

target_include_directories(sfml_test
        PRIVATE
            ${ENet_INCLUDE_DIRS}
        )

target_link_libraries(sfml_test
        sfml-network
        sfml-audio
        sfml-graphics
        sfml-window
        sfml-system
        ${ENet_LIBRARIES}
        )
