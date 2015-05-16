# The name of the target operating system.
set(CMAKE_SYSTEM_NAME Generic)

set(IOS_BASE_SDK "4.3")
set(IOS_DEPLOY_TGT "3.2")

set(DEVROOT "/Developer/Platforms/iPhoneOS.platform/Developer")
set(SDKROOT "${DEVROOT}/SDKs/iPhoneOS${IOS_BASE_SDK}.sdk")
set(IOS_ADDITIONAL_LIBRARY_PATH "$(pwd)/../../nativelibs/armv7")
set(IOS_PLATFORM_INCLUDE "${SDKROOT}/usr/include")
set(IOS_PLATFORM_LIB "${SDKROOT}/usr/lib")
	
# Location of target environment.

set(CMAKE_SYSTEM_INCLUDE_PATH "${IOS_ADDITIONAL_LIBRARY_PATH}/include" "${IOS_PLATFORM_INCLUDE}")
set(CMAKE_SYSTEM_LIBRARY_PATH "${IOS_ADDITIONAL_LIBRARY_PATH}/lib" "${IOS_PLATFORM_LIB}")

# Which compilers to use for C and C++.
set(CMAKE_C_COMPILER "${DEVROOT}/usr/bin/gcc-4.2")
set(CMAKE_CXX_COMPILER "${DEVROOT}/usr/bin/g++-4.2")

# Needed to pass the compiler tests.
set(LINK_DIRECTORIES ${NDK_PLATFORM_LIB})
set(LINK_LIBRARIES "")

set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

# Adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search
# programs in the host environment.
set(CMAKE_FIND_ROOT_PATH "")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(IOS 1)
