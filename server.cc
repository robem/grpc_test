#include <fstream>
#include <iostream>
#include <sstream>

#include <grpcpp/grpcpp.h>

#include "test.grpc.pb.h"

using std::shared_ptr;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using grpc::ServerCredentials;
using grpc::SslServerCredentialsOptions;

class TalkToMeImpl : public TalkToMe::Service {
  Status HelloWorld(ServerContext *ctx, const HelloRequest *req,
                    HelloReply *rep) override {
    std::string prefix{"Hello "};
    rep->set_msg(prefix + req->name());
    return Status::OK;
  }
};

shared_ptr<ServerCredentials> GetCredentials() {
  std::stringstream pk, cert;
  std::ifstream pk_file("./server.key");
  pk << pk_file.rdbuf();
  std::ifstream cert_file("./server.cert");
  cert << cert_file.rdbuf();

  SslServerCredentialsOptions::PemKeyCertPair pkcp = {pk.str(), cert.str()};
  SslServerCredentialsOptions ssl_opts(GRPC_SSL_DONT_REQUEST_CLIENT_CERTIFICATE);
  ssl_opts.pem_key_cert_pairs.push_back(pkcp);
  return SslServerCredentials(ssl_opts);
}

void RunServer() {
  std::string server_address("localhost:50051");
  TalkToMeImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, GetCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main() {
  RunServer();
  return 0;
}
