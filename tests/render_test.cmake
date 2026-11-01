# Renders one scene and compares it with its saved image. Run through ctest.
#
# Fails if the viewer exits with an error, logs a validation message or an
# error, or produces an image that differs from the golden. Validation layers
# only run in Debug builds, so run the tests in Debug to get that check.
#
# To accept the current output as correct, set VKENG_UPDATE_GOLDENS=1 in the
# environment before running ctest, then review the changed images.
#
# Required: VIEWER, COMPARE_PPM, SCENE, GOLDEN, OUTPUT
# Optional: FRAME (animation frame to capture; rd_viewer only)

get_filename_component(output_dir "${OUTPUT}" DIRECTORY)
file(MAKE_DIRECTORY "${output_dir}")
file(REMOVE "${OUTPUT}")

set(viewer_args "${SCENE}" --capture "${OUTPUT}")
if(NOT "${FRAME}" STREQUAL "")
    list(APPEND viewer_args --frame "${FRAME}")
endif()

execute_process(
    COMMAND "${VIEWER}" ${viewer_args}
    WORKING_DIRECTORY "${output_dir}"
    RESULT_VARIABLE capture_result
    OUTPUT_VARIABLE capture_log
    ERROR_VARIABLE capture_log)

if(NOT capture_result EQUAL 0)
    message(FATAL_ERROR "${VIEWER} failed (${capture_result}):\n${capture_log}")
endif()

# The debug callback logs every layer message with a "Validation:" prefix.
if(capture_log MATCHES "Validation:|\\[ERROR|\\[CRIT")
    message(FATAL_ERROR "${VIEWER} logged validation messages or errors:\n${capture_log}")
endif()

if(NOT "$ENV{VKENG_UPDATE_GOLDENS}" STREQUAL "")
    configure_file("${OUTPUT}" "${GOLDEN}" COPYONLY)
    message(STATUS "Updated ${GOLDEN}")
    return()
endif()

execute_process(
    COMMAND "${COMPARE_PPM}" "${OUTPUT}" "${GOLDEN}"
    RESULT_VARIABLE compare_result)

if(NOT compare_result EQUAL 0)
    message(FATAL_ERROR "Render does not match ${GOLDEN} (see ${OUTPUT})")
endif()
