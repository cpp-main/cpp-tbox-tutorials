#include <tbox/main/module.h>
#include <tbox/base/log.h>
#include <tbox/flow/state_machine.h>

class MyModule : public tbox::main::Module {
  public:
    explicit MyModule(tbox::main::Context &ctx)
        : tbox::main::Module("my", ctx)
    { }

  public:
    virtual bool onInit(const tbox::Json &js) {
        initSm();
        return true;
    }

    virtual bool onStart() override {
        sm_.start();
        return true;
    }
    
    virtual void onStop() override {
        ctx().timer_pool()->cancel(timer_token_);
        sm_.stop();
    }

  protected:
    enum class StateId {
        kTerm = 0,
        kGreen,     //! 绿灯
        kYellow,    //! 黄灯
        kRed,       //! 红灯
    };

    enum class EventId {
        kAny = 0,
        kTimeout,   //! 时间到了
    };

    void initSm() {
        using Event = tbox::flow::Event;

        auto do_enter_green  = [this] (Event) { setGreenLightStatus(true);  startTimer(20); };
        auto do_exit_green   = [this] (Event) { setGreenLightStatus(false); };
        auto do_enter_yellow = [this] (Event) { setYellowLightStatus(true); startTimer(5); };
        auto do_exit_yellow  = [this] (Event) { setYellowLightStatus(false); };
        auto do_enter_red    = [this] (Event) { setRedLightStatus(true);    startTimer(15); };
        auto do_exit_red     = [this] (Event) { setRedLightStatus(false); };

        sm_.newState(StateId::kGreen,   do_enter_green,     do_exit_green,  "Green");
        sm_.newState(StateId::kYellow,  do_enter_yellow,    do_exit_yellow, "Yellow");
        sm_.newState(StateId::kRed,     do_enter_red,       do_exit_red,    "Red");

        sm_.setInitState(StateId::kGreen);  //! 如果不明确指定，那么第一个被创建的状态就是起始状态

        sm_.addRoute(StateId::kGreen,   EventId::kTimeout, StateId::kYellow,    nullptr, nullptr, "Timeout");
        sm_.addRoute(StateId::kYellow,  EventId::kTimeout, StateId::kRed,       nullptr, nullptr, "Timeout");
        sm_.addRoute(StateId::kRed,     EventId::kTimeout, StateId::kGreen,     nullptr, nullptr, "Timeout");
    }

    void startTimer(int sec) {
        ctx().timer_pool()->cancel(timer_token_);
        timer_token_ = ctx().timer_pool()->doAfter(
            std::chrono::seconds(sec),
            [this] { sm_.run(EventId::kTimeout); }
        );
    }

    void setGreenLightStatus(bool is_on) {
        LogPrintf(is_on ? LOG_LEVEL_IMPORTANT : LOG_LEVEL_INFO, "green light: %s", is_on ? "on" : "off");
    }
    void setYellowLightStatus(bool is_on) {
        LogPrintf(is_on ? LOG_LEVEL_WARN: LOG_LEVEL_NOTICE, "yellow light: %s", is_on ? "on" : "off");
    }
    void setRedLightStatus(bool is_on) {
        LogPrintf(is_on ? LOG_LEVEL_FATAL: LOG_LEVEL_ERROR, "red light: %s", is_on ? "on" : "off");
    }

  private:
    tbox::flow::StateMachine sm_;
    tbox::eventx::TimerPool::TimerToken timer_token_;
};

namespace tbox {
namespace main {
void RegisterApps(Module &apps, Context &ctx) {
  apps.add(new MyModule(ctx));
}
}
}
