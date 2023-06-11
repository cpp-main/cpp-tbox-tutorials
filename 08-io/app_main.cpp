#include <unistd.h>
#include <tbox/main/module.h>
#include <tbox/base/log.h>
#include <tbox/event/fd_event.h>

class MyModule : public tbox::main::Module {
  public:
    explicit MyModule(tbox::main::Context &ctx)
        : tbox::main::Module("my", ctx)
        , fd_event_(ctx.loop()->newFdEvent())
    { }

    virtual ~MyModule() { delete fd_event_; }

  public:
    virtual bool onInit(const tbox::Json &js) override {
        fd_event_->initialize(0,
                              tbox::event::FdEvent::kReadEvent,
                              tbox::event::Event::Mode::kPersist);
        using namespace std::placeholders;
        fd_event_->setCallback(std::bind(&MyModule::onFdEvent, this, _1));
        return true;
    }
    virtual bool onStart() {
        fd_event_->enable();
        return true;
    }
    virtual void onStop() { fd_event_->disable(); }

  private:
    void onFdEvent(short event) {
        if (event & tbox::event::FdEvent::kReadEvent) {
            char buff[100] = {0};
            auto rsize = ::read(0, buff, sizeof(buff));
            LogInfo("read: '%s'", buff);
        }
    }

  private:
    tbox::event::FdEvent *fd_event_;
};

namespace tbox {
namespace main {
void RegisterApps(Module &apps, Context &ctx) {
  apps.add(new MyModule(ctx));
}
}
}
