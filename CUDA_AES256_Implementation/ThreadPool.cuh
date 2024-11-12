#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>
#include <tuple>

#define _THREAD_TUPLE_STORED_

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
        std::function<void()> func_;
    };
}