#include <tbox/main/main.h>

namespace tbox {
namespace main {

std::string GetAppDescribe() {
  return "This is CppTbox Demo, writen by Hevake Lee.";
}

void GetAppVersion(int &major, int &minor, int &rev, int &build) {
  major = 1;
  minor = 2;
  rev = 3;
  build = 4;
}

}
}
