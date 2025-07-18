#include <tbox/main/module.h>
#include <tbox/base/log.h>
#include <tbox/eventx/timer_fd.h>

class MyModule : public tbox::main::Module {
  public:
    explicit MyModule(tbox::main::Context &ctx)
        : tbox::main::Module("my", ctx)
        , oneshot_timer_(ctx.loop())
        , repeat_timer_(ctx.loop())
    { }

  public:
    virtual bool onInit(const tbox::Json &) {
        //! 只设置一个时间，表示单次定时5秒
        oneshot_timer_.initialize(std::chrono::seconds(5));
        oneshot_timer_.setCallback([] { LogDbg("oneshot"); });

        //! 设置了两个时间，第一个时间表示首次触发为100ms，第二个时间表示后续间隔1秒触发
        repeat_timer_.initialize(std::chrono::milliseconds(100), std::chrono::seconds(1));
        repeat_timer_.setCallback([] { LogDbg("repeat"); });

        return true;
    }

    virtual bool onStart() {
        oneshot_timer_.enable();
        repeat_timer_.enable();
        return true;
    }

    virtual void onStop() {
        repeat_timer_.disable();
        oneshot_timer_.disable();
    }

    virtual void onCleanup() {
        repeat_timer_.cleanup();
        oneshot_timer_.cleanup();
    }

  private:
    tbox::eventx::TimerFd oneshot_timer_;
    tbox::eventx::TimerFd repeat_timer_;
};

namespace tbox {
namespace main {
void RegisterApps(Module &apps, Context &ctx) {
  apps.add(new MyModule(ctx));
}
}
}
