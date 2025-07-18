#include <tbox/main/module.h>
#include <tbox/base/log.h>
#include <tbox/http/server/server.h> //! 导入Http服务

class MyModule : public tbox::main::Module {
  public:
    explicit MyModule(tbox::main::Context &ctx)
        : tbox::main::Module("my_http", ctx)
        , http_server_(ctx.loop())  //! 实例化Http服务对象
    { }

  public:
    virtual bool onInit(const tbox::Json &js) override {
        //! 初始化Http服务，绑定 0.0.0.0:12345 地址，backlog为2
        if (!http_server_.initialize(tbox::network::SockAddr::FromString("0.0.0.0:12345"), 2))
            return false;

        //! 指定Http服务的请求的处理方法
        http_server_.use(
            [] (tbox::http::server::ContextSptr http_ctx,
                const tbox::http::server::NextFunc &next_func) {
                //! 不管请求的是什么，直接指定回复的body内容
                http_ctx->res().body = R"(<p>Welcome to CppTbox.</br>This is http example.</p>)";
            }
        );

        return true;
    }
    virtual bool onStart() override {
        http_server_.start();   //! 启动Http服务
        return true;
    }
    virtual void onStop() override {
        http_server_.stop();    //! 停止Http服务
    }

  private:
    tbox::http::server::Server http_server_;    //! 定义Http服务对象
};

namespace tbox {
namespace main {
void RegisterApps(Module &apps, Context &ctx) {
  apps.add(new MyModule(ctx));
}
}
}
