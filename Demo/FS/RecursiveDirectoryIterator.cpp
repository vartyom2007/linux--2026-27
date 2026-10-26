#include <Utils/FS/Iterator.h>

#include <dirent.h>#include <cassert>#include <cstdlib>#include <exception>#include <iterator>#include <set>#include <string>#include <type_traits>#include <utility>

using FS::RecursiveDirectoryIterator;

//-----------------------------------------------------------// Compile-Time Checks//-----------------------------------------------------------

static_assert(std::is_copy_constructible_v<RecursiveDirectoryIterator>);
static_assert(std::is_copy_assignable_v<RecursiveDirectoryIterator>);

static_assert(std::is_same_v<
    typename std::iterator_traits<RecursiveDirectoryIterator>::iterator_category,
    std::input_iterator_tag
>);

static_assert(std::is_same_v<
    decltype(*std::declval<RecursiveDirectoryIterator&>()),
    const struct dirent&
>);

static_assert(std::is_same_v<
    decltype(std::declval<RecursiveDirectoryIterator&>().operator->()),
    const struct dirent*
>);

//-----------------------------------------------------------// Demo//-----------------------------------------------------------

int main(){
    const std::string root = "/tmp/hw4_iterator_test";

    const int result = std::system(
        "rm -rf /tmp/hw4_iterator_test && "
        "mkdir -p /tmp/hw4_iterator_test/alpha/nested "
        "/tmp/hw4_iterator_test/beta "
        "/tmp/hw4_iterator_test/empty && "
        "touch /tmp/hw4_iterator_test/root.txt "
        "/tmp/hw4_iterator_test/alpha/a.txt "
        "/tmp/hw4_iterator_test/alpha/nested/deep.txt "
        "/tmp/hw4_iterator_test/beta/b.txt"
    );

    assert(result == 0);

    std::multiset<std::string> entries;

    for (const auto& entry : RecursiveDirectoryIterator(root)) {
        entries.insert(entry.d_name);
    }

    assert((entries == std::multiset<std::string>{
        "alpha", "nested", "deep.txt", "a.txt",
        "beta", "b.txt", "empty", "root.txt"
    }));

    // Empty directory.
    assert(RecursiveDirectoryIterator(root + "/empty") ==
           RecursiveDirectoryIterator{});

    // Invalid directory.
    bool exception_thrown = false;

    try {
        RecursiveDirectoryIterator invalid(root + "/missing");
    }
    catch (const std::exception&) {
        exception_thrown = true;
    }

    assert(exception_thrown);
}
