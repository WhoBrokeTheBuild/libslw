#ifndef SLW_PROCESS_HPP
#define SLW_PROCESS_HPP

#include <slw/chrono.hpp>
#include <slw/environment.hpp>
#include <slw/exceptions.hpp>
#include <slw/list.hpp>
#include <slw/optional.hpp>
#include <slw/strings.hpp>
#include <slw/tuple.hpp>

#include <fcntl.h>
#include <spawn.h>
#include <sys/wait.h>

namespace slw
{

class process
{
public:

    enum pipe_select {
        STDIN = STDIN_FILENO,
        STDOUT = STDOUT_FILENO,
        STDERR = STDERR_FILENO,
        CLOSE = -1,
        DEVNULL = -2,
        PIPE = -3,
    };

    struct options
    {
        list<string> env = {};

        int input = PIPE;
        int output = PIPE;
        int error = PIPE;

        bool search_path = false;
        bool text = false;
    };

    static const options default_options;

    struct result
    {
        int code;
        string output;
        string error;
    };

    process() = default;

    process(list<string> args, options options = default_options) {
        open(args, options);
    }

    ~process() {
        if (_pid >= 0) {
            terminate();
        }
    };

    inline const result& result() {
        return _result;
    }

    void open(list<string> args, options options = default_options)
    {
        if (_pid >= 0) {
            terminate();
        }

        _pid = -1;
        _result = result();

        if (args.empty()) {
            throw invalid_argument("args cannot be empty");
        }

        char * const * argv = nullptr;
        char * const * envp = nullptr;

        list<const char *> arg_list;
        arg_list.reserve(args.size() + 1);

        // args[0] is the program name
        for (auto& str : args) {
            arg_list.push_back(str.c_str());
        }
        arg_list.push_back(nullptr);

        argv = const_cast<char * const *>(arg_list.data());

        // TODO: Support an empty environment
        list<const char *> env_list;
        if (!options.env.empty()) {
            env_list.reserve(options.env.size() + 1);

            for (auto& str : options.env) {
                env_list.push_back(str.c_str());
            }
            env_list.push_back(nullptr);

            envp = const_cast<char * const *>(env_list.data());
        }

        posix_spawn_file_actions_t file_actions;

        posix_spawn_file_actions_init(&file_actions);

        int devnull = -1;
        if (options.input == DEVNULL || options.output == DEVNULL || options.error == DEVNULL) {
            devnull = ::open("/dev/null", O_WRONLY);
        }

        switch (options.input) {
        case STDIN:
            break;

        case CLOSE:
            posix_spawn_file_actions_addclose(&file_actions, STDIN_FILENO);
            break;

        case DEVNULL:
            throw invalid_argument("options.in == DEVNULL, use CLOSE instead");
            // posix_spawn_file_actions_adddup2(&file_actions, devnull, STDIN_FILENO);
            break;

        case PIPE:
            if (pipe(_stdin.fds) < 0) {
                throw last_system_error();
            }

            posix_spawn_file_actions_adddup2(&file_actions, _stdin.read, STDIN_FILENO);
            break;

        default:
            if (options.input < 0) {
                throw invalid_argument("options.in < 0");
            }

            posix_spawn_file_actions_adddup2(&file_actions, static_cast<int>(options.input), STDIN_FILENO);
            break;
        }

        switch (options.output) {
        case STDOUT:
            break;

        case CLOSE:
            posix_spawn_file_actions_addclose(&file_actions, STDOUT_FILENO);
            break;

        case DEVNULL:
            posix_spawn_file_actions_adddup2(&file_actions, devnull, STDOUT_FILENO);
            break;

        case PIPE:
            if (pipe(_stdout.fds) < 0) {
                throw last_system_error();
            }

            posix_spawn_file_actions_adddup2(&file_actions, _stdout.write, STDOUT_FILENO);
            break;

        default:
            if (options.output < 0) {
                throw invalid_argument("options.out < 0");
            }

            posix_spawn_file_actions_adddup2(&file_actions, static_cast<int>(options.output), STDOUT_FILENO);
            break;
        }

        switch (options.error) {
        case STDERR:
            break;

        case CLOSE:
            posix_spawn_file_actions_addclose(&file_actions, STDERR_FILENO);
            break;

        case DEVNULL:
            posix_spawn_file_actions_adddup2(&file_actions, devnull, STDERR_FILENO);
            break;

        case PIPE:
            if (pipe(_stderr.fds) < 0) {
                throw last_system_error();
            }

            posix_spawn_file_actions_adddup2(&file_actions, _stderr.write, STDERR_FILENO);
            break;

        case STDOUT:
            posix_spawn_file_actions_adddup2(&file_actions, STDOUT_FILENO, STDERR_FILENO);
            break;

        default:
            if (options.error < 0) {
                throw invalid_argument("options.err < 0");
            }

            posix_spawn_file_actions_adddup2(&file_actions, static_cast<int>(options.error), STDERR_FILENO);
            break;
        }

        int minfd = std::max({ STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO }) + 1;
        posix_spawn_file_actions_addclosefrom_np(&file_actions, minfd);

        int result;

        if (options.search_path) {
            result = posix_spawnp(
                &_pid,
                argv[0],
                &file_actions,
                nullptr,
                argv,
                envp
            );
        }
        else {
            result = posix_spawn(
                &_pid,
                argv[0],
                &file_actions,
                nullptr,
                argv,
                envp
            );
        }
        
        posix_spawn_file_actions_destroy(&file_actions);
        
        if (result != 0) {
            throw last_system_error();
        }

        _stdin.close_read();
        _stdout.close_write();
        _stderr.close_write();

        // TODO: Check for exit 127?
    }

