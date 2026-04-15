# embed_files.cmake
# Reads all files under DIST_DIR recursively and writes .inl files to OUTPUT_DIR.
# Each .inl file contains the file's bytes as comma-separated hex literals.
#
# Usage:
#   cmake -DDIST_DIR=<path> -DOUTPUT_DIR=<path> -P embed_files.cmake

if(NOT DIST_DIR OR NOT OUTPUT_DIR OR NOT TEMPLATE_FILE OR NOT OUTPUT_CPP)
    message(FATAL_ERROR "embed_files.cmake requires -DDIST_DIR, -DOUTPUT_DIR, -DTEMPLATE_FILE and -DOUTPUT_CPP")
endif()

file(GLOB_RECURSE dist_files RELATIVE "${DIST_DIR}" "${DIST_DIR}/*")
file(MAKE_DIRECTORY "${OUTPUT_DIR}")

set(CONTENTS "")

foreach(rel_file IN LISTS dist_files)
    set(src_file "${DIST_DIR}/${rel_file}")

    if(IS_DIRECTORY "${src_file}")
        continue()
    endif()

    file(READ "${src_file}" hex_content HEX)

    string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," hex_bytes "${hex_content}")

    # Preserve directory structure; make only the filename safe (replace dots with underscores)
    get_filename_component(rel_dir  "${rel_file}" DIRECTORY)
    get_filename_component(rel_name "${rel_file}" NAME)
    string(REPLACE "." "_" safe_name "${rel_name}")

    if(rel_dir)
        file(MAKE_DIRECTORY "${OUTPUT_DIR}/${rel_dir}")
        set(inl_rel_path "${rel_dir}/${safe_name}.inl")
    else()
        set(inl_rel_path "${safe_name}.inl")
    endif()

    set(inl_file "${OUTPUT_DIR}/${inl_rel_path}")
    file(WRITE "${inl_file}" "${hex_bytes}\n")
    message(STATUS "Embedded: ${rel_file} -> ${inl_file}")

    # Normalize key to forward slashes
    string(REPLACE "\\" "/" key "${rel_file}")
    # Normalize inl_rel_path to forward slashes for #include
    string(REPLACE "\\" "/" inl_rel_include "${inl_rel_path}")

    string(APPEND CONTENTS
        "if(relpath==\"${key}\"){static constexpr auto bin=std::to_array<unsigned char>({
#include \"${inl_rel_include}\"
});return{bin.data(),bin.size()};}
"
    )
endforeach()

configure_file("${TEMPLATE_FILE}" "${OUTPUT_CPP}" @ONLY)
