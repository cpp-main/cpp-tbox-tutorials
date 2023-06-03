#include <tbox/main/module.h>
#include <tbox/base/log.h>

class MyModule : public tbox::main::Module {
  public:
    explicit MyModule(tbox::main::Context &ctx) : tbox::main::Module("my", ctx) { }
    virtual ~MyModule() { }

  public:
    virtual bool onInit(const tbox::Json &js) override {
        LogFatal("this is fatal log");
        LogErr("this is error log");
        LogWarn("this is warn log");
        LogNotice("this is notice log");
        LogInfo("this is info log");
        LogDbg("this is info log");
        LogTrace("this is trace log");
        LogUndo();
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
