# Repository Guidelines

## 项目结构与模块组织
- 核心源码位于 `src/`，其中 `main.cpp` 为程序入口，`glad.c` 集成 OpenGL 加载逻辑；新增模块或源文件请置于此目录并同步 `CMakeLists.txt`。
- 自研头文件集中在 `header/`，按功能拆分子目录；引用时使用 `#include "header/..."`，保持目录整洁并避免循环依赖。
- 第三方头文件统一存放于仓库根目录的 `include/`，对应静态/动态库位于 `lib/`；替换新版本时请确认架构与编译器匹配。
- 构建生成物输出到 `build/`，不要将 IDE 自动生成的缓存或日志提交到版本库。

## 构建与开发命令
- 统一执行 `build.bat [Debug|Release|RelWithDebInfo|MinSizeRel]`；脚本会自动调用 CMake 配置、编译并运行 `ctest`。
- 首次配置可手动运行 `cmake -S . -B build -G "Visual Studio 17 2022"` 指定生成器，后续直接使用脚本即可。
- 可执行文件位于 `build/Debug/OpenGL_Window.exe`（多配置）或 `build/OpenGL_Window.exe`（单配置）；如遇窗口创建失败，检查显卡驱动及 GLFW 依赖是否完整。
- 如需清理构建缓存，删除 `build/` 或扩展脚本 `build.bat clean`（待实现），然后重新执行脚本。

## 编码风格与命名约定
- 采用 C++17、UTF-8 编码与四空格缩进，不混用 Tab。
- 源文件命名使用小写下划线，如 `renderer.cpp`；头文件使用 `.hpp` 并放入 `header/`。
- 类型命名使用 PascalCase，函数与变量采用 camelCase，常量使用大写下划线；命名应体现用途，避免缩写。
- 提交前运行 `clang-format`（若已配置）或 IDE 自带格式化功能，确保跨平台风格一致。

## 验证与测试
- 当前未配置自动化测试，构建脚本调用 `ctest` 主要用于占位；未来引入测试框架时，请将测试文件置于 `tests/` 并在 `CMakeLists.txt` 注册目标。
- 每次改动至少在 Debug 与 Release 配置下运行程序，确认窗口生命周期、输入响应及 OpenGL 日志无异常。
- 若渲染效果有变更，请在 PR 或提交说明中附截图或录屏以便审查。

## 提交与拉取请求指南
- 遵循 Conventional Commits，例如 `feat: 引入摄像机控制`、`fix: 修正视口更新`。
- 提交前执行 `build.bat Debug` 或 `build.bat Release`，确保代码可构建并清理未跟踪生成物。
- PR 描述需包含背景、核心改动、验证方式与潜在风险；涉及视觉效果时请附说明材料并关联 Issue。
- 经评审通过后再合并，若存在后续事项，请在 PR 中列出 TODO 以便追踪。
