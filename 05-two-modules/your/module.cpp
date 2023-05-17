#include "module.h"
#include <tbox/base/log.h>

namespace your {

Module::Module(tbox::main::Context &ctx)
  : tbox::main::Module("your", ctx)
{ }

Module::~Module() { }

bool Module::onInit(const tbox::Json &js) {
  LogTag();
  return true;
}

bool Module::onStart() {
  LogTag();
  return true;
}

void Module::onStop() {
  LogTag();
}

void Module::onCleanup() {
  LogTag();
}

}
