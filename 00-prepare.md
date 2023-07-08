# 准备工作

下载与构建 cpp-tbox：
```
git clone https://gitee.com/cpp-master/cpp-tbox.git
cd cpp-tbox;
make 3rd-party modules RELEASE=1 STAGING_DIR=$HOME/.tbox
```
完成之后，cpp-tbox 库产生的头文件在 ~/.tbox/include 路径下，库文件在 ~/.tbox/lib 路径下。  
![导出的文件](images/010-tbox.png)

-------
[[返回主页]](README.md)
