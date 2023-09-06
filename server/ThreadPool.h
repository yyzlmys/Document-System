#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <iostream>

class Threadpool
{
public:
    void init(int size);

    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future< std::invoke_result_t<F, Args...> >;
    // std::result_of<F(Args...)>::type是函数返回值的类型，使程序员无需显式指定返回值类型
    // c++14以后，建议使用std::invoke_result或std::invoke_result_t来代替std::result_of

    static Threadpool* getInstance()
    {
        return threadpool;
    }
    ~Threadpool();
private:
    Threadpool() = default;
    Threadpool(const Threadpool& TQ) = delete;
    Threadpool operator=(const Threadpool& TQ) = delete;
    static Threadpool* threadpool;
    void work();
    std::vector< std::thread > workers;
    std::queue< std::function<void()> > tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

Threadpool* Threadpool::threadpool = new Threadpool;

void Threadpool::init(int size)
{
    while (size <= 0)
    {
        std::cout << "The size must be > 0, please re-enter the threadpool size :";
        std::cin >> size;
    }
    std::unique_lock<std::mutex> lock(queue_mutex);
    stop = false;
    lock.unlock();
    for (size_t i = 0; i < size; ++i)
    {
        workers.emplace_back(&Threadpool::work, this); // 可代替push_back，在尾部就地构造
    }
}

template<class F, class... Args>
auto Threadpool::enqueue(F&& f, Args&&... args)
-> std::future< std::invoke_result_t<F, Args...> > // 返回值是future，用于异步获取结果
{
    using return_type = std::invoke_result_t<F, Args...>; // 别名声明

    // std::packaged_task将可调用对象封装为可异步执行的任务，通过get_future获取存储执行结果的future
    auto task = std::make_shared< std::packaged_task<return_type()> > // std::make_shared构造shared_ptr智能指针
        (
            std::bind(std::forward<F>(f), std::forward<Args>(args)...) // 完美转发参数，保持其原有特性
        );
    // 构造了一个指向std::packaged_task类型的shared_ptr智能指针，其中std::packaged_task由bind绑定器构造

    std::future<return_type> res = task->get_future(); // std::packaged_task的结果会保存到res中
    std::unique_lock<std::mutex> lock(queue_mutex);
    if (stop) throw std::runtime_error("enqueue on stopped ThreadPool"); // 无catch语句处理，程序直接终止
    tasks.emplace([task]() { (*task)(); });
    // 把std::packaged_task封装为function包装器，以包装器形式调用，结果依然可以保存至res
    lock.unlock();
    condition.notify_one();
    return res;
}

inline Threadpool::~Threadpool()
{
    std::unique_lock<std::mutex> lock(queue_mutex);
    stop = true;
    lock.unlock();
    condition.notify_all(); // 唤醒所有阻塞的线程
    for (std::thread& worker : workers) worker.join();
}

void Threadpool::work()
{
    while (1)
    {
        std::function<void()> task;
        std::unique_lock<std::mutex> lock(this->queue_mutex);
        while (this->tasks.empty() && !this->stop) // 防止虚假唤醒
        {
            this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
        }
        if (this->stop) return; // stop若为true，直接结束
        task = std::move(this->tasks.front()); // 移动语义，再访问队首元素将导致未定义行为
        this->tasks.pop(); // 及时弹出
        lock.unlock();
        task(); //执行任务
    }
}

#endif

