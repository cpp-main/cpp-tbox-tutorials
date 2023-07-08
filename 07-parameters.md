# 参数系统
参数可以用于不修改代码的情况下改变程序的行为。常规的参数传入有两种方式：(1)环境变量；(2)命令参数。而命令参数是用得最多的。  
在常规的开发中，我们可能这样传入命令参数的：  
```c
int main(int argc, char **argv) {
    if (argc >= 2) {
        ... /// 解析 argc 与 argv
    }
}
```
如果直接这么写会很麻烦，通常我们会使用 `getopt()` 或是 `getopt_long()` 来辅助解析。不过这还是会比较繁琐。每个要传入的参数都得写解析代码，而每个程序要传入的参数还各不相同。这样一来，每开发一个程序都要重写一次参数解析模块，每添加或删除参数也要修改，也比较繁琐。  
好在我们的tbox.main框架中内置了一套开放性的参数系统。它允许程序传入任何类型的参数（字串、数值、数组、对象），而且还支持将参数以配置文件的形式进行导入，让传参变得非常简单。本节我们将通过一系列的例子学习它的使用。  参数可以用于不修改代码的情况下改变程序的行为。常规的参数传入有两种方式：(1)环境变量；(2)命令参数。而命令参数是用得最多的。  
在常规的开发中，我们可能这样传入命令参数的：  
```c
int main(int argc, char **argv) {
    if (argc >= 2) {
        ... /// 解析 argc 与 argv
    }
}
```
如果直接这么写会很麻烦，通常我们会使用 `getopt()` 或是 `getopt_long()` 来辅助解析。不过这还是会比较繁琐。每个要传入的参数都得写解析代码，而每个程序要传入的参数还各不相同。这样一来，每开发一个程序都要重写一次参数解析模块，每添加或删除参数也要修改，也比较繁琐。  
好在我们的tbox.main框架中内置了一套开放性的参数系统。它允许程序传入任何类型的参数（字串、数值、数组、对象），而且还支持将参数以配置文件的形式进行导入，让传参变得非常简单。本节我们将通过一系列的例子学习它的使用。  

## 实现自定义参数
在上面的http示例中，我们在 `onInit()` 中对http_server_对象进行了初始化，令其固定绑定 0.0.0.0 地址与12345端口，且backlog指定为2。将这些初始化参数写死在程序里不是一个好的主意。因为在真实的项目中，这些常常会变更。那能不能在运行的时候，通过参数进行指定呢？当然可以的。  
接下来，我们将使用参数系统，将绑定地址与端口，还以backlog通过参数传入。  

对 app_main.cpp 进行如下修改：  
![parametes示例代码](images/026-parameters-code.png)  

(1) 由于接下来要对 JSON 进行操作，需要包含JSON相关的头文件；  
(2) 实现 `onFillDefaultConfig()` 函数。这个函数是用于让用户指定本模块运行的默认参数。可以看到，我们添加了两个字段"bind_addr"与"backlog"。这两个字段，在`onInit()` 会被使用到；  
(3) 在 `onInit()` 初始化函数中从 js 对象中提取出"bind_addr"与"backlog"；  
(4) 进行使用；  

编译后，不带参数执行。发现与上面节的没有什么变化，还是绑定在 0.0.0.0:12345 端口上。  
接下来，我们给这个程序带上参数进行执行：`./demo -s 'my_http.bind_addr="0.0.0.0:44444"'`  
![parametes示例执行效果](images/027-parameter-result-1.png)  
可见，HTTP服务的绑定端口变了，由之前的 12345 变成了 44444。

这是为什么呢？  
接下来，我们来了解一下 tbox.main 框架参数系数的原理。  

## 原理
由于不同的应用的参数是不同的，tbox.main 框架的参数系统摒弃了传统linux命令的传参方式，采用JSON的形式。实现过程：  

在模块开始运行之前，tbox.main框架会创建一个叫 `js_conf` 的 JSON 对象。
```c++
Json js_conf;
```

