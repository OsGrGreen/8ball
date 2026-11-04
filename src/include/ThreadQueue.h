// Threadqueue struct

#pragma once

#include <utility>
#include <vector>

using namespace std;

template <typename T>
class ThreadQueue {
public:
    ThreadQueue()  = default;
    ~ThreadQueue() = default;
    bool is_empty();
    void push(T val);
    T pop();
    
    void splice(uint32_t pos, ThreadQueue<T>& other);
private:
    vector<T> m_queue;
};

template <typename T>
bool ThreadQueue<T>::is_empty() {
    return m_queue.is_empty();
}

template <typename T>
void ThreadQueue<T>::push(T val) {
    m_queue.push_back(std::move(val));
}

template <typename T>
T ThreadQueue<T>::pop() {
    T v = std::move(m_queue.back());
    m_queue.pop_back();
    return v;
}