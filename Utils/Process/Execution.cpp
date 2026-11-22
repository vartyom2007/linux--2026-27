#include "Execution.h"

#include <unistd.h>
#include <sys/wait.h>
#include <cerrno>

namespace Process {

int ExecuteCommand(const char* command) {
    if (command == nullptr) {
        return 1;
    }

    pid_t pid = fork();

    if (pid == -1) {
        return -1;
    }

    if (pid == 0) {
        execl("/bin/sh", "sh", "-c", command, nullptr);
        _exit(127);
    }

    int status;

    while (waitpid(pid, &status, 0) == -1) {
        if (errno != EINTR) {
            return -1;
        }
    }

    return status;
}

}
