#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "Utility.h"

#include <thread>
#include <queue>
#include <vector>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <atomic>

class ThreadPool {
public:
    bool start(int nThreads);
    bool queueJob(std::function<void()>);
    bool stop();
    bool isBusy();
    void addTasksAndWaitUntilCompleted(std::function<void()> addTasks);
private:
    void threadLoop();

    bool m_shouldTerminate;
    std::mutex m_queueMutex;
    std::condition_variable m_mutexCondition;
    std::vector<std::thread> m_threads;
    std::queue<std::function<void()>> m_jobs;
    std::atomic<int> m_nJobsPending;
    std::mutex m_mainThreadMutex;
    std::condition_variable m_mainThreadCondition;
};

#endif
