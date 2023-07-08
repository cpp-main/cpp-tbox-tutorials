# 第一个程序

创建自己的工程目录 00-first-demo，再在该目录下创建 Makefile 文件，内容如下:
```Makefile
TARGET:=demo

CXXFLAGS:=-I$(HOME)/.tbox/include
LDFLAGS:=-L$(HOME)/.tbox/lib -rdynamic
LIBS:=-ltbox_main -ltbox_terminal -ltbox_network -ltbox_eventx -ltbox_event -ltbox_log -ltbox_util -ltbox_base -lpthread -ldl

$(TARGET):
	g++ -o $(TARGET) $(LDFLAGS) $(LIBS)
```

[示例工程目录](00-first-demo)

然后执行 `make && ./demo`，效果:  
![执行效果](images/000-first-demo.png)

看到上面的显示，说明 ./demo 已经正常运行了。按 ctrl+c 可以正常退出程序。  
我们可以看到，我们的第一个程序就这么运行起来了。虽然我们什么代码都没有写，但它 tbox.main 框架自身是可以运行的。就像是一个只有火车头，没有车厢的火车一样。  

-------
[[返回主页]](README.md)
