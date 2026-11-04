// PoolTask

#pragma once

#include <vector>
#include <future>
#include <functional>
#include <utility>

using namespace std;

class ThreadWorker;

class PoolTask {
public:
    virtual ~PoolTask() = default;

    virtual void run(ThreadWorker& worker) = 0;
    virtual bool has_stealable() { return false; }
};

///////// Work
// 
// 
/////////

template <typename T>
class Work : public PoolTask {
public:
    using O = decltype(std::declval<T&>()());

    Work(T func, promise<O>&& p): m_func(std::move(func)), m_value(std::move(p))  {}
    void run(ThreadWorker &worker) override;

private:
    T m_func;
    promise<O> m_value;
};

template <typename T>
void Work<T>::run(ThreadWorker& worker) {
    m_value.set_value(std::invoke(m_func));
}

///////// Stealable Work
// 
// 
/////////
template <typename I, typename O>
class StealableWork : public PoolTask {
public:
    StealableWork(function<O(I)> func, promise<O> p): m_func(std::move(m_func)), m_value(std::move(p))  {}
    void run(ThreadWorker &worker) override;
    bool has_stealable() override;

private:
    vector<I> m_workqueue;

    function<O(I)> m_func;
    promise<vector<O>> m_value;
};

template <typename I, typename O>
void StealableWork<I, O>::run(ThreadWorker& worker) {
    vector<O> collected_values;
    while (!(m_workqueue.is_empty())) {
        I input = m_workqueue.pop();
        O output = std::invoke(m_func, input);
        collected_values.push_back(std::move(output));
    }
    m_value.set_value(std::move(collected_values));
}

template <typename I, typename O>
bool StealableWork<I, O>::has_stealable() {
    return m_workqueue.is_empty();
}