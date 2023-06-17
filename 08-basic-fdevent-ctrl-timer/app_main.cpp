#include <unistd.h>
#include <tbox/main/module.h>
#include <tbox/base/log.h>
#include <tbox/event/timer_event.h>
#include <tbox/event/fd_event.h>    //! 导入FdEvent

using namespace std::placeholders;

class MyModule : public tbox::main::Module {
  public:
    explicit MyModule(tbox::main::Context &ctx)
        : tbox::main::Module("my", ctx)
        , tick_timer_(ctx.loop()->newTimerEvent())
        , fd_event_(ctx.loop()->newFdEvent())   //! 实例化FdEvent对象
    { }

    virtual ~MyModule() {
        delete fd_event_;
        delete tick_timer_;
    }

  public:
    virtual bool onInit(const tbox::Json &js) override {
        tick_timer_->initialize(std::chrono::seconds(1), tbox::event::Event::Mode::kPersist);
        tick_timer_->setCallback([this] { onTick(); });

        //! 初始化 FdEvent，指定为 stdin 的可读事件，且是持续有效
        fd_event_->initialize(0,
                              tbox::event::FdEvent::kReadEvent,
                              tbox::event::Event::Mode::kPersist);
        //! 设置FdEvent的回调函数
        fd_event_->setCallback(std::bind(&MyModule::onFdEvent, this, _1));
        return true;
    }
    virtual bool onStart() {
        tick_timer_->enable();
        fd_event_->enable();    //! 使能FdEvent
        return true;
    }
    virtual void onStop() {
        fd_event_->disable();
        tick_timer_->disable(); //! 关闭FdEvent
    }

  private:
    void onTick() {
        ++count_;
        LogDbg("count:%d", count_);
    }

    //! stdin有输入事件时的处理
    void onFdEvent(short event) {
        //! 如果是可读事件
        if (event & tbox::event::FdEvent::kReadEvent) {
            char input[100] = {0};
            //! 从 stdin 中读取数据
            auto rsize = ::read(0, input, sizeof(input));

            std::string cmd(input);
            cmd.pop_back(); //! 丢弃尾部的'\n'
            LogDbg("cmd: '%s'", cmd.c_str());

            if (cmd == "on")
                tick_timer_->enable();  //! 开定时器
            else if (cmd == "off")
                tick_timer_->disable(); //! 关定时器
        }
    }

  private:
    tbox::event::TimerEvent *tick_timer_;
    tbox::event::FdEvent *fd_event_;    //!< 定义FdEvent对象
    int count_ = 0;
};

namespace tbox {
namespace main {
void RegisterApps(Module &apps, Context &ctx) {
  apps.add(new MyModule(ctx));
}
}
}
