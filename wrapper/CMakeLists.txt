#所有当前目录下cpp放入WRAPPER中
file(GLOB_RECURSE WRAPPER ./ *.cpp)

#编译为lib库
add_library(wrapper ${WRAPPER})