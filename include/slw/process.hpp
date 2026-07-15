#ifndef SLW_PROCESS_HPP
#define SLW_PROCESS_HPP

#include <slw/containers/list.hpp>
#include <slw/strings.hpp>
#include <slw/exceptions.hpp>
#include <slw/environment.hpp>
#include <slw/thread.hpp>
#include <slw/io/file.hpp>

#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

namespace slw
{

class process
{
public:

    struct result {
        int code;
        string out;
        string err;
    };

    struct pipe
    {
        union {
            struct {
                int read;
                int write;
            };
            int fds[2];
        };

        ~pipe() {
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

    process() = default;

    process(const string& path, slw::list<string> args, slw::list<string> env = environ::get_list())
    {
        if (!open(path, args, env)) {
            throw last_system_error();
        }
    }

    ~process() = default;

    static result run(const string& path, slw::list<string> args, slw::list<string> env = environ::get_list()) {
        process proc(path, args, env);
        proc.wait();
        return proc.result();
    }

    inline const result& result() {
        return _result;
    }

    bool open(const string& path, slw::list<string> args, slw::list<string> env = environ::get_list())
    {
        if (::pipe(_stdin.fds) < 0 || ::pipe(_stdout.fds) < 0 || ::pipe(_stderr.fds) < 0) {
            return false;
        }

        pid_t pid = fork();
        if (pid < 0) {
            return false;
        }
        else if (pid == 0) {
            if (::dup2(_stdin.read, STDIN_FILENO) < 0) {
                ::exit(errno);
            }

            if (::dup2(_stdout.write, STDOUT_FILENO) < 0) {
                ::exit(errno);
            }

            if (::dup2(_stderr.write, STDERR_FILENO) < 0) {
                ::exit(errno);
            }

            _stdin.close();
            _stdout.close();
            _stderr.close();
            
            list<const char *> argv;
            argv.reserve(args.size() + 2);

            argv.push_back(path.c_str());
            for (auto& str : args) {
                argv.push_back(str.c_str());
            }
            argv.push_back(nullptr);

            list<const char *> envp;
            envp.reserve(env.size());

            for (auto& str : env) {
                envp.push_back(str.c_str());
            }
            envp.push_back(nullptr);

            // TODO: Support null environment

            int result = ::execve(
                path.c_str(),
                const_cast<char * const *>(argv.data()),
                const_cast<char * const *>(envp.data())
            );

            throw last_system_error();
        }

        _pid = pid;

        _stdin.close_read();
        _stdout.close_write();
        _stderr.close_write();

        fcntl(_stdin.write, F_SETFL, O_NONBLOCK);
        fcntl(_stdout.read, F_SETFL, O_NONBLOCK);
        fcntl(_stderr.read, F_SETFL, O_NONBLOCK);

        return true;
    }

    inline void send_signal(int signum) {
        int result = ::kill(_pid, signum);
        if (result < 0) {
            throw last_system_error();
        }
    }

    inline void terminate() {
        send_signal(SIGTERM);
    }

    inline void kill() {
        send_signal(SIGKILL);
    }

    inline bool poll() {
        int info = 0;
        pid_t result = ::waitpid(_pid, &info, WNOHANG);
        if (result <= 0) {
            return false;
        }
        
        // TODO: Is this correct?
        communicate();

        _result.code = WEXITSTATUS(info);
        return true;
    }

    inline int wait() {
        int info = 0;
        pid_t result = ::waitpid(_pid, &info, 0);
        if (result < 0) {
            return false;
        }
        
        // TODO: Is this correct?
        communicate();

        _result.code = WEXITSTATUS(info);
        return _result.code;
    }

    inline void communicate(string_view input = {})
    {
        char buffer[4096];
        ssize_t size;

        while (!input.empty()) {
            size = ::write(_stdin.write, input.data(), input.size());
            if (size < 0) {
                if (errno == EAGAIN) {
                    continue;
                }
                throw last_system_error();
            }

            input = input.substr(size);
        }

        buffer[0] = '\0';

        for (;;) {
            size = ::read(_stdout.read, buffer, sizeof(buffer));
            if (size == 0) {
                break;
            }
            else if (size < 0) {
                if (errno == EAGAIN) {
                    break;
                }
                throw last_system_error();
            }
            else {
                buffer[size] = '\0';
                _result.out += buffer;
            }
        }

        buffer[0] = '\0';

        for (;;) {
            size = ::read(_stderr.read, buffer, sizeof(buffer));
            if (size == 0) {
                break;
            }
            else if (size < 0) {
                if (errno == EAGAIN) {
                    break;
                }
                throw last_system_error();
            }
            else {
                buffer[size] = '\0';
                _result.err += buffer;
            }
        }
    }

private:

    pid_t _pid;

    pipe _stdin;
    pipe _stdout;
    pipe _stderr;

    struct result _result;

}; // class process

} // namespace slw

#endif // SLW_PROCESS_HPP
