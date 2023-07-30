cmake_minimum_required(VERSION 3.12)
project(raft_server)

# 设置 C++标准
set(CMAKE_CXX_STANDARD 20)

# 添加 gRPC 的依赖
find_package(gRPC REQUIRED)
find_package(Protobuf REQUIRED)

# 添加生成的 protobuf 文件
add_subdirectory(proto)

# 添加服务器源文件
add_executable(raft_server
    src/server/raft_server.cpp
    src/server/server_main.cpp
)

# 链接 gRPC 库和生成的 protobuf 库
target_link_libraries(raft_server
    gRPC::grpc++
    raft_proto
)

# set_output_directory(MyExecutable ${CMAKE_BINARY_DIR}/bin)