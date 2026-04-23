# 井字棋（TicTacToe）

这是一个基于 **C++ + EasyX** 的图形化井字棋小游戏，使用 Visual Studio 2022 构建。项目已按“状态逻辑、渲染、流程控制、界面调度”拆分，适合作为 C++ 桌面小项目、EasyX 绘图练习和 Minimax 搜索示例。

## 功能概览

- 支持双人对战（PvP）
- 支持人机对战（随机 AI / Minimax AI）
- 自动判定 X 胜、O 胜和平局
- 对局结束后可选择再来一局或返回模式选择页
- 图形化棋盘、棋子、按钮和页面背景

## 技术栈

- C++17
- EasyX 图形库（`graphics.h`）
- Visual Studio 2022 / MSBuild v143
- Visual Studio C++ 测试项目（不依赖 EasyX，只测试 `GameState`）

## 目录结构

```text
Tictactoe/
├─ README.md
├─ Tictactoe.sln
├─ tests/
│  ├─ GameStateTests.cpp          # GameState 纯逻辑回归测试
│  ├─ TictactoeTests.vcxproj      # VS 测试项目
│  └─ TictactoeTests.vcxproj.filters
├─ Tictactoe/
│  ├─ Tictactoe.vcxproj           # VS C++ 项目文件
│  ├─ include/
│  │  ├─ Button.h                 # 按钮命中检测、绘制和点击回调
│  │  ├─ Game.h                   # 棋盘状态、胜负判断和 Minimax AI
│  │  ├─ GameFlow.h               # 对局结束后的弹窗与页面动作
│  │  ├─ GameRenderer.h           # EasyX 棋盘/棋子/提示文字渲染
│  │  └─ Widget.h                 # 页面管理、事件分发和模式循环
│  ├─ cpp/
│  │  ├─ Button.cpp
│  │  ├─ Game.cpp
│  │  ├─ GameFlow.cpp
│  │  ├─ GameRenderer.cpp
│  │  ├─ Widget.cpp
│  │  └─ main.cpp                 # 程序入口
│  └─ src/                        # 棋盘、棋子、背景等图片资源
├─ Tictactoe_Setup/               # 安装项目
└─ x64/                           # 构建输出目录
```

## 核心模块

### `GameState`

`GameState` 是纯逻辑层，不直接依赖 EasyX。它负责维护 3x3 棋盘、当前玩家、运行状态、落子校验、胜负判断和平局判断，并提供 Minimax + Alpha-Beta 剪枝搜索。现在它会拒绝终局后的额外落子，避免事件队列里残留点击继续修改棋盘。

### `GameRenderer`

`GameRenderer` 只读取 `GameState` 的状态并绘制画面。棋盘和棋子图片由 `std::unique_ptr<IMAGE>` 管理，避免手动 `new/delete`；透明棋子绘制会复制图片后处理白色背景，不会污染已缓存的原始资源。

### `GameFlowController`

`GameFlowController` 专门处理对局结束后的流程：如果还未终局就继续循环；如果 X/O 获胜或平局，就弹出确认框，根据玩家选择返回“重开”或“返回模式选择页”的动作。

### `Widget` 和 `Button`

`Widget` 负责页面切换、鼠标事件分发和三种游戏模式的循环驱动；`Button` 封装按钮的命中检测、悬停缩放、绘制和点击回调。这样界面调度和游戏规则不会混在同一个类里。

## 运行流程

1. `main.cpp` 创建 `Widget(600, 600)`。
2. `Widget::init()` 初始化 EasyX 窗口、页面图片、按钮和渲染资源。
3. `Widget::run()` 进入主循环，持续处理鼠标移动和点击。
4. 玩家选择 PvP、随机 AI 或 Minimax AI 后，`Widget` 进入对应对局循环。
5. 每帧按顺序处理输入、AI 落子、渲染、终局流程和帧率限制。
6. 对局结束后，玩家可以继续下一局，也可以返回模式选择页。

## 构建与运行

### 使用 Visual Studio 2022

1. 安装 EasyX，并确认 `graphics.h` 可以被 Visual Studio C++ 项目找到。
2. 打开 `Tictactoe.sln`。
3. 选择 `Debug|x64` 或 `Release|x64`。
4. 生成并运行 `Tictactoe`。

### 使用命令行

如果 `msbuild` 已加入 PATH：

```powershell
msbuild Tictactoe.sln /t:Build /p:Configuration=Debug /p:Platform=x64
```

如果未加入 PATH，可以使用 Visual Studio Installer 的 `vswhere.exe` 查找：

```powershell
& 'C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe' `
  -latest -products * -requires Microsoft.Component.MSBuild `
  -find MSBuild\**\Bin\MSBuild.exe
```

然后用查到的 `MSBuild.exe` 路径构建，例如：

```powershell
& 'D:\vs\MSBuild\Current\Bin\MSBuild.exe' `
  Tictactoe.sln /t:Build /p:Configuration=Debug /p:Platform=x64
```

构建成功后，可执行文件通常位于 `x64/Debug/Tictactoe.exe` 或 `x64/Release/Tictactoe.exe`。

> 说明：`Tictactoe_Setup.vdproj` 是安装项目，MSBuild 不支持直接构建 `.vdproj`。当前解决方案已取消安装项目在 `Release|x64` 下的 Build 勾选，因此命令行 Release 构建会构建主程序和测试项目，不会被安装项目阻断。

## 逻辑测试

仓库包含一个正式的 Visual Studio C++ 测试项目 `TictactoeTests`，只链接 `Game.cpp`，不依赖 EasyX。构建解决方案后运行测试可执行文件：

```powershell
& 'D:\vs\MSBuild\Current\Bin\MSBuild.exe' `
  Tictactoe.sln /t:Build /p:Configuration=Debug /p:Platform=x64

.\x64\Debug\Tests\TictactoeTests.exe
```

当前测试覆盖非法落子、先手/后手胜负判断、平局判断、终局后拒绝继续落子，以及 Minimax AI 的立即取胜和立即防守决策。

## 维护建议

- 新增规则或 AI 行为时，优先在 `GameState` 中写纯逻辑，再让 `Widget` 调用。
- 新增页面或按钮时，优先修改 `Widget::init()` 和 `Button`，不要把页面跳转写进规则层。
- 新增绘制资源时，放在 `Tictactoe/src/`，并在 `GameRenderer::loadResources()` 中集中加载。
- 源码建议统一为 UTF-8，避免 Visual Studio 出现 C4819 编码警告。

## 许可与用途

本项目用于学习、课程练习和交流演示。
