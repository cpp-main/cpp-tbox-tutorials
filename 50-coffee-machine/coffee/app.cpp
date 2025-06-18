#include "app.h"

#include <random>

#include <tbox/base/log.h>
#include <tbox/base/defines.h>
#include <tbox/event/timer_event.h>
#include <tbox/util/timestamp.h>
#include <tbox/terminal/terminal_nodes.h>
#include <tbox/terminal/helper.h>

namespace coffee {

using SM = tbox::flow::StateMachine;
using Event = tbox::flow::Event;

App::App(tbox::main::Context &ctx)
  : tbox::main::Module("coffee", ctx)
  , timer_(ctx.loop()->newTimerEvent())
{ }

App::~App() {
    CHECK_DELETE_RESET_OBJ(timer_);
}

bool App::onInit(const tbox::Json &js) {
    initSm();
    initShell();

    timer_->initialize(std::chrono::seconds(1), tbox::event::Event::Mode::kPersist);
    timer_->setCallback([this] { onTimerTick(); });

    return true;
}

bool App::onStart() {
    sm_.start();
    return true;
}

void App::onStop() {
    sm_.stop();
}

void App::onCleanup() { }

void App::initSm() {
    auto do_enter_check = [this] (Event) {
        LogDbg("start check");
        remain_sec_ = 3;
        timer_->enable();
    };
    auto do_exit_check = [this] (Event) {
        timer_->disable();
    };

    auto do_enter_working = [this] (Event) {
        LogDbg("start make");
        remain_sec_ = 10;
        timer_->enable();
    };
    auto do_exit_working = [this] (Event) {
        timer_->disable();
    };

    //! 创建状态
    sm_.newState(StateId::kCheck,   do_enter_check, nullptr, "Check");
    sm_.newState(StateId::kIdle,    nullptr, nullptr, "Idle");
    sm_.newState(StateId::kWorking, do_enter_working, do_exit_working, "Working");
    sm_.newState(StateId::kFault,   nullptr, nullptr, "Fault");

    //! 建立状态间的跳转链路
    sm_.addRoute(StateId::kCheck,      EventId::kSucc,     StateId::kIdle,     nullptr, nullptr, "Succ");
    sm_.addRoute(StateId::kCheck,      EventId::kFail,     StateId::kFault,    nullptr, nullptr, "Fault");

    sm_.addRoute(StateId::kIdle,       EventId::kMake,     StateId::kWorking,  nullptr, nullptr, "Make");

    sm_.addRoute(StateId::kWorking,    EventId::kSucc,     StateId::kIdle,     nullptr, nullptr, "Succ");
    sm_.addRoute(StateId::kWorking,    EventId::kFail,     StateId::kFault,    nullptr, nullptr, "Fail");
    sm_.addRoute(StateId::kWorking,    EventId::kStop,     StateId::kIdle,     nullptr, nullptr, "Stop");

    sm_.addRoute(StateId::kFault,      EventId::kCheck,    StateId::kCheck,    nullptr, nullptr, "Check");

    //! 注册状态跳转打印
    sm_.setStateChangedCallback(
        [this] (SM::StateID from, SM::StateID to, Event event) {
            LogDbg("state changed: %s --(%s)-> %s",
                ToString(static_cast<StateId>(from)).c_str(),
                ToString(static_cast<EventId>(event.id)).c_str(),
                ToString(static_cast<StateId>(to)).c_str()
            );
        }
    );
}

bool RamdomFail() {
    std::default_random_engine e;
    std::uniform_int_distribution<int> u(0, 100);

    e.seed(tbox::util::GetUtcMilliseconds());
    return u(e) < 10;
}

void App::onTimerTick() {
    if (remain_sec_ > 0) {
        --remain_sec_;
        if (RamdomFail()) {
            sm_.run(EventId::kFail);
        }

    } else {
        sm_.run(EventId::kSucc);
    }
}

void App::initShell() {
    auto &shell = *ctx().terminal();
    auto dir_node = shell.createDirNode();

    shell.mountNode(shell.rootNode(), dir_node, name());

    tbox::terminal::AddFuncNode(shell, dir_node, "make", [this] { sm_.run(EventId::kMake); });
    tbox::terminal::AddFuncNode(shell, dir_node, "check", [this] { sm_.run(EventId::kCheck); });
    tbox::terminal::AddFuncNode(shell, dir_node, "stop", [this] { sm_.run(EventId::kStop); });
}

std::string App::ToString(StateId id) {
    const char* tbl[] = {
        "Term",
        "Check",
        "Idle",
        "Working",
        "Fault"
    };

    static_assert(NUMBER_OF_ARRAY(tbl) == static_cast<size_t>(StateId::kMax));
    auto index = static_cast<size_t>(id);
    if (index >= 0 && index < NUMBER_OF_ARRAY(tbl))
        return tbl[index];

    return "unknown " + std::to_string(index);
}

std::string App::ToString(EventId id) {
    const char* tbl[] = {
        "Any",
        "Make",
        "Succ",
        "Fail",
        "Stop",
        "Check",
    };

    static_assert(NUMBER_OF_ARRAY(tbl) == static_cast<size_t>(EventId::kMax));
    auto index = static_cast<size_t>(id);
    if (index >= 0 && index < NUMBER_OF_ARRAY(tbl))
        return tbl[index];

    return "unknown " + std::to_string(index);

}


}
