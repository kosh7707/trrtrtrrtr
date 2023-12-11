# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. The list of available CPACK_xxx variables and their associated
# documentation may be obtained using
#  cpack --help-variable-list
#
# Some variables are common to all generators (e.g. CPACK_PACKAGE_NAME)
# and some are specific to a generator
# (e.g. CPACK_NSIS_EXTRA_INSTALL_COMMANDS). The generator specific variables
# usually begin with CPACK_<GENNAME>_xxxx.


set(CPACK_BUILD_SOURCE_DIRS "C:/Users/Administrator/Desktop/trrtrtrrtr/DatabaseTermProjectClient;C:/Users/Administrator/Desktop/trrtrtrrtr/DatabaseTermProjectClient/cmake-build-release")
set(CPACK_CMAKE_GENERATOR "Ninja")
set(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
set(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_FILE "C:/Program Files/JetBrains/CLion 2022.2.1/bin/cmake/win/x64/share/cmake-3.26/Templates/CPack.GenericDescription.txt")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_SUMMARY "DatabaseTermProjectClient built using CMake")
set(CPACK_GENERATOR "TGZ")
set(CPACK_INSTALL_CMAKE_PROJECTS "C:/Users/Administrator/Desktop/trrtrtrrtr/DatabaseTermProjectClient/cmake-build-release;DatabaseTermProjectClient;ALL;/")
set(CPACK_INSTALL_PREFIX "C:/Program Files (x86)/DatabaseTermProjectClient")
set(CPACK_MODULE_PATH "C:/Users/Administrator/Desktop/trrtrtrrtr/DatabaseTermProjectClient/libpqxx/cmake")
set(CPACK_NSIS_DISPLAY_NAME "DatabaseTermProjectClient 7.9.0")
set(CPACK_NSIS_INSTALLER_ICON_CODE "")
set(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
set(CPACK_NSIS_PACKAGE_NAME "DatabaseTermProjectClient 7.9.0")
set(CPACK_NSIS_UNINSTALL_NAME "Uninstall")
set(CPACK_OBJCOPY_EXECUTABLE "C:/Program Files/JetBrains/CLion 2022.2.1/bin/mingw/bin/objcopy.exe")
set(CPACK_OBJDUMP_EXECUTABLE "C:/Program Files/JetBrains/CLion 2022.2.1/bin/mingw/bin/objdump.exe")
set(CPACK_OUTPUT_CONFIG_FILE "C:/Users/Administrator/Desktop/trrtrtrrtr/DatabaseTermProjectClient/cmake-build-release/CPackConfig.cmake")
set(CPACK_PACKAGE_DEFAULT_LOCATION "/")
set(CPACK_PACKAGE_DESCRIPTION_FILE "C:/Program Files/JetBrains/CLion 2022.2.1/bin/cmake/win/x64/share/cmake-3.26/Templates/CPack.GenericDescription.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "DatabaseTermProjectClient built using CMake")
set(CPACK_PACKAGE_FILE_NAME "DatabaseTermProjectClient-7.9.0-win64")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "DatabaseTermProjectClient 7.9.0")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "DatabaseTermProjectClient 7.9.0")
set(CPACK_PACKAGE_NAME "DatabaseTermProjectClient")
set(CPACK_PACKAGE_RELOCATABLE "true")
set(CPACK_PACKAGE_VENDOR "Humanity")
set(CPACK_PACKAGE_VERSION "7.9.0")
set(CPACK_PACKAGE_VERSION_MAJOR "7")
set(CPACK_PACKAGE_VERSION_MINOR "9")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_READELF_EXECUTABLE "C:/Program Files/JetBrains/CLion 2022.2.1/bin/mingw/bin/readelf.exe")
set(CPACK_RESOURCE_FILE_LICENSE "C:/Program Files/JetBrains/CLion 2022.2.1/bin/cmake/win/x64/share/cmake-3.26/Templates/CPack.GenericLicense.txt")
set(CPACK_RESOURCE_FILE_README "C:/Program Files/JetBrains/CLion 2022.2.1/bin/cmake/win/x64/share/cmake-3.26/Templates/CPack.GenericDescription.txt")
set(CPACK_RESOURCE_FILE_WELCOME "C:/Program Files/JetBrains/CLion 2022.2.1/bin/cmake/win/x64/share/cmake-3.26/Templates/CPack.GenericWelcome.txt")
set(CPACK_SET_DESTDIR "OFF")
set(CPACK_SOURCE_7Z "ON")
set(CPACK_SOURCE_GENERATOR "7Z;ZIP")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "C:/Users/Administrator/Desktop/trrtrtrrtr/DatabaseTermProjectClient/cmake-build-release/CPackSourceConfig.cmake")
set(CPACK_SOURCE_ZIP "ON")
set(CPACK_SYSTEM_NAME "win64")
set(CPACK_THREADS "1")
set(CPACK_TOPLEVEL_TAG "win64")
set(CPACK_WIX_SIZEOF_VOID_P "8")

if(NOT CPACK_PROPERTIES_FILE)
  set(CPACK_PROPERTIES_FILE "C:/Users/Administrator/Desktop/trrtrtrrtr/DatabaseTermProjectClient/cmake-build-release/CPackProperties.cmake")
endif()

if(EXISTS ${CPACK_PROPERTIES_FILE})
  include(${CPACK_PROPERTIES_FILE})
endif()
