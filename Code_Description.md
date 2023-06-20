# 代码说明文档

## 1. 程序介绍

该程序实现了一个命令行数独游戏，包括生成数独终局，生成数独游戏，解局等具体功能。

具体实现方法如下：

## 2. 代码解析

### 2.1 头文件引用

```c++
#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <string>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <sstream>
```

程序使用了一些标准库的头文件，包括iostream、vector、set、unordered_set、algorithm、string、fstream和sstream。还引用了ctime和cstdlib头文件，用于处理随机数和时间相关的操作。

### 2.2 宏定义及全局变量

```c++
#define FIR_INT 362880
#define GRID_SIZE 9
static int initial_line[9] = { 1,2,3,4,5,6,7,8,9 };//生成终局用行
int solvenum = 0;
```

程序定义了两个宏常量：FIR_INT和GRID_SIZE。FIR_INT的值为362880，用于限制生成的数独终局的数量，在该数量之后的数独终局生成会采用额外办法。GRID_SIZE的值为9，表示数独的行列数为9。

两个全局变量一个用于生成数独，一个用于记录某一数独游戏有几个解。

### 2.3 数独类

```c++
class Sudoku {
public:
	vector<vector<int>> grid;//数独盘面
	int shift[8] = {3, 6, 2, 5, 8, 1, 4, 7};

	Sudoku();//默认构造函数
	Sudoku(const Sudoku& sudoku);//拷贝构造
	void Gen_Final(int fline[]);//产生一个终局
	void Gen_Finals(int n);//产生n个终局
	void generate_empty(int r1, int r2, int m = 1);//生成一局游戏
	void generate_only_empty(int m = 1);//生成唯一解游戏 
	bool isSafe(int row, int col, int num);//判断num数字在[row,col]处是否合法
	void isonly(int i, int j);//判断是否唯一解
	bool solveSudoku(int i, int j);//针对grid解局
};//数独类
```

程序定义了一个名为Sudoku的类，表示一个数独游戏。该类包含以下成员：

- grid：表示数独盘面的二维向量。
- shift：表示行变换的偏移量数组。

类的成员函数包括：

- 默认构造函数：初始化数独盘面为全0。
- 拷贝构造函数：根据给定的数独对象进行拷贝。
- Gen_Final函数：根据给定的首行数组生成一个数独终局。
- Gen_Finals函数：生成指定数量的数独终局。
- generate_empty函数：在数独盘面上随机生成一局游戏，并挖空指定数量的格子。
- generate_only_empty函数：在数独盘面上随机生成一局只有唯一解的游戏，并挖空指定数量的格子。
- isSafe函数：判断给定位置是否可以填入指定数字。
- isonly函数：判断数独盘面是否只有唯一解。
- solveSudoku函数：使用回溯法解决数独游戏。

### 2.4 关于数独类的全局变量

```c++
struct SudokuCmp {
	bool operator()(const Sudoku& obj1, const Sudoku& obj2) const {
		return obj1.grid < obj2.grid;
	}
};
set<Sudoku, SudokuCmp> Finals;//数独终局集合
vector<Sudoku> SudokuGame;//数独游戏集合
vector<Sudoku> SovledGame;//解局游戏集合
```

使用集合和容器的方式来对数独进行管理：

- Finals：用于存放生成的数独终局，重载类的对比方式SudokuCmp以使用set去重。
- SudokuGame：用于存放生成或读取的数独游戏。
- SovledGame：用于存放针对SudokuGame解局后的数独终局。

### 2.5 控制及辅助函数

```c++
void Para_Handle(int argc, char* argv[]);//输入参数处理
bool InRange(int num, int min, int max);//判断整形num是否在范围内
static void Save2File(const vector<Sudoku>& finals, const string& filepath);//保存终局s至文件对于vector
static void Save2File(const set<Sudoku, SudokuCmp>& finals, const string& filepath);//保存终局s至文件对于set
static int Load2SdkGame(string& filepath);//加载数独游戏
void gen_sudokugame(int r1, int r2, int m = 1, bool u = 0);//生成游戏开局,m为难度（1-3），n判断唯一解（0没有）
void SovleSdkg();//解SudokuGame中所有数独游戏
```

除类函数以外，使用上述函数进行类的管理和输入控制等：

