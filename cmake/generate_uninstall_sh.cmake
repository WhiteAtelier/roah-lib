
set(UNINSTALL_SH_PATH "${CMAKE_BINARY_DIR}/uninstall.sh")
file(WRITE ${UNINSTALL_SH_PATH} "#!/bin/sh\n")

foreach(FILE_PATH IN LISTS CMAKE_INSTALL_MANIFEST_FILES)
    file(APPEND ${UNINSTALL_SH_PATH} "rm -f \"${FILE_PATH}\"\n")
endforeach()

file(APPEND ${UNINSTALL_SH_PATH} "echo \"Uninstallation complete.\"\n")
file(APPEND ${UNINSTALL_SH_PATH} "exit 0\n")

file(
    CHMOD
        ${UNINSTALL_SH_PATH}
    PERMISSIONS
        OWNER_EXECUTE OWNER_WRITE OWNER_READ
        GROUP_EXECUTE GROUP_READ
        WORLD_EXECUTE WORLD_READ
)
