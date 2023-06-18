#include <unistd.h>
#include <tbox/main/module.h>
#include <tbox/base/log.h>
#include <tbox/event/timer_event.h>
#include <tbox/event/signal_event.h>    //! 导入SignalEvent

using namespace std::placeholders;

class MyModule : public tbox::main::Module {
  public:
    explicit MyModule(tbox::main::Context &ctx)
        : tbox::main::Module("my", ctx)
        , tick_timer_(ctx.loop()->newTimerEvent())
        , signal_event_(ctx.loop()->newSignalEvent())   //! 实例化SignalEvent对象
    { }

    virtual ~MyModule() {
        delete signal_event_;   //! 释放SignalEvent对象
        delete tick_timer_;
    }

  public:
    virtual bool onInit(const tbox::Json &js) override {
        tick_timer_->initialize(std::chrono::seconds(1), tbox::event::Event::Mode::kPersist);
        tick_timer_->setCallback([this] { onTick(); });

        //! 初始化SignalEvent，令其同时监听SIGUSR1,SIGUSR2信号，且是持续有效
        signal_event_->initialize({SIGUSR1, SIGUSR2}, tbox::event::Event::Mode::kPersist);
        //! 设置回调函数，均指向onSignalEvent()
        signal_event_->setCallback(std::bind(&MyModule::onSignalEvent, this, _1));
        return true;
    }
    virtual bool onStart() {
        tick_timer_->enable();
        signal_event_->enable(); //! 使能SignalEvent
        return true;
    }
    virtual void onStop() {
        signal_event_->disable();
        tick_timer_->disable();  //! 关闭SignalEvent
    }

  private:
    void onTick() {
        ++count_;
        LogDbg("count:%d", count_);
    }

    //! 信号事件时的处理
    void onSignalEvent(int signo) {
        LogInfo("got signal: %d", signo);
        if (signo == SIGUSR1)
            tick_timer_->disable(); //! 关闭定时器
        else if (signo == SIGUSR2)
            tick_timer_->enable();  //! 打开定时器
    }

  private:
    tbox::event::TimerEvent  *tick_timer_;
    tbox::event::SignalEvent *signal_event_;    //!< 定义SignalEvent对象
    int count_ = 0;
};

namespace tbox {
namespace main {
void RegisterApps(Module &apps, Context &ctx) {
  apps.add(new MyModule(ctx));
}
}
}
