#include <chrono>
#include <iostream>
#include <thread>

#include <grpcpp/grpcpp.h>

#include "test.grpc.pb.h"

using namespace std::literals::chrono_literals;

using grpc::ClientContext;
using grpc::CreateChannel;
using grpc::InsecureChannelCredentials;

int main() {
  grpc::string server_address{"localhost:50051"};
  HelloRequest request;
  request.set_name(std::string{"gRPC noob"});

  auto channel = CreateChannel(server_address, InsecureChannelCredentials());
  auto stub = TalkToMe::NewStub(channel);

  while (true) {
    ClientContext ctx;
    HelloReply reply;
    auto status = stub->HelloWorld(&ctx, request, &reply);

    if (status.ok()) {
      std::cout << "Server response: " << reply.msg() << std::endl;
    } else {
      std::cerr << "Ooops. Something went wrong" << std::endl;
    }
    std::this_thread::sleep_for(1s);
  }
  return 0;
}