- Para_Handle函数：用于对命令行输入的参数进行解析并进行具体处理，程序在进入main函数后将控制权交予此函数。
- InRange函数：用于判断num参数是否在min~max范围内（包含）。
- Save2File函数：有两个重载，分别接收set与vector，将其内容输出至指定文件。
- Load2SdkGame函数：从输入文件路径中读取数独游戏至SudokuGame容器中，并返回读取的游戏数量。
- gen_sudokugame函数：对Finals集合中的数独终局生成数独游戏，空格范围r1~r2，难度m，是否唯一n。
- SovleSdkg函数：解SudokuGame中所有数独游戏。

## 3. 核心函数解析

### 3.1 生成终局

```c++
void Sudoku::Gen_Finals(int n)
{
	Finals.clear();
	while (Finals.size() < n && Finals.size() < FIR_INT) {
		this->Gen_Final(initial_line);
		Finals.insert(*this);
		next_permutation(initial_line, initial_line + 9);
	}
	while (Finals.size() < n) {
		for (const auto& f : Finals) {
			Sudoku sdk1(f);
			swap(sdk1.grid[1], sdk1.grid[2]);
			Finals.insert(sdk1);
			if (Finals.size() == n) break;
			Sudoku sdk2(f);
			swap(sdk2.grid[4], sdk2.grid[5]);
			Finals.insert(sdk2);
			if (Finals.size() == n) break;
			Sudoku sdk3(f);
			swap(sdk3.grid[7], sdk3.grid[8]);
			Finals.insert(sdk3);
			if (Finals.size() == n) break;
		}
	}
}
void Sudoku::Gen_Final(int fline[])
{
	for (int i = 0; i < GRID_SIZE; i++)this->grid[0][i] = fline[i];
	for (int i = 1; i < GRID_SIZE; i++)
		for (int j = 0; j < GRID_SIZE; j++)
			this->grid[i][j] = this->grid[0][(j + shift[i - 1]) % 9];
	return;
}
```

void Sudoku::Gen_Finals(int n) 方法用于生成多个数独终局。它接受一个整数n作为参数，表示要生成的数独终局的数量。

方法首先清空存储数独终局的集合Finals。然后，使用一个循环来生成n个数独终局。在每次循环中，调用Gen_Final方法生成一个数独终局，每次传入生成终局的首行，首行会在每一次使用后进行变换至下一个字典序更大的排序，然后将数独终局插入到 Finals 集合中，至多以此法生成FIR_INT个终局。

如果Finals集合中的终局数量不足n，则使用另一种方法生成额外的终局。对于每个已生成的终局，通过交换其中的数字行来生成新的终局，并将其插入到Finals集合中，直到达到指定的数量。

最终，该方法会生成指定数量的数独终局，并存储在Finals集合中。

void Sudoku::Gen_Final(int fline[]) 方法用于生成一个数独终局。它接受一个包含9个整数的数组作为参数，表示数独终局的首行。该方法根据给定的首行生成完整的数独终局。

方法首先将给定的首行复制到数独盘面的第一行中。然后通过循环遍历剩余的8行，并根据特定的规则填充每一行的数字。填充规则是根据当前行的索引和一个预先定义的偏移数组shift来确定该行的数字。通过循环和取模运算，可以将首行的数字按照规则填充到后续行中的相应位置。

### 3.2 生成数独游戏

```c++
void Sudoku::generate_empty(int r1, int r2, int m)
{
	srand(time(NULL));
	int emptynum = 0;
	switch (m) {
	case 1: m = 21; break;
	case 2: m = 26; break;
	case 3: m = 30; break;
	default:cout << "难度错误" << endl;
	}
	int random;
	for (int row = 0; row < GRID_SIZE; row++) {
		for (int col = 0; col < GRID_SIZE; col++) {
			random = rand() % 36;
			if (random < m) {
				grid[row][col] = 0;
				emptynum++;
				if (emptynum >= r2)
					return;
			}
		}
	}
	if (emptynum < r1) {
		while (emptynum < r1) {
			for (int row = 0; row < GRID_SIZE; row++) {
				for (int col = 0; col < GRID_SIZE; col++) {
					random = rand() % 2;
					if (grid[row][col] != 0 && random == 0) {
						grid[row][col] = 0;
						emptynum++;
					}
				}
			}
		}
	}
}
```

