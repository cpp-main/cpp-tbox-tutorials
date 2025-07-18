#include <tbox/main/module.h>
#include <tbox/base/log.h>
#include <tbox/http/server/server.h>
#include <tbox/base/json.hpp>   //! 引入Json的实现
#include <tbox/util/json.h>     //! 引入GetField()函数，方便操作Json

class MyModule : public tbox::main::Module {
  public:
    explicit MyModule(tbox::main::Context &ctx)
        : tbox::main::Module("my_http", ctx)
        , http_server_(ctx.loop())
    { }

  public:
    //! 指定默认的参数值
    virtual void onFillDefaultConfig(tbox::Json &js) override {
        js["bind_addr"] = "0.0.0.0:12345";
        js["backlog"] = 2;
    }
    virtual bool onInit(const tbox::Json &js) override {
        std::string bind_addr;
        int backlog = 0;
        //! 从js中获取bind_addr与backlog的值
        if (!tbox::util::json::GetField(js, "bind_addr", bind_addr) ||  //! 从js中提取"bind_addr"字段存入bind_addr
            !tbox::util::json::GetField(js, "backlog", backlog))        //! 从js中提取"backlog"字段存入backlog
            return false;

        //! 在初始化中，使用bind_addr与backlog
        if (!http_server_.initialize(tbox::network::SockAddr::FromString(bind_addr), backlog))
            return false;

        http_server_.use(
            [] (tbox::http::server::ContextSptr http_ctx,
                const tbox::http::server::NextFunc &next_func) {
                http_ctx->res().body = R"(<p>Welcome to CppTbox.</br>This is http example.</p>)";
            }
        );
        return true;
    }
    virtual bool onStart() override {
        http_server_.start();
        return true;
    }
    virtual void onStop() override {
        http_server_.stop();
    }

  private:
    tbox::http::server::Server http_server_;
};

namespace tbox {
namespace main {
void RegisterApps(Module &apps, Context &ctx) {
  apps.add(new MyModule(ctx));
}
}
}
