#ifndef COFFEE_APP_H
#define COFFEE_APP_H

#include <tbox/main/module.h>
#include <tbox/flow/state_machine.h>

namespace coffee {

class App : public tbox::main::Module {
  public:
    explicit App(tbox::main::Context &ctx);
    virtual ~App();

  public:
    virtual bool onInit(const tbox::Json &js) override;
    virtual bool onStart() override;
    virtual void onStop() override;
    virtual void onCleanup() override;

  protected:
    enum class StateId {
        kTerm = 0,
        kCheck,     //!< 自检中
        kIdle,      //!< 空闲中
        kWorking,   //!< 工作中
        kFault,     //!< 故障中
        kMax
    };
    std::string ToString(StateId id);

    enum class EventId {
        kAny = 0,
        kMake,      //!< 制作
        kSucc,      //!< 成功
        kFail,      //!< 失败
        kStop,      //!< 停止
        kCheck,     //!< 自检
        kMax
    };
    std::string ToString(EventId id);

    void initSm();
    void initShell();

    void onTimerTick();

  private:
    tbox::event::TimerEvent *timer_ = nullptr;
    tbox::flow::StateMachine sm_;

    int remain_sec_ = 0;
};

}

#endif //COFFEE_APP_H
