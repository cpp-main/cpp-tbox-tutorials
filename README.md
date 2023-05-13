# CppTbox 的入门教程

本项目为 [cpp-tbox](https://gitee.com/cpp-master/cpp-tbox) 的入门教程。  
您可以通过下面一个个的教程逐步掌握 cpp-tbox 的使用。  

## 1. 第一个程序
创建自己的工程目录,创建 Makefile。

Makefile
```
TARGET:=00_demo

CXXFLAGS:=-I$(HOME)/.tbox/include
LDFLAGS:=-L$(HOME)/.tbox/lib -rdynamic
LIBS:=-ltbox_main -ltbox_terminal -ltbox_network -ltbox_eventx -ltbox_event -ltbox_log -ltbox_util -ltbox_base -lpthread -ldl

$(TARGET):
	g++ -o $(TARGET) $(LDFLAGS) $(LIBS)
```

然后执行 `make && ./00_demo`,效果:
![执行效果](images/first-demo.png)

你可以看到，你的第一个程序就这么运行起来了。虽然你什么代码都没有写，但它 tbox 框架自身是可以运行的。就像是只有火车头，没有车厢的火车一样。  
注意看，你会发现程序在运行之前有一堆提示：
```

```
这是 tbox 运行时，发现没有任何以在运行的负载，向开发者打印的提示。

## 2. 写一个自己的 Module
接下来，我们按第一个课程的提示，编写自己的 `Module`。
