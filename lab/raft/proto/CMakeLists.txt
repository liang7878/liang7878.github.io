# 生成 protobuf 代码的编译规则
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS raft.proto)

# 生成 protobuf 代码的库
add_library(raft_proto ${PROTO_SRCS} ${PROTO_HDRS})

# 链接 protobuf 库和依赖项（例如 gRPC）
target_link_libraries(raft_proto PUBLIC protobuf::libprotobuf)
