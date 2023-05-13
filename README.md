# cpp-tbox-tutorials  -- CppTbox 的入门教程

# 1. 第一个程序
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

# 2. 写一个自己的 Module

TODO...