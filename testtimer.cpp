#include<iostream>
#include<string>
#include<memory>

#include"tstimer.hpp"

using namespace std;

void EchoFunc(std::string&& s){
    std::cout << "test : " << s << endl;
}
 
int main(){
    TsTimer t;
    //开线程周期性执行定时任务
    t.StartTimer(1000, std::bind(EchoFunc,"hello world!"));
    std::this_thread::sleep_for(std::chrono::seconds(4));
    std::cout << "try to expire timer!" << std::endl;
    //t.Expire();
 
    std::this_thread::sleep_for(std::chrono::seconds(2));
 
    //只执行一次定时任务
    //同步
    t.SyncWait(1000, EchoFunc, "hello SyncWait!");

    //开线程异步
    t.AsyncWait(1000, EchoFunc, "hello AsyncWait!");
 
    return 0;
}