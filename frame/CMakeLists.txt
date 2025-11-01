#所有当前目录下cpp放入SHADER中
file(GLOB_RECURSE FRAME ./ *.cpp)

#编译为lib库
add_library(frame ${FRAME})