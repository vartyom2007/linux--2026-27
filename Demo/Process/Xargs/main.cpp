#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    int n = 0;
    int start = 1;

    if (argc > 2 && std::string(argv[1]) == "-n") {
        n = std::stoi(argv[2]);
        start = 3;
    }

    if (start >= argc) {
        return 1;
    }

    std::vector<std::string> args;
    std::string word;

    while (std::cin >> word) {
        args.push_back(word);
    }

    std::string command = argv[start];

    for (int i = start + 1; i < argc; i++) {
        command += " ";
        command += argv[i];
    }

    if (n == 0) {
        n = args.size();
    }

    for (size_t i = 0; i < args.size(); i += n) {
        std::vector<char*> exec_args;

        exec_args.push_back(const_cast<char*>(command.c_str()));

        for (size_t j = i; j < args.size() && j < i + n; j++) {
            exec_args.push_back(const_cast<char*>(args[j].c_str()));
        }

        exec_args.push_back(nullptr);

        pid_t pid = fork();

        if (pid == 0) {
            execvp(exec_args[0], exec_args.data());
            _exit(127);
        }

        if (pid < 0) {
            return 1;
        }

        waitpid(pid, nullptr, 0);
    }

    return 0;
}
