
#include "PoolTask.h"
#include "ThreadWorker.h"
#include "ThreadPool.h"

#include <cstdlib>
#include <iomanip>
#include <utility>
#include <iostream>

#include <chrono>
#include <thread>

uint32_t five() {
    return 5;
}

uint32_t hard_task(int i, int o) {
    // DO WORK
    //std::this_thread::sleep_for(2000ms);
    int c = i * o; // 10
    int a = c >> o;
    return a * i;
}

int main(int argc, char *argv[]) {

    auto func = []() {
        return five();
    };

    ThreadPool tp{1};
    int tmp1 = 5;
    int tmp2 = 3;
    auto hard_result = tp.submit(move(hard_task), move(tmp1), tmp2);
    tmp2 = 2;
    auto result = tp.submit_task(move(func));

    std::cout << "rem is: " << tmp1 << ", " << tmp2 << '\n';

    std::cout << "res is: " << result.get() << '\n';
    std::cout << "hard res is: " << hard_result.get() << '\n';
    
    return argc == 3 ? EXIT_SUCCESS : EXIT_FAILURE; // optional return value
}