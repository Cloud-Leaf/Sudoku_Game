#include<iostream>
#include<vector>
#include<set>
#include<unordered_set>
#include<algorithm>
#include<string>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <sstream>

#define FIR_INT 362880
#define GRID_SIZE 9

using namespace std;

static int initial_line[9] = { 1,2,3,4,5,6,7,8,9 };//生成终局用行
int solvenum = 0;

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

struct SudokuCmp {
	bool operator()(const Sudoku& obj1, const Sudoku& obj2) const {
		return obj1.grid < obj2.grid;
	}
};

set<Sudoku, SudokuCmp> Finals;//数独终局集合

vector<Sudoku> SudokuGame;//数独游戏集合

vector<Sudoku> SovledGame;//解局游戏集合

void Para_Handle(int argc, char* argv[]);//输入参数处理

bool InRange(int num, int min, int max);//判断整形num是否在范围内

static void Save2File(const vector<Sudoku>& finals, const string& filepath);//保存终局s至文件对于vector

static void Save2File(const set<Sudoku, SudokuCmp>& finals, const string& filepath);//保存终局s至文件对于set

static int Load2SdkGame(string& filepath);//加载数独游戏

void gen_sudokugame(int r1, int r2, int m = 1, bool u = 0);//生成游戏开局,m为难度（1-3），n判断唯一解（0没有）

void SovleSdkg();//解SudokuGame中所有数独游戏

//默认构造
Sudoku::Sudoku() {
	grid.resize(GRID_SIZE, vector<int>(GRID_SIZE));
	for (int i = 0; i < GRID_SIZE; i++)
		for (int j = 0; j < GRID_SIZE; j++)
			grid[i][j] = 0;
}

//拷贝构造
Sudoku::Sudoku(const Sudoku& sudoku)
{
	grid.resize(GRID_SIZE, vector<int>(GRID_SIZE));
	for (int i = 0; i < GRID_SIZE; i++)
		for (int j = 0; j < GRID_SIZE; j++)
			this->grid[i][j] = sudoku.grid[i][j];
}

void Sudoku::Gen_Final(int fline[])
{
	//首行
	for (int i = 0; i < GRID_SIZE; i++)this->grid[0][i] = fline[i];
	//其余
	for (int i = 1; i < GRID_SIZE; i++)
		for (int j = 0; j < GRID_SIZE; j++)
			this->grid[i][j] = this->grid[0][(j + shift[i - 1]) % 9];
	return;
}

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

bool Sudoku::isSafe(int row, int col, int num)
{
	// 检查行中是否有重复数字
	for (int i = 0; i < GRID_SIZE; i++) {
		if (grid[row][i] == num) {
			return false;
		}
	}
	// 检查列中是否有重复数字
	for (int i = 0; i < GRID_SIZE; i++) {
		if (grid[i][col] == num) {
			return false;
		}
	}
	// 检查单元格中是否有重复数字
	int startRow = row - row % 3;
	int startCol = col - col % 3;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (grid[i + startRow][j + startCol] == num) {
				return false;
			}
		}
	}
	return true;
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
						//solveSudoku();
						grid[row][col] = 0;

					}
				}
				// 如果无法填充任何数字，则返回false
				return false;
			}
		}
		col = 0;
	}
	// 如果所有位置都填充了数字，则返回true，表示成功解开了数独
	//SovledGame.push_back(*this);
	return true;
}

void gen_sudokugame(int r1, int r2, int m, bool u)
{
	if(!u)
		for (auto f : Finals) {
			f.generate_empty(r1, r2);
			SudokuGame.push_back(f);
		}
	else {
		for (auto f : Finals) {
			f.generate_only_empty(m);
			SudokuGame.push_back(f);
		}	
	}
}

void SovleSdkg()
{
	for (auto g : SudokuGame) {
		g.solveSudoku(0, 0);
		SovledGame.push_back(g);
	}
}

bool InRange(int num, int min, int max) {
	return num >= min && num <= max;
}

static void Save2File(const vector<Sudoku>& finals, const string& filepath) {
	ofstream file(filepath);//覆盖写模式
	int count = 1;
	if (file.is_open()) {
		for (const auto& f : finals) {
			file << "盘面" << count << endl;
			for (const auto& r : f.grid) {
				for (int v : r) {
					if (v == 0)
						file << "$" << " ";
					else file << v << " ";
				}
				file << endl;
			}
			count++;
		}
		file.close();
		cout << "已将" << count - 1 << "个终局/游戏写入文件" << filepath << endl;
	}
	else {
		cerr << "打开文件失败！" << filepath << endl;
	}
}

