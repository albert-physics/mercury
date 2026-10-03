# Mercury

## C++

The C++ library can be found in the lib directory.  This library is built using cmake and can be built using the standard tools 
```
cmake -S lib -B build-lib
cmake --build build-lib
```

This code can be integrated into C++/cmake build systems using 

```
include(FetchContent)


FetchContent_Declare(
  Mercury
  GIT_REPOSITORY https://github.com/vovasolo/pymercury
  SOURCE_SUBDIR lib
)
FetchContent_MakeAvailable(Mercury)

...

target_link_libraries(TARGET_NAME PRIVATE Mercury)

```

## Python

This repo also contains a Python binding of the C++ library.  These binding, created using pybind11 and contained in the src/include directories, can be installed using pip.  To install the remote copy you can use 
```
pip install git+https://github.com/vovasolo/pymercury.git
```
If you are doing development you can install the local version using 
```
pip install .
```

In both cases you can access the package using 
```
import mercury
```