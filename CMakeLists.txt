# get IDF version for comparison
set(idf_version "${IDF_VERSION_MAJOR}.${IDF_VERSION_MINOR}")

idf_build_get_property(target IDF_TARGET)

if(${IDF_TARGET} STREQUAL "esp32")
    set(links   "-L ${CMAKE_CURRENT_SOURCE_DIR}/esp-code-scanner/lib/esp32" "-L ${CMAKE_CURRENT_SOURCE_DIR}/esp-code-scanner/lib")

elseif(${IDF_TARGET} STREQUAL "esp32s2")
    set(links   "-L ${CMAKE_CURRENT_SOURCE_DIR}/esp-code-scanner/lib/esp32s2" "-L ${CMAKE_CURRENT_SOURCE_DIR}/esp-code-scanner/lib")

elseif(${IDF_TARGET} STREQUAL "esp32s3")
    set(links   "-L ${CMAKE_CURRENT_SOURCE_DIR}/esp-code-scanner/lib/esp32s3" "-L ${CMAKE_CURRENT_SOURCE_DIR}/esp-code-scanner/lib")

endif()

# set sources
set(srcs
  src/quirc_qr_code_decoder.cpp
  src/espressif_qr_code_decoder.cpp
  quirc/quirc.c
  quirc/decode.c
  quirc/identify.c
  quirc/version_db.c
  quirc/collections.c
)

set(include_dirs
  inc
  quirc
  esp-code-scanner/include
)

idf_component_register(
  SRCS ${srcs}
  INCLUDE_DIRS ${include_dirs}
)

target_link_libraries(${COMPONENT_TARGET}   INTERFACE   ${links})

set(lib     libesp-code-scanner.a
            libnewlib_iconv.a)
target_link_libraries(${COMPONENT_TARGET}   INTERFACE   ${lib})
