#include <tbox/main/module.h>
#include <tbox/base/log.h>

class MyModule : public tbox::main::Module {
  public:
    explicit MyModule(tbox::main::Context &ctx) : tbox::main::Module("my", ctx) { }
    virtual ~MyModule() { }

  public:
    virtual bool onInit(const tbox::Json &js) override { LogTag(); return true; }
    virtual bool onStart() override { LogTag(); return true; }
    virtual void onStop() override { LogTag(); }
    virtual void onCleanup() override { LogTag(); }
};

namespace tbox {
namespace main {
void RegisterApps(Module &apps, Context &ctx) {
  apps.add(new MyModule(ctx));
}

std::string GetAppDescribe() {
  return "This is CppTbox Demo, writen by Hevake Lee.";
}

void GetAppVersion(int &major, int &minor, int &rev, int &build) {
  major = 1;
  minor = 2;
  rev = 3;
  build = 4;
}

std::string GetAppBuildTime() {
  return std::string(__DATE__) + " " + __TIME__;
}

}
}
