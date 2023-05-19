#include <tbox/main/module.h>
#include <tbox/base/log.h>

#include "my/module.h"

namespace tbox {
namespace main {
void RegisterApps(Module &apps, Context &ctx) {
  apps.add(new my::Module(ctx));
}

}
}
