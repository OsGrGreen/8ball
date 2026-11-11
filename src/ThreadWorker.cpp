// Threadworker

#include "ThreadWorker.h"
#include "ThreadPool.h"
#include "ballutils.h"

ThreadWorker::ThreadWorker(ThreadPool& pool): m_poolReference(pool), m_workerQueue(), m_thread(&ThreadWorker::work, this) {}
 
ThreadWorker::~ThreadWorker() {
    std::printf("Deconstructing Threadworker\n");
    kill();
    if (m_thread.joinable()) {
        m_thread.join();
    }
    std::printf("Finished waiting for thread\n");
}

bool ThreadWorker::kill() {
    bool prev = m_isRunning;
    m_isRunning = false;
    return m_isRunning;
}

bool ThreadWorker::add_work(Work&& w) {
    m_workerQueue.push(std::move(w));
    return true;
}


void ThreadWorker::work() {
    while(m_isRunning) {
        // If has work, then do work running `sequential_load` tasks sequentially
        vector<Work> todo = m_workerQueue.take(sequential_load);
        if(!todo.empty()) {
            for (Work& w : todo) {
                w->run(*this);
            }
        }
        // Otherwise, take work from pool
        m_poolReference.divide_work();
        // Otherwise, steal work
        //m_poolReference.steal_work(*this);
    }
}