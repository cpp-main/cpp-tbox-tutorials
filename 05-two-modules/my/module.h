#include <tbox/main/module.h>

namespace my {

class Module : public tbox::main::Module {
  public:
    explicit Module(tbox::main::Context &ctx);
    virtual ~Module();

  public:
    virtual bool onInit(const tbox::Json &js) override;
    virtual bool onStart() override;
    virtual void onStop() override;
    virtual void onCleanup() override;
};

}
