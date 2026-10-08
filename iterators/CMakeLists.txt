add_executable(
    filter_integer_iterator_demo
    FilterIntegerIterator.cpp
)

add_executable(
    reverse_array_iterator_demo
    ReverseArrayIterator.cpp
)

add_executable(
    circular_int_iterator_demo
    CircularIntIterator.cpp
)

target_include_directories(
    filter_integer_iterator_demo
    PRIVATE
    ${PROJECT_SOURCE_DIR}/Utils
)

target_include_directories(
    reverse_array_iterator_demo
    PRIVATE
    ${PROJECT_SOURCE_DIR}/Utils
)

target_include_directories(
    circular_int_iterator_demo
    PRIVATE
    ${PROJECT_SOURCE_DIR}/Utils
)
