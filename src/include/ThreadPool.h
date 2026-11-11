// Threadpool struct

#pragma once

#include "ThreadQueue.h"
#include "PoolTask.h"
#include "ThreadWorker.h"
#include "BallUtils.h"

#include <vector>
#include <future>
#include <memory>
#include <type_traits>
#include <functional>
#include <utility>

using namespace std;


class ThreadPool {
public:
    ThreadPool(uint16_t starting_workers);
    ~ThreadPool();

    template <typename T, typename... Args>
    future<std::invoke_result_t<T&, Args ...>> submit(T&& task, Args&&... args);

    template <typename T>
    future<std::invoke_result_t<T&>> submit_task(T&& task);

    template <typename T>
    future<std::invoke_result_t<T&>> submit_chunks(T&& task, vector<T>, uint32_t max_chunk);

    void do_work(ThreadWorker& worker);
    //Work steal_work(ThreadWorker& worker);

    uint64_t num_workers();

    bool divide_work();

private:
    void spawn_thread();
    void kill_threads();

private:
    WorkQueue m_workqueue;   
    std::vector<std::unique_ptr<ThreadWorker>> m_workers;

private:
    uint8_t max_workers = 1;
};

template <typename T, typename... Args>
future<std::invoke_result_t<T&, Args ...>> ThreadPool::submit(T&& task, Args&&... args) {
    auto bound_task = std::bind_front(
        std::forward<T>(task),
        std::forward<Args>(args)...
    );

    return submit_task(std::move(bound_task));
}

template <typename T>
future<std::invoke_result_t<T&>> ThreadPool::submit_task(T&& task) {
    using Task = std::decay_t<T>;
    using O = std::invoke_result_t<Task&>;
    
    std::promise<O> promise;
    std::future<O> future = promise.get_future();

    std::unique_ptr<PoolTask> work = std::make_unique<SingleWork<Task>>(
        std::forward<T>(task),
        std::move(promise)
    );
    m_workqueue.push(std::move(work));

    return future;
}

template <typename T>
future<std::invoke_result_t<T&>> ThreadPool::submit_chunks(T&& task, vector<T>, uint32_t max_chunk) {

}