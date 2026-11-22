#include <Utils/Process/Execution.h>

#include <cassert>
#include <cstdlib>
#include <sys/wait.h>
int main(){
    // Successful command.
    int status = Process::ExecuteCommand("exit 0");

    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 0);

    // Nonzero exit status.
    status = Process::ExecuteCommand("exit 42");

    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 42);

    // Shell command with redirection.
    status = Process::ExecuteCommand("echo Hello > /dev/null");

    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 0);

    // Shell pipeline.
    status = Process::ExecuteCommand("echo Hello | grep Hello > /dev/null");

    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 0);

    // Nonexistent command.
    status = Process::ExecuteCommand("nonexistent_hw8_command");

    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 127);

    // Compare with the standard implementation.
    const int expected = std::system("exit 17");
    const int actual = Process::ExecuteCommand("exit 17");

    assert(expected == actual);
}
