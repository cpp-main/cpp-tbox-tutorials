#include <tbox/main/module.h>
#include <tbox/base/log.h>

class MyModule : public tbox::main::Module {
  public:
    explicit MyModule(tbox::main::Context &ctx)
        : tbox::main::Module("my", ctx)
    { }

  public:
    virtual bool onStart() {
        //! 启动周期性定时
        repeat_timer_token_ = ctx().timer_pool()->doEvery(std::chrono::seconds(1), [this] { onTick(); });
        //! 启动单次性定时
        oneshot_timer_token_ = ctx().timer_pool()->doAfter(std::chrono::seconds(5), [] { LogTag(); });
        return true;
    }

    virtual void onStop() {
        //! 退出时要记得cancel所有的定时器，以防生命期倒挂问题
        ctx().timer_pool()->cancel(repeat_timer_token_);
        ctx().timer_pool()->cancel(oneshot_timer_token_);
    }

  private:
    void onTick() {
        ++count_;
        LogDbg("count:%d", count_);
    }

  private:
    tbox::eventx::TimerPool::TimerToken repeat_timer_token_;
    tbox::eventx::TimerPool::TimerToken oneshot_timer_token_;
    int count_ = 0;
};

namespace tbox {
namespace main {
void RegisterApps(Module &apps, Context &ctx) {
  apps.add(new MyModule(ctx));
}
}
}
