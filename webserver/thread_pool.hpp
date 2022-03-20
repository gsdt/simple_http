//
// source https://github.com/yhirose/cpp-httplib/blob/master/httplib.h
//

#ifndef TUAN_HTTP_THREAD_POOL_HPP
#define TUAN_HTTP_THREAD_POOL_HPP

#include <mutex>
#include <vector>
#include <list>
#include <thread>

class TaskQueue {
public:
    TaskQueue() = default;

    virtual ~TaskQueue() = default;

    virtual void enqueue(std::function<void()> fn) = 0;

    virtual void shutdown() = 0;

    virtual void on_idle() {}
};

class ThreadPool : public TaskQueue {
public:
    explicit ThreadPool(size_t n) : shutdown_(false) {
        while (n) {
            threads_.emplace_back(worker(*this));
            n--;
        }
    }

    ThreadPool(const ThreadPool &) = delete;

    ~ThreadPool() override = default;

    void enqueue(std::function<void()> fn) override {
        std::unique_lock<std::mutex> lock(mutex_);
        jobs_.push_back(std::move(fn));
        cond_.notify_one();
    }

    void shutdown() override {
        // Stop all worker threads...
        {
            std::unique_lock<std::mutex> lock(mutex_);
            shutdown_ = true;
        }

        cond_.notify_all();

        // Join...
        for (auto &t : threads_) {
            t.join();
        }
    }

private:
    struct worker {
        explicit worker(ThreadPool &pool) : pool_(pool) {}

        void operator()() {
            for (;;) {
                std::function<void()> fn;
                {
                    std::unique_lock<std::mutex> lock(pool_.mutex_);

                    pool_.cond_.wait(
                            lock, [&] { return !pool_.jobs_.empty() || pool_.shutdown_; });

                    if (pool_.shutdown_ && pool_.jobs_.empty()) { break; }

                    fn = pool_.jobs_.front();
                    pool_.jobs_.pop_front();
                }

                assert(true == static_cast<bool>(fn));
                fn();
            }
        }

        ThreadPool &pool_;
    };

    friend struct worker;

    std::vector<std::thread> threads_;
    std::list<std::function<void()>> jobs_;

    bool shutdown_;

    std::condition_variable cond_;
    std::mutex mutex_;
};

#endif //TUAN_HTTP_THREAD_POOL_HPP
