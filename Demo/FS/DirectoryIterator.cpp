#include <Utils/FS/Iterator.h>

#include <dirent.h>
#include <cassert>
#include <cstdlib>
#include <exception>
#include <iterator>
#include <set>
#include <string>
#include <type_traits>
#include <utility>

using FS::DirectoryIterator;

//-----------------------------------------------------------
// Compile-Time Checks
//-----------------------------------------------------------

static_assert(std::is_copy_constructible_v<DirectoryIterator>);
static_assert(std::is_copy_assignable_v<DirectoryIterator>);

static_assert(std::is_same_v<
    typename std::iterator_traits<DirectoryIterator>::iterator_category,
    std::input_iterator_tag
>);

static_assert(std::is_same_v<
    decltype(*std::declval<DirectoryIterator&>()),
    const struct dirent&
>);

static_assert(std::is_same_v<
    decltype(std::declval<DirectoryIterator&>().operator->()),
    const struct dirent*
>);

static_assert(std::is_same_v<
    decltype(++std::declval<DirectoryIterator&>()),
    DirectoryIterator&
>);

static_assert(std::is_same_v<
    decltype(std::declval<DirectoryIterator&>()++),
    DirectoryIterator
>);

//-----------------------------------------------------------
// Demo
//-----------------------------------------------------------

int main()
{
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

    std::set<std::string> entries;

    for (const auto& entry : DirectoryIterator(root)) {
        entries.insert(entry.d_name);
    }

    assert((entries == std::set<std::string>{
        "alpha", "beta", "empty", "root.txt"
    }));

    // Empty directory.
    assert(DirectoryIterator(root + "/empty") == DirectoryIterator{});

    // Invalid directory.
    bool exception_thrown = false;

    try {
        DirectoryIterator invalid(root + "/missing");
    }
    catch (const std::exception&) {
        exception_thrown = true;
    }

    assert(exception_thrown);
}
