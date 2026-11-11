// Thread worker

#pragma once

#include "BallUtils.h"

using namespace std;

class ThreadPool;

class ThreadWorker {
public:
    ThreadWorker(ThreadPool& pool);
    ~ThreadWorker();

    bool kill();

    bool add_work(Work&& w);
    //Work steal_work();

    const uint64_t work_amount() { return m_workerQueue.size(); };  

private:
    void work();

private:
    WorkQueue m_workerQueue;
    ThreadPool& m_poolReference;
    thread m_thread;
    bool m_isRunning = true;
    uint16_t sequential_load = 2;
};