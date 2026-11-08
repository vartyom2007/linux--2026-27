#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
namespace fs = std::filesystem;

//-----------------------------------------------------------// Helpers//-----------------------------------------------------------

const std::string root = "/tmp/hw6_rm_test";

int run(const std::string& arguments){
    return std::system(("./rm " + arguments +
                        " > /dev/null 2>&1").c_str());
}

void write_file(const fs::path& path, const std::string& content){
    std::ofstream file(path, std::ios::binary);
    assert(file.is_open());

    file << content;
    assert(file.good());
}

void create_test_environment(){
    fs::remove_all(root);

    fs::create_directories(root + "/source/nested/deep");
    fs::create_directories(root + "/source/empty");
    fs::create_directories(root + "/protected");

    write_file(root + "/file.txt", "Hello Linux!");
    write_file(root + "/file1.txt", "First");
    write_file(root + "/file2.txt", "Second");
    write_file(root + "/source/a.txt", "A");
    write_file(root + "/source/nested/b.txt", "B");
    write_file(root + "/source/nested/deep/c.txt", "C");
    write_file(root + "/protected/important.txt", "Important");

    fs::create_symlink(root + "/protected",
                       root + "/directory_link");

    fs::create_symlink(root + "/file.txt",
                       root + "/file_link");
}

//-----------------------------------------------------------// Regular File Tests//-----------------------------------------------------------

void test_regular_file(){
    assert(run(root + "/file.txt") == 0);
    assert(!fs::exists(root + "/file.txt"));
}

void test_multiple_files(){
    assert(run(root + "/file1.txt " +
               root + "/file2.txt") == 0);

    assert(!fs::exists(root + "/file1.txt"));
    assert(!fs::exists(root + "/file2.txt"));
}

void test_empty_file(){
    write_file(root + "/empty.txt", "");

    assert(run(root + "/empty.txt") == 0);
    assert(!fs::exists(root + "/empty.txt"));
}

//-----------------------------------------------------------// Recursive Removal Tests//-----------------------------------------------------------

void test_recursive_remove(){
    assert(run("-r " + root + "/source") == 0);
    assert(!fs::exists(root + "/source"));
}

void test_empty_directory(){
    fs::create_directory(root + "/empty_dir");

    assert(run("-r " + root + "/empty_dir") == 0);
    assert(!fs::exists(root + "/empty_dir"));
}

void test_multiple_directories(){
    fs::create_directories(root + "/dir1/nested");
    fs::create_directories(root + "/dir2/nested");

    write_file(root + "/dir1/a.txt", "A");
    write_file(root + "/dir2/b.txt", "B");

    assert(run("-r " + root + "/dir1 " +
               root + "/dir2") == 0);

    assert(!fs::exists(root + "/dir1"));
    assert(!fs::exists(root + "/dir2"));
}

//-----------------------------------------------------------// Symbolic Link Tests//-----------------------------------------------------------

void test_file_symlink(){
    assert(run(root + "/file_link") == 0);

    assert(!fs::is_symlink(fs::symlink_status(root + "/file_link")));
}

void test_directory_symlink(){
    assert(run("-r " + root + "/directory_link") == 0);

    assert(!fs::is_symlink(
        fs::symlink_status(root + "/directory_link")));

    // The target directory must remain untouched.
    assert(fs::exists(root + "/protected"));
    assert(fs::exists(root + "/protected/important.txt"));
}

//-----------------------------------------------------------// Error Handling Tests//-----------------------------------------------------------

void test_nonexistent_file(){
    assert(run(root + "/missing.txt") != 0);
}

void test_directory_without_recursive(){
    assert(run(root + "/protected") != 0);

    assert(fs::exists(root + "/protected"));
    assert(fs::exists(root + "/protected/important.txt"));
}

void test_nonexistent_directory(){
    assert(run("-r " + root + "/missing_dir") != 0);
}

//-----------------------------------------------------------// Main//-----------------------------------------------------------

int main(){
    create_test_environment();

    test_regular_file();
    test_multiple_files();
    test_empty_file();

    test_recursive_remove();
    test_empty_directory();
    test_multiple_directories();

    test_file_symlink();
    test_directory_symlink();

    test_nonexistent_file();
    test_directory_without_recursive();
    test_nonexistent_directory();

    fs::remove_all(root);
}
