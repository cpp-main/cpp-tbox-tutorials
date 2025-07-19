#include <tbox/main/module.h>
#include <tbox/base/log.h>
#include <set>
#include <map>
#include <iostream>

//! CPU密集运算
uint64_t Fibonacci(int n) {
    if (n <= 1) return n;
    return Fibonacci(n - 1) + Fibonacci(n - 2);
}

class MyModule : public tbox::main::Module {
  public:
    explicit MyModule(tbox::main::Context &ctx)
        : tbox::main::Module("my", ctx)
    { }

  public:
    virtual bool onStart() {
        startCalculateTask(45);
        startCalculateTask(40);
        startCalculateTask(43);
        startCalculateTask(42);
        return true;
    }

    //! 启动计算任务
    void startCalculateTask(int n) {
        struct Tmp {
            int n = 0;
            uint64_t result = 0;
        };
        auto tmp = std::make_shared<Tmp>();
        tmp->n = n;
        unfinished_task_.insert(n);

        LogTrace("start caculate fibonacci %d", n);
        ctx().thread_pool()->execute(
            [tmp] {
                //! 工作线程中执行，要避免让它访问到公共资源
                LogTrace("fibonacci %d underway", tmp->n);
                tmp->result = Fibonacci(tmp->n);
            },
            [this, tmp] {
                //! Loop线程中执行，允许访问公共资源
                onCaculateTaskFinished(tmp->n, tmp->result);
            }
        );
        //! Q: 为什么不能在工作线程中直接调用onCaculateTaskFinished()？
        //! A: 因为onCaculateTaskFinished()函数会访问到公共变量unfinished_task_,result_，这是非常危险的事情。
        //!    Loop线程与工作线程都去访问就会导致变量竞态，如果没有加锁就会引起不可预知的异常。
        //!    为此，onCaculateTaskFinished()必须在工作线程做完事情后的后续处理函数中调用，后续处理函数是由
        //!    Loop线程进执行，所以是安全的。
        //!    由于工作线程不能直接访问公共数据，那么它正常工作需要的数据与执行的结果都需要通过Tmp这个结构体
        //!    进行传递。由于tmp所指的对象被Loop线程与工作线程的访问在时间上是隔离的，所以是安全的。
    }

    //! 计算任务完成后的处理
    void onCaculateTaskFinished(int n, uint64_t result) {
        LogTrace("caculate fibonacci %d result: %llu", n, result);
        unfinished_task_.erase(n);  //! 从未完成任务中移除n
        results_[n] = result;       //! 将结果存放到result_中

        if (unfinished_task_.empty()) {
            LogTrace("=== all task finished ===");
            for (auto item : results_) {
                std::cout << item.first << " --> " << item.second << std::endl;
            }
        }
    }

  private:
    std::set<int> unfinished_task_;
    std::map<int, uint64_t> results_;
};

namespace tbox {
namespace main {
void RegisterApps(Module &apps, Context &ctx) {
  apps.add(new MyModule(ctx));
}
}
}
