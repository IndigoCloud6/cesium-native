if(ANDROID)
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        set(DETECTED_VCPKG_TRIPLET "x64-android")
    else()
        set(DETECTED_VCPKG_TRIPLET "arm64-android")
    endif()
elseif(IOS)
    set(DETECTED_VCPKG_TRIPLET "arm64-ios")
elseif(WINDOWS_STORE)
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64")
        set(DETECTED_VCPKG_TRIPLET "x64-uwp")
    else()
        set(DETECTED_VCPKG_TRIPLET "arm64-uwp")
    endif()
elseif (WIN32)
    set(DETECTED_VCPKG_TRIPLET "x64-windows")
elseif(APPLE)
    if(CMAKE_OSX_ARCHITECTURES)
        list(LENGTH CMAKE_OSX_ARCHITECTURES CMAKE_OSX_ARCHITECTURES_COUNT)
        if(CMAKE_OSX_ARCHITECTURES_COUNT GREATER 1)
            set(DETECTED_VCPKG_TRIPLET_ERROR "vcpkg has no built-in triplet for universal macOS binaries. Please specify only one architecture in CMAKE_OSX_ARCHITECTURES, or specify a custom triplet with VCPKG_TRIPLET.")
        elseif(CMAKE_OSX_ARCHITECTURES STREQUAL "x86_64")
            set(DETECTED_VCPKG_TRIPLET "x64-osx")
        else()
            set(DETECTED_VCPKG_TRIPLET "${CMAKE_OSX_ARCHITECTURES}-osx")
        endif()
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        set(DETECTED_VCPKG_TRIPLET "arm64-osx")
    else()
        set(DETECTED_VCPKG_TRIPLET "x64-osx")
    endif()
elseif(LINUX)
    # Assuming x64 here isn't necessarily correct, but it's the only platform we officially support.
    set(DETECTED_VCPKG_TRIPLET "x64-linux")
else()
    set(DETECTED_VCPKG_TRIPLET_ERROR "Cannot guess an appropriate value for VCPKG_TRIPLET because the operating system is unknown. Please set it manually.")
endif()
