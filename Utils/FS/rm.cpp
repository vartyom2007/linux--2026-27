#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

bool remove_file(const std::string& path, bool recursive) {
    struct stat st;

    if (lstat(path.c_str(), &st) == -1) {
        perror(path.c_str());
        return false;
    }

    // Если это директория
    if (S_ISDIR(st.st_mode)) {
        if (!recursive) {
            std::cerr << "Cannot remove directory: " << path << '\n';
            return false;
        }

        DIR* dir = opendir(path.c_str());

        if (!dir) {
            perror(path.c_str());
            return false;
        }

        bool success = true;
        struct dirent* entry;

        while ((entry = readdir(dir)) != nullptr) {
            std::string name = entry->d_name;

            if (name == "." || name == "..")
                continue;

            std::string child = path + "/" + name;

            if (!remove_file(child, true))
                success = false;
        }

        closedir(dir);

        if (rmdir(path.c_str()) == -1) {
            perror(path.c_str());
            return false;
        }

        return success;
    }

    // Файл или символическая ссылка
    if (unlink(path.c_str()) == -1) {
        perror(path.c_str());
        return false;
    }

    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: rm [-r] file...\n";
        return 1;
    }

    bool recursive = false;
    int start = 1;

    if (std::string(argv[1]) == "-r") {
        recursive = true;
        start = 2;
    }

    if (start == argc) {
        std::cerr << "Missing file operand\n";
        return 1;
    }

    bool success = true;

    for (int i = start; i < argc; i++) {
        if (!remove_file(argv[i], recursive))
            success = false;
    }

    return success ? 0 : 1;
}
