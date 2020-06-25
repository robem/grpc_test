CXX=clang++
PROTOC=`which protoc`
CPP_PLUGIN=`which grpc_cpp_plugin`

LD_FLAGS+=`pkg-config --libs protobuf grpc++`
CXX_FLAGS+=-std=c++17

all: client server

client: test.pb.o test.grpc.pb.o client.o
	$(CXX) $^ $(LD_FLAGS) -o $@

server: test.pb.o test.grpc.pb.o server.o
	$(CXX) $^ $(LD_FLAGS) -o $@

%.grpc.pb.cc: %.proto
	$(PROTOC) --proto_path=. --grpc_out=. --plugin=protoc-gen-grpc=$(CPP_PLUGIN) $<

%.pb.cc: %.proto
	$(PROTOC) --proto_path=. --cpp_out=. $<

format:
	clang-format -i `find . -name "*.cc"`
clean:
	rm -f *.o *.pb.cc *.pb.h client server
