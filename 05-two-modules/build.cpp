#include <tbox/main/main.h>

namespace tbox {
namespace main {

std::string GetAppBuildTime() {
  return std::string(__DATE__) + " " + __TIME__;
}

}
}
