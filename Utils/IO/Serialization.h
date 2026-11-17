#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <string>
#include <type_traits>
#include <stdexcept>
#include <cstring>

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

namespace IO {

template <typename T>
void Serialize(const T& obj, const std::string& filename) {
    static_assert(std::is_trivially_copyable_v<T>);

    int fd = open(filename.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);

    if (fd == -1)
        throw std::runtime_error("open failed");

    if (ftruncate(fd, sizeof(T)) == -1) {
        close(fd);
        throw std::runtime_error("ftruncate failed");
    }

    void* ptr = mmap(nullptr, sizeof(T),
                     PROT_READ | PROT_WRITE,
                     MAP_SHARED, fd, 0);

    if (ptr == MAP_FAILED) {
        close(fd);
        throw std::runtime_error("mmap failed");
    }

    std::memcpy(ptr, &obj, sizeof(T));

    munmap(ptr, sizeof(T));
    close(fd);
}

template <typename T>
T Deserialize(const std::string& filename) {
    static_assert(std::is_trivially_copyable_v<T>);

    int fd = open(filename.c_str(), O_RDONLY);

    if (fd == -1)
        throw std::runtime_error("open failed");

    struct stat st;

    if (fstat(fd, &st) == -1) {
        close(fd);
        throw std::runtime_error("fstat failed");
    }

    if (st.st_size != sizeof(T)) {
        close(fd);
        throw std::runtime_error("invalid file size");
    }

    void* ptr = mmap(nullptr, sizeof(T),
                     PROT_READ, MAP_PRIVATE, fd, 0);

    if (ptr == MAP_FAILED) {
        close(fd);
        throw std::runtime_error("mmap failed");
    }

    T obj;
    std::memcpy(&obj, ptr, sizeof(T));

    munmap(ptr, sizeof(T));
    close(fd);

    return obj;
}

}

#endif
