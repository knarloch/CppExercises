add_executable(fork_vs_thread fork_vs_thread.cpp worker.cpp)
add_executable(worker worker_main.cpp worker.cpp)

add_dependencies(fork_vs_thread worker)

add_test(NAME fork_vs_thread COMMAND fork_vs_thread)
