#include <grpcpp/grpcpp.h>

#include "test.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class TalkToMeImpl : public TalkToMe::Service {
  Status HelloWorld(ServerContext *ctx, const HelloRequest *req,
                    HelloReply *rep) override {
    std::string prefix{"Hello "};
    rep->set_msg(prefix + req->name());
    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  TalkToMeImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main() {
  RunServer();
  return 0;
}
