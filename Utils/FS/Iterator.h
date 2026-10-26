#ifndef UTILS_FS_ITERATOR_H
#define UTILS_FS_ITERATOR_H

#include <dirent.h>
#include <cerrno>
#include <cstring>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <stack>
#include <sys/stat.h>
#include <cassert>
#include <set>

namespace FS {

class DirectoryIterator {
private:
    struct State {
        DIR* dir = nullptr;
        struct dirent* entry = nullptr;

        explicit State(const std::string& path)
        {
            dir = opendir(path.c_str());

            if (dir == nullptr) {
                throw std::runtime_error(
                    "Cannot open directory: " + path
                );
            }

            try {
                next();
            } catch (...) {
                closedir(dir);
                dir = nullptr;
                throw;
            }
        }

        ~State()
        {
            if (dir != nullptr) {
                closedir(dir);
            }
        }

        void next()
        {
            entry = nullptr;

            if (dir == nullptr) {
                return;
            }

            while (true) {
                errno = 0;
                struct dirent* current = readdir(dir);

                if (current == nullptr) {
                    if (errno != 0) {
                        throw std::runtime_error(
                            "Cannot read directory"
                        );
                    }

                    return;
                }

                if (std::strcmp(current->d_name, ".") == 0 ||
                    std::strcmp(current->d_name, "..") == 0) {
                    continue;
                }

                entry = current;
                return;
            }
        }
    };

    std::shared_ptr<State> _state;
    std::vector<std::string> _entries;
    std::size_t _index = 0;

    void load_entries(const std::string& path)
    {
        State state(path);

        while (state.entry != nullptr) {
            _entries.emplace_back(state.entry->d_name);
            state.next();
        }
    }

    struct dirent _current{};

    void update_current()
    {
        if (_index >= _entries.size()) {
            _state.reset();
            return;
        }

        std::strncpy(
            _current.d_name,
            _entries[_index].c_str(),
            sizeof(_current.d_name) - 1
        );

        _current.d_name[sizeof(_current.d_name) - 1] = '\0';
    }

public:
    using iterator_category = std::input_iterator_tag;
    using value_type = struct dirent;
    using difference_type = std::ptrdiff_t;
    using pointer = const struct dirent*;
    using reference = const struct dirent&;
    DirectoryIterator begin() const{
	return *this;
}
    DirectoryIterator end() const{
	return DirectoryIterator();
}

    DirectoryIterator() = default;

    explicit DirectoryIterator(const std::string& path)
    {
        load_entries(path);

        if (!_entries.empty()) {
            _state = std::make_shared<State>(path);
            update_current();
        }
    }

    reference operator*() const
    {
        if (_index >= _entries.size()) {
            throw std::out_of_range(
                "Cannot dereference end iterator"
            );
        }

        return _current;
    }

    pointer operator->() const
    {
        return &operator*();
    }

    DirectoryIterator& operator++()
    {
        if (_index < _entries.size()) {
            ++_index;
            update_current();
        }

        return *this;
    }

    DirectoryIterator operator++(int)
    {
        DirectoryIterator temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const DirectoryIterator& other) const
    {
        bool this_end = _index >= _entries.size();
        bool other_end = other._index >= other._entries.size();

        if (this_end || other_end) {
            return this_end && other_end;
        }

        return _entries == other._entries &&
               _index == other._index;
    }

    bool operator!=(const DirectoryIterator& other) const
    {
        return !(*this == other);
    }
};





class RecursiveDirectoryIterator {
private:
    struct Frame {
        DIR* dir = nullptr;
        std::string path;

        Frame(const std::string& p, bool at_begin = true)
            : path(p)
        {
            dir = opendir(path.c_str());

            if (!dir) {
                throw std::runtime_error(
                    "Cannot open directory: " + path
                );
            }

            if (!at_begin) {
                // Используется только при восстановлении позиции.
            }
        }

