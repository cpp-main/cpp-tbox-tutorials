#include <tbox/main/module.h>
#include <tbox/base/log.h>
#include <tbox/http/server/server.h>
#include <tbox/terminal/terminal_nodes.h>

class MyModule : public tbox::main::Module {
  public:
    explicit MyModule(tbox::main::Context &ctx)
        : tbox::main::Module("my_http", ctx)
        , http_server_(ctx.loop())
    { }

  public:
    virtual bool onInit(const tbox::Json &js) override {
        initShell();

        if (!http_server_.initialize(tbox::network::SockAddr::FromString("0.0.0.0:12345"), 2))
            return false;

        http_server_.use(
            [this] (tbox::http::server::ContextSptr http_ctx,
                const tbox::http::server::NextFunc &next_func) {
                http_ctx->res().body = content_;
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

  protected:
    void initShell() {
        using namespace tbox::terminal;
        auto shell = ctx().terminal();
        auto dir_node = shell->createDirNode();
        shell->mountNode(shell->findNode("/"), dir_node, "my_http");

        {
            auto func_node = shell->createFuncNode(
                [this] (const Session &s, const Args &) {
                    s.send(content_ + "\r\n");
                },
                "print content_ value"
            );
            shell->mountNode(dir_node, func_node, "print_content");
        }
        {
            auto func_node = shell->createFuncNode(
                [this] (const Session &s, const Args &a) {
                    if (a.size() != 2) {
                        s.send("Usage: " + a[0] + " 'new_content'\r\n");
                    } else {
                        content_ = a[1];
                        s.send("done\r\n");
                    }
                },
                "set content_ value"
            );
            shell->mountNode(dir_node, func_node, "set_content");
        }
    }

  private:
    tbox::http::server::Server http_server_;
    std::string content_ = R"(<p>hello terminal</p>)";
};

namespace tbox {
namespace main {
void RegisterApps(Module &apps, Context &ctx) {
  apps.add(new MyModule(ctx));
}
}
}