然后为每个模块，以模块名创建一个子JSON `js_module`。  
并调用该模块的 `fillDefaultConfig(Json &js)` 方法，让模块根据自身功能的需要对 `js_module` 进行填充。
```c++
for (auto &m : modules) {
    Json &js_module = js_conf[m.name()];
    m.fillDefaultConfig(js_module);
}
```

然后，开始对参数进行逐一解析。 

如遇到 `-s 'a.b.c=xxxx'` 的参数，则将参数转换成这样的JSON：  
```json
"a":{
  "b":{
    "c":xxxx
  }
}
```
将它合并到 `js_conf` 中。
```c++
js_conf.merge_patch(js_tmp);
```

如遇到 `-c jsonfile.js` 的参数，则从 jsonfile.js 文件中导入JSON对象。  
然后，也合并到 `js_conf` 中。
```c++
js_conf.merge_patch(js_tmp);
```

直到解析完所有的参数。最后得到了一个修改后的 `js_conf` JSON对象。  
在对模块进行初始化的时候，以模块名从js_conf取出对应的js_module。在调用模块的`onInit()`时作为参数传入：  
```c++
for (auto &m : modules) {
    const Json &js_module = js_conf[m.name()];
    m.onInit(js_module);
}
```
于是，我们的各自模块在 `onInit()` 虚函数中，得到了一个 JSON 对象的引用。通过获取其中的字段，我们就可以从中获得参数。  
加上 JSON 可以包含任何的数据类型，且支持数组、对象，使得这种参数系统非常灵活，可适应绝大部分的传参需求。

上面的描述为了方便理解，本人刻意简化了其中的细节。具体实现，请阅读源码：

- [module/main/args.cpp](https://gitee.com/cpp-master/cpp-tbox/blob/master/modules/main/args.cpp)
- [module/main/run_in_frontend.cpp](https://gitee.com/cpp-master/cpp-tbox/blob/master/modules/main/run_in_frontend.cpp)

可以得出一个结论：如果我们想给名为"xxx"的模块指定参数，只需要使用 -s 或 -c 在`js_conf`中构建出以下的内容：
```json
{
  "xxx":{ ... }
}
```

明白了其中的原理，我们就能很好地理解上面示例的现象了。  
参数 `-s 'my_http.bind_addr="0.0.0.0:44444"'` 表示：设置 my_http.bind_addr 的参数值为 "0.0.0.0:44444"。于是，在 `MyModule::onInit()` 函数被执行时，所传入的js对象中存在"bind_addr"字段。它的值就是我们在执行参数中指定的 "0.0.0.0:44444"。  
那为什么是 "my_http.bind_addr" 而不是 "abc.bind_addr"？因为在 L10 构造中，指定了 MyModule 的名称为 "my_http"。当然，我们可以取其它名称。  

## 一个`-s`设置多个参数
如果每设置一个参数都使用一个`-s`会使得参数很长，`-s`的数量很多。
也可以多个字段一起传，例如：
```shell
./demo -s 'my_http={"bind_addr":"0.0.0.0:44444","backlog":2}'
```

## 以配置文件形式导入
如果每次程序启动时都带这么长串的参数，是很繁琐的，也不直观。这时，我们可以采用配置文件的形式导入。  
将参数写入到配置文件中，如 config.json
```json
{
  "my_http":{
    "bind_addr":"0.0.0.0:44444",
    "backlog":2  
  }
}
```
然后在运行的时候使用 '-c config.json' 从配置文件中直接导入。
```shell
./demo -c config.json
```
这样就可以很方便地一致性导入大批的参数，简洁，也简单。  
通常，对于固定不变的参数，我们使用导入配置文件的形式加载。对于临时需要调整的，就采用`-s 'a.b.c=xxx'`的形式。  

## 查看help

## 查看当前参数

## 内置参数说明
上面，我们认识到了参数系统的灵活性。除了我们自定义自己业务相关的参数，还有很多tbox.main内置的参数。我们可以通过修改它们来改变tbox.main的行为。

-------
[[返回主页]](README.md)
