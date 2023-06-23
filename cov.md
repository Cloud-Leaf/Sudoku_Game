# 程序覆盖率测试

我们编写了23个用例来对我们的程序进行测试，为了尽可能多的覆盖我们的代码，样例中不仅有正确输入的参数，还包含了一些非正确参数。

我们使用OpenCppCoverage工具来进行覆盖率测试，最终我们选取的测试输入为：

%EXECUTABLE%为测试程序

```
%EXECUTABLE% 
%EXECUTABLE% -c 100
%EXECUTABLE% -c 0
%EXECUTABLE% -c 0 -n
%EXECUTABLE% -s nofile
%EXECUTABLE% -s SudokuGame.txt
%EXECUTABLE% -s nofile 0
%EXECUTABLE% -n
%EXECUTABLE% -n 0
%EXECUTABLE% -n 10
%EXECUTABLE% -n 10 -u
%EXECUTABLE% -n 10 -s
%EXECUTABLE% -n 10 -m 2
%EXECUTABLE% -n 10 -m 5
%EXECUTABLE% -n 10 -r 20~55
%EXECUTABLE% -n 10 -r 0~81
%EXECUTABLE% -n -s
%EXECUTABLE% -n 10 -m 3
%EXECUTABLE% -a
%EXECUTABLE% -n 10 -m 1 -u
%EXECUTABLE% -n 10 -r 50~55
%EXECUTABLE% -c 363000
%EXECUTABLE% -n 100 -r 54~55
```

具体的测试参数参见仓库中/text-example.txt

仓库中的/coverage文件夹内容为测试覆盖率后输出结果，包括一些html，可以看到具体程序的覆盖率和具体覆盖了哪些代码。

\coverage\coverage.html\index.html：

![image-20230622124015350](cov-pic\image-1.png)

最终在这些测试样例下，我们的测试样例具有94%的代码覆盖率。

\coverage\coverage.html\Modules\Sudoku\Sudoku.cpp.html：

可以看到具体覆盖了哪些代码。

可以发现，一些没有被覆盖的代码是关于文件的打开判断，这部分属于意外处理内容。

还有一些是在函数return后的'}'：

![image-20230622124647036](cov-pic\image-2.png)

这可能是因为我们使用的这个OpenCppCoverage工具对具体的统计有一些小问题，例如一行代码：

```
}
```

这种情况不应该被计算为未覆盖。

在排除上述情况下，预估代码覆盖率应该具有99%以上。