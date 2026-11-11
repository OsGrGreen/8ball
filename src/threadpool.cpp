// Threadpool implementation

#include "ThreadPool.h"
#include "ThreadWorker.h"

#include <cstdio>

ThreadPool::ThreadPool(uint16_t starting_workers): m_workers(), m_workqueue() {
    for (int i = 0; i < starting_workers; ++i) {
        m_workers.push_back(std::make_unique<ThreadWorker>(*this));
    }
}

ThreadPool::~ThreadPool() {
    std::printf("Deconstructing threadpool\n");
}

bool ThreadPool::divide_work() {
    if (m_workqueue.empty()) {
        return false;
    }
    for(auto& tw : m_workers) {
        tw->add_work(std::move(m_workqueue.pop()));
    }
    return true;
}

void ThreadPool::do_work(ThreadWorker& worker){
    auto value = m_workqueue.pop();
    value->run(worker);
}
