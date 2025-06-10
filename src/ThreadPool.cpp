#include "ThreadPool.h"
#include "Utility.h"

#include <thread>
#include <mutex>
#include <functional>

bool ThreadPool::start(int nThreads) {
    m_shouldTerminate = false;

    for (int i = 0; i < nThreads; i++) {
        m_threads.emplace_back(&ThreadPool::threadLoop, this);
    }

    return true;
}

bool ThreadPool::queueJob(std::function<void()> job) {
    {
        u_lock lock(m_queueMutex);
        m_jobs.push(job);
    }

    m_nJobsPending++;

    m_mutexCondition.notify_one();

    return true;
}

bool ThreadPool::stop() {
    {
        u_lock lock(m_queueMutex);
        m_shouldTerminate = true;
    }

    m_mutexCondition.notify_all();

    for (std::thread& thread : m_threads) {
        thread.join();
    }

    m_threads.clear();

    return true;
}

bool ThreadPool::isBusy() {
    bool isBusy;
    {
        u_lock lock(m_queueMutex);
        isBusy = !m_jobs.empty();
    }

    return isBusy;
}

void ThreadPool::threadLoop() {
    while (true) {
        std::function<void()> job;
        {
            u_lock lock(m_queueMutex);

            m_mutexCondition.wait(lock, [this] {
                return !m_jobs.empty() || m_shouldTerminate;
            });

            if (m_shouldTerminate) {
                return;
            }

            job = m_jobs.front();

            m_jobs.pop();
        }

        job();

        {
            std::thread::id id = std::this_thread::get_id();

            u_lock lock(m_mainThreadMutex);

            if ( --m_nJobsPending == 0 ) {
                m_mainThreadCondition.notify_one();
            }
        }
    }
}

void ThreadPool::addTasksAndWaitUntilCompleted(std::function<void()> addTasks) {
    u_lock lock(m_mainThreadMutex);

    addTasks();

    m_mainThreadCondition.wait(lock);
}
