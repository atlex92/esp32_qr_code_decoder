# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)
set(EXTRA_COMPONENT_DIRS ../../../esp32_qr_code_reader)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(qr_code_decoder_example)
idf_build_set_property(COMPILE_OPTIONS "-DBOARD_HAS_PSRAM" APPEND)
