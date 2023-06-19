#include<iostream>
#include<vector>
#include<set>

#define FIR_INT 362880
#define GRID_SIZE 9
using namespace std;

static int initial_line[9] = { 1,2,3,4,5,6,7,8,9 };//生成终局用行

class Sudoku {
public:
	vector<vector<int>> grid;//数独盘面
	int shift[8] = {3, 6, 2, 5, 8, 1, 4, 7};

	Sudoku();//默认构造函数
	Sudoku(const Sudoku& sudoku);//拷贝构造
	void Gen_Final(int fline[]);//产生一个终局
	void Gen_Finals(int n);//产生n个终局

};//数独类

struct SudokuCmp {
	bool operator()(const Sudoku& obj1, const Sudoku& obj2) const {
		return obj1.grid < obj2.grid;
	}
};

set<Sudoku, SudokuCmp> Finals;//数独终局集合

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

int main(){
    return 0;
}