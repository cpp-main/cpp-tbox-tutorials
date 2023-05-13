# CppTbox 的入门教程

本项目为 [cpp-tbox](https://gitee.com/cpp-master/cpp-tbox) 的入门教程。  
您可以通过下面一个个的教程逐步掌握 cpp-tbox 的使用。  

## 准备工作

## 第一个程序

创建自己的工程目录 00-first-demo，再在该目录下创建 Makefile 文件，内容如下：
```
TARGET:=00_demo

CXXFLAGS:=-I$(HOME)/.tbox/include
LDFLAGS:=-L$(HOME)/.tbox/lib -rdynamic
LIBS:=-ltbox_main -ltbox_terminal -ltbox_network -ltbox_eventx -ltbox_event -ltbox_log -ltbox_util -ltbox_base -lpthread -ldl

$(TARGET):
	g++ -o $(TARGET) $(LDFLAGS) $(LIBS)
```

[示例代码](00-first-demo)

然后执行 `make && ./00_demo`,效果:
![执行效果](images/first-demo.png)

你可以看到，你的第一个程序就这么运行起来了。虽然你什么代码都没有写，但它 tbox.main 框架自身是可以运行的。就像是一个只有火车头，没有车厢的火车一样。  

## 写一个自己的 Module

如果你很细心，你会发现上面的程序在运行之前有一堆提示：
```
WARN: You should implement tbox::main::RegisterApps().
Exp:

#include <tbox/main/main.h>
#include "your_app.h"

namespace tbox {
namespace main {
void RegisterApps(Module &apps, Context &ctx)
{
    apps.add(new YourApp(ctx));
}
}
}

Err: create director /var/log/00_demo fail.
WARN: filelog init fail
```
这就是 tbox.main 框架在运行时，发现它没有任何可以运行的负载，于是向开发者打印了提示。  
它希望开发者自己去定义一个自己的模块，如 `YourApp`（名字开发者自己取），然后按上面的方式注册到 tbox.main 框架上。  
接下来，我们按第一个课程的提示，编写自己的 `Module`。

创建 app\_main.cpp，内容如下：
```
#include <tbox/main/module.h>

class MyModule : public tbox::main::Module {
  public:
    explicit MyModule(tbox::main::Context &ctx) : tbox::main::Module("my", ctx) {}
};

namespace tbox {
namespace main {
void RegisterApps(Module &apps, Context &ctx) {
  apps.add(new MyModule(ctx));
}
}
}
```
然后再修改 Makefile，将 app\_main.cpp 加入到源文件中，见[Makefile](01-your-first-module/Makefile)  

[代码](01-your-first-module)

编译执行：`make && ./demo`，运行结果：
[运行效果图](images/)  
这次可以看到，它没有再打印之前的提示了。

但是，它也没有体现我们 `MyModule` 的任务行为。  
接下来，我们再往 `MyModule` 中添加自定义的功能。  
```
class MyModule : public tbox::main::Module {
  public:
    explicit MyModule(tbox::main::Context &ctx) : tbox::main::Module("my", ctx) { }
    virtual ~MyModule() { }

    virtual bool onInit() override { LogTag(); }
    virtual bool onStart() override { LogTag(); }
    virtual void onStop() override { LogTag(); }
    virtual void onCleanup() override { LogTag(); }
};
```
我们重写了`MyModule`类的四个虚函数：`onInit()`,`onStart()`,`onStop()`,`onCleanup()`。并在每个虚函数中都添加了`LogTag()`日志打印。  
为了能使用`LogTag()`日志打印函数，我们需要添加`#include <tbox/base/log.h>`

[代码](02-your-first-module)  
在编译的时间，会看到编译警告：
![没有指定LOG\_MODULE\_ID警告]()
它是在说我们程序没有指定日志的模块名。我们可以忽略它，也可以在`CXXFLAGS`中添加`-DLOG_MODULE_ID='"demo"'` 进行指定。  
再编译执行效果：
![运行效果图](images/)  

可以看到，上面的 LogTag() 执行顺序正是`onInit()`,`onStart()`,`onStop()`,`onCleanup()`。
在真实的项目中，就在重写 `tbox::main::Module` 中定义的虚函数与构造函数、析构函数来实现模块的功能的。


有读者会问：我看到上面有 `new MyModule(ctx)`，但我没有看到有对它的`delete`语句，是忘了写吗？  
答：tbox.main 架框会自己管理已注册`tbox::main::Module`派生类的生命期，一旦它被`add()`上去了，它的生命期就不需要开发者操心。

## 完善应用信息

## 一个进程同时运行多个Module

## 日志的打印

## 参数系统
### 内置参数说明
### 实现自定义参数

## 终端

### 使能终端
### 内置命令介绍
### 实现自定义命令
### RPC

## 日志输出

## 主线程向线程池委派任务

## 子线程向主线程委派任务

## 定时器池使用

## 运行时异常捕获功能

## 多层级Module

## IO事件
### 写一个终端交互服务

## 定时器事件


## 信号事件

## HTTP模块

## 使用TcpServer模块写一个echo服务

## 使用TcpClient模块写一个客户端

## 使用TcpAcceptor + TcpConnection 实现echo服务

## 使用TcpConnector + TcpConnection 实现客户端

## 串口使用
### 写一个串口与终端的连接服务
### 写一个两个串口的连接服务
### 写一个串口转TCP的服务
