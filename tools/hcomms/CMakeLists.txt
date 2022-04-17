# Build our pyhon->c++ bindings for serial comms on the host
find_package(Boost)
find_package(Python3)
add_library(hello_ext SHARED newton/hcomms/hello.cpp)
set_property(TARGET hello_ext PROPERTY POSITION_INDEPENDENT_CODE ON)
# We must ensure that the library is named 'hello_ext'. If it's prefixed with
# the default 'lib', then python won't import it.
set(CMAKE_SHARED_LIBRARY_PREFIX "")
target_link_libraries(hello_ext python3 boost_python3)
# install the shared library into the python package for distribution...
install(
    TARGETS hello_ext
    DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/newton/hcomms/
)