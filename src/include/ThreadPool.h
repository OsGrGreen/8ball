// Threadpool struct

#pragma once

#include "ThreadQueue.h"
#include "ThreadWorker.h"
#include "PoolTask.h"
#include "BallUtils.h"

#include <vector>
#include <future>
#include <memory>

using namespace std;

class ThreadWorker;

class ThreadPool {
public:
    ThreadPool();
    ~ThreadPool();

    template <typename T, typename... Args>
    future<std::invoke_result_t<T&>> submit(T&& task, Args&&... args);

    template <typename T>
    future<std::invoke_result_t<T&>> submit_task(T&& task);

    template <typename T>
    future<std::invoke_result_t<T&>> submit_chunks(T&& task, vector<T>, uint32_t max_chunk);

    void do_work(ThreadWorker& worker);

private:
    WorkQueue m_workqueue;   
};

template <typename T>
future<std::invoke_result_t<T&>> ThreadPool::submit_task(T&& task) {
    using O = std::invoke_result_t<T&>;
    std::promise<O> promise;
    std::future<O> future = promise.get_future();

    std::unique_ptr<PoolTask> work = std::make_unique<Work<std::decay_t<T>>>(
        std::forward<T>(task),
        std::move(promise)
    );
    m_workqueue.push(std::move(work));

    return future;
}

template <typename T>
future<std::invoke_result_t<T&>> ThreadPool::submit_chunks(T&& task, vector<T>, uint32_t max_chunk) {

}