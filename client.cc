#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

#include <grpcpp/grpcpp.h>

#include "test.grpc.pb.h"

using namespace std::literals::chrono_literals;

using std::shared_ptr;

using grpc::ChannelCredentials;
using grpc::ClientContext;
using grpc::CreateChannel;
using grpc::SslCredentialsOptions;

shared_ptr<ChannelCredentials> GetCredentials() {
  std::stringstream cert;
  std::ifstream cert_file("./server.cert");
  cert << cert_file.rdbuf();

  SslCredentialsOptions ssl_opts;
  ssl_opts.pem_root_certs = cert.str();
  return grpc::SslCredentials(ssl_opts);
}

int main() {
  grpc::string server_address{"localhost:50051"};
  HelloRequest request;
  request.set_name(std::string{"gRPC noob"});

  auto channel = CreateChannel(server_address, GetCredentials());
  auto stub = TalkToMe::NewStub(channel);

  while (true) {
    ClientContext ctx;
    HelloReply reply;
    auto status = stub->HelloWorld(&ctx, request, &reply);

    if (status.ok()) {
      std::cout << "Server response: " << reply.msg() << std::endl;
    } else {
      std::cerr << "Error: " << status.error_message() << std::endl;
    }
    std::this_thread::sleep_for(1s);
  }
  return 0;
}
