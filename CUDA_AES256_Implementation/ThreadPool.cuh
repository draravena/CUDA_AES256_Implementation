#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>
#include <tuple>
#include <set>

namespace thread {
    class Task {
    public:
        template <typename Func, typename... Args>
        Task(Func&& func, Args&&... args)
            : func_(std::bind(std::forward<Func>(func), std::forward<Args>(args)...)) {}
        void invoke() {
            func_();
        }
    private:
        uint32_t priority = UINT32_MAX;
        std::function<void()> func_;
    };
    class ThreadPool {
        public:
            ThreadPool(uint32_t threadCount) : 
            threadCount_(threadCount)
            {
                threads_.reserve(threadCount_);
                for (uint64_t i = 0; i < threadCount; i++) {
                    std::thread thread(&ThreadPool::processingFunction, this);
                    thread.detach();
                    std::thread::id id = thread.get_id();
                    threads_[id] = std::move(thread);
                    freeThreads_.insert(id);
                }
            }
            void start() {
                OPERATIONAL = true;
            }
            void stop() {
                OPERATIONAL = false;
            }
            void enqueue(Task t) {
            }
            bool OPERATIONAL = false;
        private:
            void processingFunction() {
                std::condition_variable cv;
                std::mutex mtx;
                while (true) {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [&] {
                        return !OPERATIONAL;
                    });
                    if (OPERATIONAL == true)
                    freeThreads_.insert(std::this_thread::get_id());
                }
            }
            std::set<std::thread::id> freeThreads_;
            std::unordered_map<std::thread::id ,std::thread> threads_;
            std::priority_queue<Task> p_queue_;
            uint32_t threadCount_ = 0;
    };


}