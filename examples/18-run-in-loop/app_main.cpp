#include <tbox/main/module.h>
#include <tbox/base/log.h>
#include <thread>

class MyModule : public tbox::main::Module {
  public:
    explicit MyModule(tbox::main::Context &ctx)
        : tbox::main::Module("my", ctx)
    { }

  public:
    virtual bool onStart() override {
        //! 创建子线程，令其周期性调用runInLoop()
        thread_ = std::thread([this]  {
            int index = 0;
            while (keep_running_) {
                //! 延时1秒
                std::this_thread::sleep_for(std::chrono::seconds(1));
                ++index;

                LogDbg("thread index: %d", index);
                //! 委托给Loop线程
                ctx().loop()->runInLoop([index] {
                    LogDbg("loop   index: %d", index);
                });
            }
        });
        return true;
    }
    
    virtual void onStop() override { keep_running_ = false; }
    virtual void onCleanup() override { thread_.join(); }

  private:
    bool keep_running_ = true;
    std::thread thread_;
};

namespace tbox {
namespace main {
void RegisterApps(Module &apps, Context &ctx) {
  apps.add(new MyModule(ctx));
}
}
}
