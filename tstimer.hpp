#pragma once

#include <iostream>
#include <functional>
#include <chrono>
#include <thread>
#include <atomic>
#include <memory>
#include <mutex>
#include <condition_variable>

using namespace std;

class TsTimer{
public:
    TsTimer() :expired_(true), try_to_expire_(false){
    }
 
    TsTimer(const TsTimer& t){
        expired_ = t.expired_.load();
        try_to_expire_ = t.try_to_expire_.load();
    }
    ~TsTimer(){
        Expire();
        //std::cout << "timer destructed!" << std::endl;
    }
    
    //开线程周期性执行定时任务
    void StartTimer(int interval, std::function<void()> task){
        if (expired_ == false){
            //          std::cout << "timer is currently running, please expire it first..." << std::endl;
            return;
        }
        expired_ = false;
        std::thread([this, interval, task](){
            while (!try_to_expire_){
                std::this_thread::sleep_for(std::chrono::milliseconds(interval));
                task();
            }
            //          std::cout << "stop task..." << std::endl;
            {
                std::lock_guard<std::mutex> locker(mutex_);
                expired_ = true;
                expired_cond_.notify_one();
            }
        }).detach();
    }
 
    void Expire(){
        if (expired_){
            return;
        }
 
        if (try_to_expire_){
            //std::cout << "timer is trying to expire, please wait..." << std::endl;
            return;
        }
        try_to_expire_ = true;
        {
            std::unique_lock<std::mutex> locker(mutex_);
            expired_cond_.wait(locker, [this]{return expired_ == true; });
            if (expired_ == true){
                //std::cout << "timer expired!" << std::endl;
                try_to_expire_ = false;
            }
        }
    }
    
    //执行一次同步定时任务
    template<typename callable, class... arguments>
    void SyncWait(int after, callable&& f, arguments&&... args){
 
        std::function<typename std::result_of<callable(arguments...)>::type()> task
            (std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));

        std::this_thread::sleep_for(std::chrono::milliseconds(after));
        task();
    }

    //开线程执行一次异步定时任务
    template<typename callable, class... arguments>
    void AsyncWait(int after, callable&& f, arguments&&... args){
        std::function<typename std::result_of<callable(arguments...)>::type()> task
            (std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
 
        std::thread([after, task](){
            std::this_thread::sleep_for(std::chrono::milliseconds(after));
            task();
        }).detach();
    }
     
private:
    std::atomic<bool> expired_;
    std::atomic<bool> try_to_expire_;
    std::mutex mutex_;
    std::condition_variable expired_cond_;
};