        Frame(const Frame& other) : path(other.path)
        {
            long position = telldir(other.dir);

            dir = opendir(path.c_str());

            if (!dir) {
                throw std::runtime_error(
                    "Cannot copy directory iterator: " + path
                );
            }

            seekdir(dir, position);
        }

        Frame(Frame&& other) noexcept
            : dir(other.dir), path(std::move(other.path))
        {
            other.dir = nullptr;
        }

        Frame& operator=(Frame&& other) noexcept
        {
            if (this != &other) {
                if (dir) {
                    closedir(dir);
                }

                dir = other.dir;
                path = std::move(other.path);
                other.dir = nullptr;
            }

            return *this;
        }

        Frame& operator=(const Frame&) = delete;

        ~Frame()
        {
            if (dir) {
                closedir(dir);
            }
        }
    };

    std::vector<Frame> _stack;
    struct dirent _entry{};
    std::string _entry_path;
    bool _valid = false;

    void advance()
    {
        _valid = false;

        while (!_stack.empty()) {
            Frame& frame = _stack.back();

            errno = 0;
            struct dirent* entry = readdir(frame.dir);

            if (!entry) {
                if (errno != 0) {
                    throw std::runtime_error(
                        "Cannot read directory: " + frame.path
                    );
                }

                _stack.pop_back();
                continue;
            }

            if (std::strcmp(entry->d_name, ".") == 0 ||
                std::strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            _entry = *entry;
            _entry_path = frame.path + "/" + entry->d_name;

            struct stat info {};

            if (lstat(_entry_path.c_str(), &info) != 0) {
                throw std::runtime_error(
                    "Cannot inspect entry: " + _entry_path
                );
            }

            // Не переходим по символическим ссылкам.
            if (S_ISDIR(info.st_mode)) {
                _stack.emplace_back(_entry_path);
            }

            _valid = true;
            return;
        }
    }

public:
    using iterator_category = std::input_iterator_tag;
    using value_type = struct dirent;
    using difference_type = std::ptrdiff_t;
    using pointer = const struct dirent*;
    using reference = const struct dirent&;
    RecursiveDirectoryIterator begin() const {      
    return *this;
    }                                                     
    RecursiveDirectoryIterator end() const {
        return  RecursiveDirectoryIterator();
    }   
    RecursiveDirectoryIterator() = default;

    explicit RecursiveDirectoryIterator(const std::string& path)
    {
        _stack.emplace_back(path);
        advance();
    }

    RecursiveDirectoryIterator(
        const RecursiveDirectoryIterator& other
    )
        : _stack(other._stack),
          _entry(other._entry),
          _entry_path(other._entry_path),
          _valid(other._valid)
    {
    }

    RecursiveDirectoryIterator& operator=(
        const RecursiveDirectoryIterator& other
    )
    {
        if (this != &other) {
            RecursiveDirectoryIterator temp(other);
            swap(temp);
        }

        return *this;
    }

    void swap(RecursiveDirectoryIterator& other) noexcept
    {
        _stack.swap(other._stack);

        using std::swap;
        swap(_entry, other._entry);
        swap(_entry_path, other._entry_path);
        swap(_valid, other._valid);
    }

    reference operator*() const
    {
        if (!_valid) {
            throw std::out_of_range(
                "Cannot dereference end iterator"
            );
        }

        return _entry;
    }

    pointer operator->() const
    {
        return &operator*();
    }

    RecursiveDirectoryIterator& operator++()
    {
        if (_valid) {
            advance();
        }

        return *this;
    }

    RecursiveDirectoryIterator operator++(int)
    {
        RecursiveDirectoryIterator temp(*this);
        ++(*this);
        return temp;
    }

    bool operator==(
        const RecursiveDirectoryIterator& other
    ) const
    {
        if (!_valid && !other._valid) {
            return true;
        }

        return _valid == other._valid &&
               _entry_path == other._entry_path &&
               _stack.size() == other._stack.size();
    }

    bool operator!=(
        const RecursiveDirectoryIterator& other
    ) const
    {
        return !(*this == other);
    }
};


} 

#endif
