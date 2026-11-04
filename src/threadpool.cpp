// Threadpool implementation

#include "ThreadPool.h"
#include "ThreadWorker.h"

ThreadPool::ThreadPool() {

}
ThreadPool::~ThreadPool() {

}

void ThreadPool::do_work(ThreadWorker& worker){
    auto value = m_workqueue.pop();
    value->run(worker);
}
