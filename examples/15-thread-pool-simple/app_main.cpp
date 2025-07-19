#include <tbox/main/module.h>
#include <tbox/base/log.h>

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
        int n = 40;
        LogTrace("start caculate fibonacci %d", n);
        //! 委托线程池执行任务
        ctx().thread_pool()->execute(
            [n] {
                //! 在线程池中执行
                LogTrace("fibonacci %d calculating", n);
                auto result = Fibonacci(n); //! 进行CPU密集的运算
                LogTrace("caculate fibonacci %d result: %llu", n, result);
            }
        );

        LogTag();
        return true;
    }
};

namespace tbox {
namespace main {
void RegisterApps(Module &apps, Context &ctx) {
  apps.add(new MyModule(ctx));
}
}
}
