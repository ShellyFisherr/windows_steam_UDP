cmake_minimum_required(VERSION 3.16)

project(tracker_reader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OPENVR_DIR "C:/Users/HAMGIST/Desktop/project/openvr")

# --------------------------------------------------
# Original device detection program
# --------------------------------------------------

add_executable(tracker_reader
    main.cpp
)

target_include_directories(tracker_reader PRIVATE
    "${OPENVR_DIR}/headers"
)

target_link_libraries(tracker_reader PRIVATE
    "${OPENVR_DIR}/lib/win64/openvr_api.lib"
)

add_custom_command(TARGET tracker_reader POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${OPENVR_DIR}/bin/win64/openvr_api.dll"
        "$<TARGET_FILE_DIR:tracker_reader>"
)


# --------------------------------------------------
# Continuous tracker data program
# --------------------------------------------------

add_executable(trackerdata
    trackerdata.cpp
)

target_include_directories(trackerdata PRIVATE
    "${OPENVR_DIR}/headers"
)

target_link_libraries(trackerdata PRIVATE
    "${OPENVR_DIR}/lib/win64/openvr_api.lib"
    ws2_32
)

add_custom_command(TARGET trackerdata POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${OPENVR_DIR}/bin/win64/openvr_api.dll"
        "$<TARGET_FILE_DIR:trackerdata>"
)

# --------------------------------------------------
# Local print tracker data program
# --------------------------------------------------

add_executable(trackerdata_local 
    trackerdata_local.cpp
)

target_include_directories(trackerdata_local PRIVATE
    "${OPENVR_DIR}/headers")

target_link_libraries(trackerdata_local PRIVATE
    "${OPENVR_DIR}/lib/win64/openvr_api.lib"
)

add_custom_command(TARGET trackerdata_local POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${OPENVR_DIR}/bin/win64/openvr_api.dll"
    "$<TARGET_FILE_DIR:trackerdata_local>"
)