static void Save2File(const set<Sudoku, SudokuCmp>& finals, const string& filepath) {
	ofstream file(filepath);//覆盖写模式
	int count = 1;
	if (file.is_open()) {
		for (const auto& f : finals) {
			file << "终局" << count << endl;
			for (const auto& r : f.grid) {
				for (int v : r) {
					if (v == 0)
						file << "$" << " ";
					else file << v << " ";
				}
				file << endl;
			}
			count++;
		}
		file.close();
		cout << "已将" << count - 1 << "个终局/游戏写入文件" << filepath << endl;
	}
	else {
		cerr << "打开文件失败！" << filepath << endl;
	}
}

int Load2SdkGame(string& filepath)
{
	ifstream file(filepath);
	if (!file.is_open()) { cout << "请输入正确的文件路径！" << endl; return -1; }
	string line;
	int index = 0;
	Sudoku sdk;
	while (getline(file, line)) {
		if (index % 10) {
			for (int i = 0; i < GRID_SIZE; i++) {
				if (line[i * 2] == '$')
					sdk.grid[index % 10 - 1][i] = 0;
				else
					sdk.grid[index % 10 - 1][i] = (line[i * 2] - '0');
			}
		}
		if (index % 10 == 9) {
			SudokuGame.push_back(sdk);
		}
		index++;
	}
	cout << "读取文件成功" << endl;
	return index / 10;
}

void Para_Handle(int argc, char* argv[]) {
	int c(0);//终局个数
	int n(0);//游戏数量
	int m(1);//游戏难度
	int min(0);//挖空最小值
	int max(81);//挖空最大值
	string filepath;
	Sudoku sdk;

	if (argc == 1) { cout << "请输入参数，具体使用方法参见用户手册" << endl; return; }

	string fir_arg = argv[1];//获取第一个参数
	if (fir_arg == "-c") {
		//生成游戏终局
		if (argc == 3) {
			c = stoi(argv[2]);
			if (InRange(c, 1, 1000000)) {
				sdk.Gen_Finals(c);
				Save2File(Finals, "Finals.txt");
				return;
			}
			else { cout << "请正确输入要生成的终局数量，范围1~100w" << endl; return; }
		}
		else { cout << "请正确输入参数-c <count>，具体使用方法参见用户手册" << endl; return; }
	}
	else if (fir_arg == "-s") {
		//根据文件解局
		if (argc == 3) {
			filepath = argv[2];
			n = Load2SdkGame(filepath);
			if (n == -1)return;//错误文件路径
			else {
				SovleSdkg();
				Save2File(SovledGame, "Ans.txt");
			}
		}
		else { cout << "请正确输入参数-s <filepath>，具体使用方法参见用户手册" << endl; return; }
	}
	else if (fir_arg == "-n") {
		//生成游戏
		if (argc < 3) { cout << "请正确输入参数-n <count>，具体使用方法参见用户手册" << endl; return; }
		n = stoi(argv[2]);
		if (!InRange(n, 1, 10000)) { cout << "请正确输入要生成的终局数量，范围1~1w" << endl; return; }
		if (argc == 3) {
			sdk.Gen_Finals(n);
			gen_sudokugame(min, max);
			Save2File(SudokuGame, "SudokuGame.txt");
			return;
		}
		else if (argc == 4) {
			string sec_arg = argv[3];
			if (sec_arg == "-u") {
				sdk.Gen_Finals(n);
				gen_sudokugame(min, max, m, true);
				Save2File(SudokuGame, "SudokuGame.txt");
				return;
			}
			else { cout << "请正确输入参数，具体使用方法参见用户手册" << endl; return; }
		}
		else if (argc == 5) {
			string sec_arg = argv[3];
			if (sec_arg == "-m") {
				m = stoi(argv[4]);
				if (InRange(m, 1, 3)) {
					sdk.Gen_Finals(n);
					gen_sudokugame(min, max, m);
					Save2File(SudokuGame, "SudokuGame.txt");
					return;
				}
				else { cout << "请正确输入参数-m <mode>，具体使用方法参见用户手册" << endl; return; }
			}
			else if (sec_arg == "-r") {
				istringstream range(argv[4]);
				string rmin, rmax;
				getline(range, rmin, '~');
				getline(range, rmax, '~');
				min = stoi(rmin);
				max = stoi(rmax);
				if (min > max|| !InRange(max, 20, 55)|| !InRange(min, 20, 55)) { cout << "请正确输入参数-r <range>，具体使用方法参见用户手册" << endl; return; }
				sdk.Gen_Finals(n);
				gen_sudokugame(min, max);
				Save2File(SudokuGame, "SudokuGame.txt");
				return;
			}
		}
		/*else if (argc == 7) {
			{ cout << "请正确输入参数" << endl; return; }
		}*/
		else { cout << "请正确输入参数-n <count>，具体使用方法参见用户手册" << endl; return; }
	}
	else { cout << "请正确输入参数，具体使用方法参见用户手册" << endl; return; }
}

int main(int argc, char* argv[]) {
	Para_Handle(argc, argv);
	return 0;
}