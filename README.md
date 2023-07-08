![cpp-tbox-logo](images/logo-with-slogan.png)  

# CppTbox 的入门教程

本项目为 [cpp-tbox](https://gitee.com/cpp-master/cpp-tbox) 的入门教程。  
您可以通过下面一个个的教程逐步掌握 cpp-tbox 的使用。  

## 准备工作
下载与构建 cpp-tbox：
```
git clone https://gitee.com/cpp-master/cpp-tbox.git
cd cpp-tbox;
make 3rd-party modules RELEASE=1 STAGING_DIR=$HOME/.tbox
```
完成之后，cpp-tbox 库产生的头文件在 ~/.tbox/include 路径下，库文件在 ~/.tbox/lib 路径下。  
![导出的文件](images/010-tbox.png)

## 第一个程序
[点击前往](00-first-demo.md)

## 写一个自己的 Module
[点击前往](01-first-module.md)

## 日志的打印
接下来，我们学习如何打印日志。  
[点击前往](02-add-log-tag.md)  

## 事件驱动模式的编程
[点击前往](03-event-drive.md)  

## 定时器事件
[点击前往](04-timer-event.md)

## IO事件
[点击前往](05-fd-event.md)

## HTTP服务
[点击前往](06-http-server.md)

## 参数系统
[点击前往](07-parameters.md)

## 终端
### 使能终端
### 实现自定义命令
### 内置命令介绍
### RPC接口

## 日志输出

## 线程池的使用

## 子线程向主线程委派任务

## 定时器池使用

## 运行时异常捕获功能

## 多层级Module

## 使用TcpServer模块写一个echo服务

## 使用TcpClient模块写一个客户端

## 使用TcpAcceptor + TcpConnection 实现echo服务

## 使用TcpConnector + TcpConnection 实现客户端

## 串口使用
### 写一个串口与终端的连接服务
### 写一个两个串口的连接服务
### 写一个串口转TCP的服务

## 完善应用信息
上面，我们通过定义一个`MyModule`类，并重写其虚函数，实现了我们一个简单的应用。但仅仅是这么做是不完整的。  
当我们执行：`./demo -h` 时，当我们执行：`./demo -v` 或 `./demo --version` 时看到：  
![无应用信息](images/005-no-desc.png)  
这是因为，我们并没有对当前程序的描述、版本进行描述。我们可以通过在 app\_main.cpp 文件中加如下红框中的代码完善它：  
![添加代码](images/006-add-code.png)  
上面定义了三个函数：`GetAppDescribe()`、`GetAppVersion()`、`GetAppBuildTime()`，分析用于告析 tbox.main 框架当前应用的描述、版本号、编译时间点。  

[完整示例代码](03-add-app-info)

重新构建，再次执行 `./demo -v; ./demo -h` 效果：  
![](images/007-has-desc.png)

## 推荐的工程结构
在上面的示例中，我在 app_main.cpp 实现了所有的代码。而在真正的项目开发中，是不推荐这么做的。  
我们应该对 `app_main.cpp` 进行简单地拆分：  

- 将 `GetAppBuildTime()` 函数放到单独的文件中 build.cpp，并修改 Makefile 保证每次构建时，它都会被重新编译，以确保这个函数返回的时间与构建的时间一致；
- 将 `GetAppVersion()`, `GetAppDescribe()` 放置在 `info.cpp` 中，只要更新版本或应用描述才会改到它；
- 将 `RegisterApps()` 放置在 `apps.cpp` 中，以后基本不会再改它了；
- 创建你自己的应用目录，存放自定义的 `Module` 的实现，所有的业务相关的代码，都在这个目录下实现。

最终产生文件结构：  
```
.
├── apps.cpp    # 定义 RegisterApps()
├── build.cpp   # 定义 GetAppBuildTime()
├── info.cpp    # 定义 GetAppVersion(), GetAppDescribe()
├── Makefile
└── my          # 业务相关Module实现
    ├── app.mk
    ├── module.cpp
    └── module.h
```

[示例工程目录](04-normal-app-demo)

建议：在 my/ 目录下的所有代码，均以 `my` 作为顶层命名空间，以防在下一节的多Module情况下，命名空间污染。  

## 多个Module的工程结构
之前有提过：tbox.main 就像一个火车头。它可以像第一个示例所展示的那样，不带负载运行，也可以像上面一个示例所展示的那样，带一个负载运行。  
本小节将向你展示的是：它可以同时带多个负载运行。也就是说：一个 tbox.main 框架，可以同时将多个毫不相关的业务Module运行在同一个进程里，就像是一个火车头可同时拉多节车相一样。

接下来，我们在上一小节的基础上，再新增一个叫 "your" 的 Module，使之与之前叫 "my" 的 Module 一起运行在是一个进程里。  
步骤：
1. 将 my 目录复制为 your；
2. 进入 your，修改 module.cpp 与 module.h 将命名空间由 `my` 改成 `your`；
3. 打开 your/app.mk，将所有的 `my` 改成 `your`；
4. 打开 Makefile，在 `include my/app.mk` 下面添加 `include your/app.mk`；
5. 打开 apps.cpp，将所有`my`相关的都复制一份命名为`your`。  
![对apps.cpp的修改](images/008-modify-apps.png)  

最后工程文件结构如下：  
```
├── apps.cpp
├── build.cpp
├── info.cpp
├── Makefile
├── my
│   ├── app.mk
│   ├── module.cpp
│   └── module.h
└── your
    ├── app.mk
    ├── module.cpp
    └── module.h
```

[示例工程目录](05-two-modules)

构建后运行：  
![多Module在同一进程运行效果](images/009-two-modules.png)

## 模块插件化
