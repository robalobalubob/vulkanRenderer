# Renders one RD scene and compares it with its saved image. Run through ctest.
#
# Fails if rd_viewer exits with an error, logs a validation message or an
# error, or produces an image that differs from the golden. Validation layers
# only run in Debug builds, so run the tests in Debug to get that check.
#
# To accept the current output as correct, set VKENG_UPDATE_GOLDENS=1 in the
# environment before running ctest, then review the changed images.
#
# Required: RD_VIEWER, COMPARE_PPM, SCENE, FRAME, GOLDEN, OUTPUT

get_filename_component(output_dir "${OUTPUT}" DIRECTORY)
file(MAKE_DIRECTORY "${output_dir}")
file(REMOVE "${OUTPUT}")

execute_process(
    COMMAND "${RD_VIEWER}" "${SCENE}" --capture "${OUTPUT}" --frame "${FRAME}"
    WORKING_DIRECTORY "${output_dir}"
    RESULT_VARIABLE capture_result
    OUTPUT_VARIABLE capture_log
    ERROR_VARIABLE capture_log)

if(NOT capture_result EQUAL 0)
    message(FATAL_ERROR "rd_viewer failed (${capture_result}):\n${capture_log}")
endif()

# The debug callback logs every layer message with a "Validation:" prefix.
if(capture_log MATCHES "Validation:|\\[ERROR|\\[CRIT")
    message(FATAL_ERROR "rd_viewer logged validation messages or errors:\n${capture_log}")
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
