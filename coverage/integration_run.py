import os
import subprocess
import re

# 指定覆盖率文件的目录
coverage_directory = "D:\\桌面\\Test\\ne"

# 使用正则表达式匹配所有以 .cov 结尾的文件
coverage_files = [os.path.join(coverage_directory, f) for f in os.listdir(
    coverage_directory) if re.match(r".+\.cov$", f)]

# 构建 OpenCppCoverage 命令
open_cpp_coverage_command = ["OpenCppCoverage.exe"]
for coverage_file in coverage_files:
    open_cpp_coverage_command.extend(["--input_coverage", coverage_file])
open_cpp_coverage_command.extend(
    ["--export_type=html:D:\\桌面\\Test\\ne\\coverage.html"])

# 执行 OpenCppCoverage 命令
subprocess.call(open_cpp_coverage_command)
