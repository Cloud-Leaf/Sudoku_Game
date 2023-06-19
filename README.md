# 数独游戏用户手册

## 1. 程序介绍与概述

### 1.1 程序介绍

该程序是一个数独游戏程序，用于生成数独终局、生成游戏开局、解数独游戏等功能。

### 1.2 安装和配置说明

请使用64-bit Windows 10命令行运行程序。

## 2. 使用指南

### 2.1 运行程序

 1. 请找到sudoku.exe程序所在文件夹，并在此文件夹内运行命令行：

    ![image-20230619222219089](./manual-pic/image-1.png)

 2. 在命令行中输入指令运行程序，具体用法参见下文：

    ![image-20230619222700751](./manual-pic/image-2.png)

### 2.2 参数处理

程序支持以下参数：

- `-c <count>`：生成指定数量（count）的数独终局并保存到文件。
- `-n <count>`：生成指定数量（count）的数独游戏开局。
- `-m <mode>`：设置生成游戏难度模式。
- `-r <range>`：设置生成游戏空格数量（20~55）。
- `-u`：设置生成唯一解的数独游戏开局。
- `-s <filepath>`：从文件加载数独游戏并解局。

### 2.3 生成数独终局

要生成数独终局，请使用以下命令行参数：

```
-c <count>
```

其中，`<count>`为要生成的数独终局数量，范围1~1,000,000。

示例：

```
sudoku -c 10
```

上述命令将生成10个数独终局，并将其保存到文件"Finals.txt"中。

### 2.4 生成数独游戏开局

要生成数独游戏开局，请使用以下命令行参数：

```
-n <count>
```

其中，`<count>`为要生成的数独游戏开局数量，范围1~10,000。

示例：

```
sudoku -n 5
```

上述命令将生成5个数独游戏开局，并将其保存到文件"SudokuGame.txt"中。

### 2.5 生成特定难度的数独游戏开局

要生成特定难度的数独游戏开局，请使用以下命令行参数，注意需要先使用-n参数设定生成的游戏数量：

```
-n <count> -m <mode>
```

其中，`<count>`为要生成的数独游戏开局数量，`<mode>`为游戏难度模式（1-3），默认难度为1。

示例：

```
sudoku -n 5 -m 2
```

上述命令将生成5个中等难度（模式2）的数独游戏开局，并将其保存到文件"SudokuGame.txt"中。

### 2.6 生成特定空格数量的数独游戏开局

要生成特定空格数量的数独游戏开局，请使用以下命令行参数，注意需要先使用-n参数设定生成的游戏数量：

```
-n <count> -r <range>
```

其中，`<count>`为要生成的数独游戏开局数量，`<range>`为空格数量的范围，输入模式：min~max，支持范围：20~55。

示例：

```
sudoku -n 3 -r 22~55
```

上述命令将生成3个空格范围在22至55的数独游戏开局，并将其保存到文件"SudokuGame.txt"中。

### 2.7 生成唯一解的数独游戏开局

要生成唯一解的数独游戏开局，请使用以下命令行参数，注意需要先使用-n参数设定生成的游戏数量：：

```
-n <count> -u
```

其中，`<count>`为要生成的数独游戏开局数量。

示例：

```
sudoku -n 3 -u
```

上述命令将生成3个唯一解的数独游戏开局，并将其保存到文件"SudokuGame.txt"中。

### 2.8 从文件加载数独游戏并求解

要从文件加载数独游戏，请使用以下命令行参数：

```
-s <filepath>
```

其中，`<filepath>`为包含数独游戏的文件路径。

示例：

```
sudoku -s games.txt
```

上述命令将从文件"games.txt"中加载数独游戏，并解局，将解局结果保存至文件"Ans.txt"中。