    inline void detach()
    {
        _pid = -1;

        _stdin.close();
        _stdout.close();
        _stderr.close();
    }

    inline void send_signal(int signum)
    {
        if (_pid < 0) {
            throw system_error(ECHILD, std::generic_category());
        }

        int result = ::kill(_pid, signum);
        if (result < 0) {
            throw last_system_error();
        }
    }

    inline void interrupt() {
        send_signal(SIGINT);
    }

    inline void kill() {
        send_signal(SIGKILL);
    }

    inline void terminate() {
        send_signal(SIGTERM);
    }

    inline bool poll()
    {
        if (_pid < 0) {
            throw system_error(ECHILD, std::generic_category());
        }
        
        int info = 0;
        pid_t result = ::waitpid(_pid, &info, WNOHANG);
        if (result <= 0) {
            return false;
        }

        _pid = -1;

        std::tie(_result.output, _result.error) = read_output();

        _result.code = WEXITSTATUS(info);
        return true;
    }

    inline struct result wait()
    {
        if (_pid < 0) {
            throw system_error(ECHILD, std::generic_category());
        }

        int info = 0;
        pid_t result = ::waitpid(_pid, &info, 0);
        if (result < 0) {
            throw last_system_error();
        }

        _pid = -1;

        std::tie(_result.output, _result.error) = read_output(1s);

        _result.code = WEXITSTATUS(info);
        return _result;
    }

    inline void write_input(string_view input)
    {
        if (_pid < 0) {
            throw system_error(ECHILD, std::generic_category());
        }

        while (!input.empty()) {
            ssize_t size = ::write(_stdin.write, input.data(), input.size());
            if (size < 0) {
                if (errno == EINTR) {
                    continue;
                }
                throw last_system_error();
            }

            input = input.substr(size);
        }
    }

    inline tuple<string, string> read_output(optional<nanoseconds> timeout = nullopt)
    {
        std::string out;
        std::string err;

        if (_stdout.read < 0 && _stderr.read < 0) {
            return { out, err };
        }

        fd_set readfds;
        FD_ZERO(&readfds);
        // FD_SET(_stdin.write, &fds);

        if (_stdout.read > 0) {
            FD_SET(_stdout.read, &readfds);
        }

        if (_stderr.read > 0) {
            FD_SET(_stderr.read, &readfds);
        }

        char buffer[4096];
        ssize_t size;

        struct timespec timespec = { 0, 0 };
        if (timeout) {
            timespec = to_timespec(timeout.value());
        }

        int maxfd = std::max({ _stdout.read, _stderr.read });
        int result = ::pselect(maxfd + 1, &readfds, nullptr, nullptr, &timespec, nullptr);
        if (result == 0) {
            // Timed out
            printf("Timed out\n");
            return { out, err };
        }

        if (FD_ISSET(_stdout.read, &readfds)) {
            size = ::read(_stdout.read, buffer, sizeof(buffer) - 1);
            if (size < 0) {
                if (errno == EINTR) {
                    return { out, err };
                }
                throw last_system_error();
            }
            else if (size > 0) {
                buffer[size] = '\0';
                out += buffer;
            }
        }

        if (FD_ISSET(_stderr.read, &readfds)) {
            size = ::read(_stderr.read, buffer, sizeof(buffer) - 1);
            if (size < 0) {
                if (errno == EINTR) {
                    return { out, err };
                }
                throw last_system_error();
            }
            else if (size > 0) {
                buffer[size] = '\0';
                out += buffer;
            }
        }

        return { out, err };
    }

private:

    struct _pipe
    {
        union {
            struct {
                int read;
                int write;
            };
            int fds[2];
        };

        _pipe()
            : read(-1)
            , write(-1)
        { }

        ~_pipe() {
            close();
        }

        void close_read() {
            ::close(read);
            read = -1;
        }

        void close_write() {
            ::close(write);
            write = -1;
        }

        void close() {
            close_read();
            close_write();
        }
    };

    pid_t _pid = -1;

    _pipe _stdin;
    _pipe _stdout;
    _pipe _stderr;

    struct result _result;

}; // class process

inline const process::options process::default_options = {};

} // namespace slw

#endif // SLW_PROCESS_HPP
