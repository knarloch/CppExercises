#include <chrono>
#include <functional>
#include <thread>
#include <sys/wait.h>
#include <iostream>
#include <filesystem>

#include "worker.hpp"


void print_duration(std::string tag, int times, std::chrono::nanoseconds result) {
    tag.resize(24, ' ');
    auto avg_duration = (result.count()/10e3 ) / times;
    std::cout << tag << std::setw(7) << times << " times: " << std::setw(15)<< result.count() << "ns (" << std::setw(5)
              << std::setprecision(3) << std::fixed << avg_duration << "us each)\n";
}

std::chrono::nanoseconds fork_and_wait(int times) {
    auto t0 = std::chrono::high_resolution_clock::now();
    for (auto i = 0; i < times; i++) {
        auto pid = fork();
        if (pid == 0) {
            exit(0);
        }
        int status;
        waitpid(pid, &status, 0);
    }
    auto result = std::chrono::high_resolution_clock::now() - t0;
    print_duration(__func__, times, result);
    return result;
}

std::chrono::nanoseconds fork_work_and_wait(int times, std::function<void()> do_some_work) {
    auto t0 = std::chrono::high_resolution_clock::now();
    for (auto i = 0; i < times; i++) {
        auto pid = fork();
        if (pid == 0) {
            do_some_work();
            exit(0);
        }
        int status;
        waitpid(pid, &status, 0);
    }
    auto result = std::chrono::high_resolution_clock::now() - t0;
    print_duration(__func__, times, result);
    return result;
}

std::chrono::nanoseconds fork_exec_and_wait(int times, std::filesystem::path worker_path) {
    auto t0 = std::chrono::high_resolution_clock::now();

    for (auto i = 0; i < times; i++) {
        auto pid = fork();
        if (pid == 0) {
            execl(worker_path.c_str(), worker_path.filename().c_str(), nullptr);
        }
        int status;
        waitpid(pid, &status, 0);
    }
    auto result = std::chrono::high_resolution_clock::now() - t0;
    print_duration(__func__, times, result);
    return result;
}

std::chrono::nanoseconds thread_and_join(int times) {
    auto t0 = std::chrono::high_resolution_clock::now();
    for (auto i = 0; i < times; i++) {
        auto t = std::thread{[] {}};
        t.join();
    }
    auto result = std::chrono::high_resolution_clock::now() - t0;
    print_duration(__func__, times, result);
    return result;
}

std::chrono::nanoseconds thread_work_and_join(int times, std::function<void()> do_some_work) {
    auto t0 = std::chrono::high_resolution_clock::now();
    for (auto i = 0; i < times; i++) {
        auto t = std::thread{do_some_work};
        t.join();
    }
    auto result = std::chrono::high_resolution_clock::now() - t0;
    print_duration(__func__, times, result);
    return result;
}

int main() {
    int n = 1000;
    thread_and_join(n);
    fork_and_wait(n);
    thread_work_and_join(n, do_some_work);
    fork_work_and_wait(n, do_some_work);
    fork_exec_and_wait(n, "./worker");

    return 0;
}