# 编译架构判断

cmake示例：
```txt
[arch] Host processor architecture: loongarch64
[arch] Host processor architecture: x86_64
[arch] Host processor architecture: mips64
[arch] Host processor architecture: aarch64
```
```cmake
if(UNIX) 
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE CACHE STRING "Release")
        message(STATUS "TEST")
    endif()
    message(STATUS "[arch] Host processor architecture: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
    if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "mips64")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mxgot")
    elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "loongarch64")
        message(STATUS " host processor arch loongarch64")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-implicit-fallthrough")
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wl,--disable-new-dtags -Wl,-rpath,'$ORIGIN':'$ORIGIN'/../lib")
    # loongarch sqlite3 -Wcast-function-type
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-cast-function-type -Wno-unused-result")
    set(CMAKE_SKIP_BUILD_RPATH TRUE)
elseif(MSVC)
    # cmake --build . --config Release
endif()
```

其他：
```cmake
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^mips" OR
    CMAKE_SYSTEM_PROCESSOR MATCHES "(^aarch64)|(^arm64)|(^ARM64)" OR
    CMAKE_SYSTEM_PROCESSOR MATCHES "^arm" OR
    CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(x86_64)|(AMD64|amd64)|(^i.86$)" OR
    CMAKE_SYSTEM_PROCESSOR MATCHES "^(powerpc|ppc)" OR
    CMAKE_SYSTEM_PROCESSOR MATCHES "^(s390x)" OR
    CMAKE_SYSTEM_PROCESSOR MATCHES "^riscv" OR
    CMAKE_SYSTEM_PROCESSOR MATCHES "^loongarch")
        set(SUPPORTED_CPU_FEATURES_ARCH TRUE)
endif()
```
qmake示例：
```txt
Project MESSAGE: [arch] QMAKE_HOST.arch value is: x86_64
Project MESSAGE: [arch] QMAKE_HOST.arch value is: mips64
Project MESSAGE: [arch] QMAKE_HOST.arch value is: aarch64
Project MESSAGE: [arch] QMAKE_HOST.arch value is: loongarch64
```

```qmake
unix {
    message("[arch] QMAKE_HOST.arch value is: $$QMAKE_HOST.arch")
    contains(QMAKE_HOST.arch, loongarch64) {
        message("Host architecture is loongarch64")
        QMAKE_CXXFLAGS += "-Wno-deprecated-declarations -Wno-deprecated-declarations"
    }

    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\':\'\$$ORIGIN\'/../lib"
}
```