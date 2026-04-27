# Tile Shift 2048

Tile Shift 2048 是一个用 C++ 实现的 2048 小游戏。玩家通过方向键移动数字方块，相同数字合并，目标是在 4x4 棋盘上合成 2048。

这个项目是一个练手性质的小游戏，界面使用 Dear ImGui 绘制，窗口和输入由 GLFW 处理，构建方式已经整理为 CMake。

## 功能

- 4x4 2048 棋盘
- 方向键控制方块滑动
- 相同数字合并
- 每次有效移动后随机生成新方块
- 分数显示
- 胜利和失败判断
- 一键重新开始

## 操作

| 按键/按钮 | 功能 |
| --- | --- |
| `Up` | 向上移动 |
| `Down` | 向下移动 |
| `Left` | 向左移动 |
| `Right` | 向右移动 |
| `restart` | 重新开始 |

## 项目结构

```text
tile-shift-2048/
├── CMakeLists.txt
├── README.md
├── src/
│   └── main.cpp
└── external/
    ├── glfw/
    └── imgui/
```

## 构建环境

当前 CMake 配置面向 Windows，支持 MinGW 或 MSVC。项目内已经带有 GLFW 和 Dear ImGui 相关文件，系统侧需要准备：

- CMake 3.20 或更高版本
- Ninja，或其他 CMake 生成器
- MinGW-w64 或 Visual Studio C++ 编译环境
- 支持 OpenGL 3.3 的显卡/驱动

## 编译运行

使用 MinGW + Ninja：

```powershell
cmake -S . -B build-mingw -G Ninja
cmake --build build-mingw
.\build-mingw\tile-shift-2048.exe
```

如果 CMake 默认找到了不合适的编译器，可以显式指定 MinGW：

```powershell
cmake -S . -B build-mingw -G Ninja `
  -DCMAKE_CXX_COMPILER="C:/path/to/mingw64/bin/g++.exe"
cmake --build build-mingw
.\build-mingw\tile-shift-2048.exe
```

CMake 构建后会把 `glfw3.dll` 自动复制到输出目录。直接运行 exe 时，请确保 exe 旁边存在该 DLL。

## 规则

- 初始棋盘为空，第一次按方向键后生成初始方块。
- 每次有效移动后会随机生成 1 到 2 个新方块。
- 两个相同数字相撞时会合并。
- 出现 `2048` 即胜利。
- 棋盘已满且四个方向都无法移动时失败。

## 说明

这是一个学习性质的小项目，核心逻辑集中在 `board` 类中，包括棋盘状态、移动合并、随机生成、胜负判断和 ImGui 显示。后续可以把游戏逻辑和界面渲染拆开，方便测试和维护。
