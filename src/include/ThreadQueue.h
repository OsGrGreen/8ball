// Threadqueue struct

#pragma once

#include <utility>
#include <vector>
#include <iterator>

using namespace std;

template <typename T>
class ThreadQueue {
public:
    ThreadQueue()  = default;
    ~ThreadQueue() = default;
    bool empty();
    uint64_t size();
    void push(T val);
    T pop();
    
    void splice(uint32_t pos, ThreadQueue<T>& other);
    vector<T> take(int num);
private:
    vector<T> m_queue;
};

template <typename T>
bool ThreadQueue<T>::empty() {
    return m_queue.empty();
}

template <typename T>
vector<T> ThreadQueue<T>::take(int num) {
    num = std::min(num, static_cast<int>(m_queue.size()));

    vector<T> v;
    v.reserve(num);

    std::move(
        m_queue.begin(),
        m_queue.begin() + num,
        std::back_inserter(v)
    );

    m_queue.erase(m_queue.begin(), m_queue.begin() + num);

    return v;
}


template <typename T>
uint64_t ThreadQueue<T>::size() {
    return m_queue.size();
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