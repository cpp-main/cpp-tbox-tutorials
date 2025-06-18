#include <tbox/main/module.h>
#include <tbox/base/log.h>

#include "coffee/app.h"

namespace tbox {
namespace main {

void RegisterApps(Module &apps, Context &ctx) {
  apps.add(new coffee::App(ctx));
}

}
}
