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
    // std::result_of<F(Args...)>::type�Ǻ�������ֵ�����ͣ�ʹ����Ա������ʽָ������ֵ����
    // c++14�Ժ󣬽���ʹ��std::invoke_result��std::invoke_result_t������std::result_of

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
        workers.emplace_back(&Threadpool::work, this); // �ɴ���push_back����β���͵ع���
    }
}

template<class F, class... Args>
auto Threadpool::enqueue(F&& f, Args&&... args)
-> std::future< std::invoke_result_t<F, Args...> > // ����ֵ��future�������첽��ȡ���
{
    using return_type = std::invoke_result_t<F, Args...>; // ��������

    // std::packaged_task���ɵ��ö����װΪ���첽ִ�е�����ͨ��get_future��ȡ�洢ִ�н����future
    auto task = std::make_shared< std::packaged_task<return_type()> > // std::make_shared����shared_ptr����ָ��
        (
            std::bind(std::forward<F>(f), std::forward<Args>(args)...) // ����ת��������������ԭ������
        );
    // ������һ��ָ��std::packaged_task���͵�shared_ptr����ָ�룬����std::packaged_task��bind��������

    std::future<return_type> res = task->get_future(); // std::packaged_task�Ľ���ᱣ�浽res��
    std::unique_lock<std::mutex> lock(queue_mutex);
    if (stop) throw std::runtime_error("enqueue on stopped ThreadPool"); // ��catch��䴦������ֱ����ֹ
    tasks.emplace([task]() { (*task)(); });
    // ��std::packaged_task��װΪfunction��װ�����԰�װ����ʽ���ã������Ȼ���Ա�����res
    lock.unlock();
    condition.notify_one();
    return res;
}

inline Threadpool::~Threadpool()
{
    std::unique_lock<std::mutex> lock(queue_mutex);
    stop = true;
    lock.unlock();
    condition.notify_all(); // ���������������߳�
    for (std::thread& worker : workers) worker.join();
}

void Threadpool::work()
{
    while (1)
    {
        std::function<void()> task;
        std::unique_lock<std::mutex> lock(this->queue_mutex);
        while (this->tasks.empty() && !this->stop) // ��ֹ��ٻ���
        {
            this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
        }
        if (this->stop) return; // stop��Ϊtrue��ֱ�ӽ���
        task = std::move(this->tasks.front()); // �ƶ����壬�ٷ��ʶ���Ԫ�ؽ�����δ������Ϊ
        this->tasks.pop(); // ��ʱ����
        lock.unlock();
        task(); //ִ������
    }
}

#endif

