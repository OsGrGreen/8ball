
#include "PoolTask.h"
#include "ThreadWorker.h"
#include "ThreadPool.h"

#include <cstdlib>
#include <iomanip>
#include <utility>
#include <iostream>

uint32_t five() {
    return 5;
}

uint32_t hard_task(int i, int o) {
    // DO WORK

    int c = i * o;
    int a = c >> o;
    return a * i;
}

int main(int argc, char *argv[])
{
    std::cout << "argc == " << argc << '\n';
    
    for (int ndx{}; ndx != argc; ++ndx) {
        std::cout << "argv[" << ndx << "] == " << std::quoted(argv[ndx]) << '\n';
    }
    
    std::cout << "argv[" << argc << "] == "
              << static_cast<void*>(argv[argc]) << '\n';


    auto func = []() {
        return five();
    };


    ThreadWorker tw;
    ThreadPool tp;
    int tmp1 = 5;
    int tmp2 = 2;
    auto result = tp.submit_task(move(func));

    tp.do_work(tw);

    std::cout << "res is: " << result.get() << '\n';
    
    
    return argc == 3 ? EXIT_SUCCESS : EXIT_FAILURE; // optional return value
}