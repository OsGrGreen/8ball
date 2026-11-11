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

///////// SingleWork
// 
// 
/////////

template <typename T>
class SingleWork : public PoolTask {
public:
    using O = decltype(std::declval<T&>()());

    SingleWork(T func, promise<O>&& p): m_func(std::move(func)), m_value(std::move(p))  {}
    void run(ThreadWorker &worker) override;

private:
    T m_func;
    promise<O> m_value;
};

template <typename T>
void SingleWork<T>::run(ThreadWorker& worker) {
    m_value.set_value(std::invoke(m_func));
}

///////// Dividable Work
// 
// 
/////////
template <typename I, typename O>
class DividableWork : public PoolTask {
public:
    DividableWork(function<O(I)> func, promise<O> p): m_func(std::move(m_func)), m_value(std::move(p))  {}
    void run(ThreadWorker &worker) override;
    bool has_stealable() override;

private:
    vector<I> m_workqueue;

    function<O(I)> m_func;
    promise<vector<O>> m_value;
};

template <typename I, typename O>
void DividableWork<I, O>::run(ThreadWorker& worker) {
    vector<O> collected_values;
    while (!(m_workqueue.empty())) {
        I input = m_workqueue.pop();
        O output = std::invoke(m_func, input);
        collected_values.push_back(std::move(output));
    }
    m_value.set_value(std::move(collected_values));
}

template <typename I, typename O>
bool DividableWork<I, O>::has_stealable() {
    return m_workqueue.empty();
}