函数接受三个整形参数，意义分别为：生成空格数量在r1~r2的数独游戏，难度为m（默认为1）。

函数使用随机挖空的方式对数独终局进行处理，从而生成一个数独游戏。

### 3.3 生成唯一解的数独游戏

```c++
void Sudoku::generate_only_empty(int m) {
	srand(time(NULL));
	switch (m) {
	case 1: m = 25; break;
	case 2: m = 27; break;
	case 3: m = 28; break;
	default:cout << "难度错误" << endl;
	}
	vector<vector<int>> empty_grid = grid;
	int random;
	for (int row = 0; row < GRID_SIZE; row++) {
		for (int col = 0; col < GRID_SIZE; col++) {
			random = rand() % 36;
			if (random < m) {
				grid[row][col] = 0;
				solvenum = 0;
				isonly(0, 0);
				if (solvenum == 1) {
					empty_grid[row][col] = 0;
				}
				grid = empty_grid;
			}
		}
	}
	grid = empty_grid;
}
void Sudoku::isonly(int i, int j) {
	if (solvenum >= 2)return;
	int row = i;
	int col = j;
	//bool temp = true;
	for (; row < GRID_SIZE; row++) {
		for (; col < GRID_SIZE; col++) {
			if (grid[row][col] == 0) {
				// 尝试填充1到9的数字
				for (int num = 1; num <= 9; num++) {
					if (solvenum >= 2)return;
					if (isSafe(row, col, num)) {
						// 如果填充的数字安全，则继续递归填充下一个位置
						grid[row][col] = num;
						isonly(row, col);
						grid[row][col] = 0;
					}
				}
				// 如果无法填充任何数字，则返回false
				//if (!temp)
					return;
			}
		}
		col = 0;
	}
	// 如果所有位置都填充了数字，则返回true，表示成功解开了数独
	solvenum++;
	return;
}
```

generate_only_empty函数与生成数独游戏函数类似，均采取随机挖空的方式生成游戏，但在每一次挖空某一格后会对此时的游戏盘面进行判断，若此时的解唯一则成功挖空这一格，否则此格不能被挖去。

isonly函数与数独解局函数类似，只在解局函数基础上加入对solvenum的处理和判断，这个变量用于记录解的个数，若其大于1，则终止函数的递归。

### 3.4 解数独游戏

```c++
bool Sudoku::solveSudoku(int i, int j)
{
	int row = i;
	int col = j;
	for (; row < GRID_SIZE; row++) {
		for (; col < GRID_SIZE; col++) {
			if (grid[row][col] == 0) {
				// 尝试填充1到9的数字
				for (int num = 1; num <= 9; num++) {
					if (isSafe(row, col, num)) {
						// 如果填充的数字安全，则继续递归填充下一个位置
						grid[row][col] = num;
						if (solveSudoku(row, col))
							return true;
						grid[row][col] = 0;
					}
				}
				// 如果无法填充任何数字，则返回false
				return false;
			}
		}
		col = 0;
	}
	return true;
}
```

函数采用递归的方式对一个数独盘面进行求解。

该函数接受两个整数参数i和j，表示当前要填充的格子的行和列索引。

函数的主要逻辑如下：

1. 初始化局部变量row和col，分别赋值为i和j。
2. 通过两个嵌套的循环，遍历数独谜题的每个格子，从位置(row, col)开始。
3. 如果当前位置的格子是空白格子（值为0），则执行以下操作：
   - 使用一个循环尝试填充数字1到9。
   - 对于每个数字num，检查是否可以将num放置在当前位置，调用isSafe函数进行检查。
   - 如果可以安全地将num放置在当前位置，则将当前位置的值设置为num，并递归调用solveSudoku函数来填充下一个格子。
   - 如果递归调用solveSudoku返回true（表示成功解开数独），则返回true。
   - 如果递归调用solveSudoku返回false（表示无法解开数独），则将当前位置的值重新设置为0，尝试下一个数字。
4. 如果所有数字都尝试过了仍然无法填充任何数字，则返回false，表示无法解开数独。
5. 当完成所有格子的遍历时，返回true，表示成功解开了数独。

注：此函数仅可对于游戏盘面生成一个解，若需要多个解，可参考上一小节中isonly函数的实现方法对函数进行微调。