# 最佳实践

## 目录结构
| 目录 | 内容 | 说明 |
| :----: | -- | -- |
| crash | 崩溃 |  |
| global | 全局变量引用 |  |
| leak | 内存泄漏 |  |
| doxygen | doxygen文档 |  |
| gtest | 单元测试 | C++11和cmake3.14 |

## 编译
```
mkdir -p out

gcc -o out/crash crash/main.c

gcc -o out/global global/main_support.c global/main.c

gcc -o out/leakc leak/main.c
g++ -g -o out/leakcpp leak/main.cpp
valgrind --tool=memcheck --leak-check=full --show-reachable=yes ./out/leakcpp
valgrind --tool=massif --time-unit=B ./out/leakcpp && ms_print massif.out.30403
```

## 文档生成
* 生成命令：cd path && doxygen Doxyfile

## 单元测试
* https://google.github.io/googletest/quickstart-cmake.html

```
# lcov的路径必须100%匹配，比如不能多个斜杠
# 除了build目录和test目录，其他模块内容都进行分析
prj_path=/root/cpp.practice/gtest/
module_name=demo
cd ${prj_path}
rm -rf build && cmake -S . -B build && cmake --build build && cd build/${module_name} && ./${module_name}
lcov -d . -t unittest -o ${module_name}.ut.info -b . -c
lcov -r ${module_name}.ut.info -o ${module_name}.ut.info "${prj_path}build/*" "${prj_path}${module_name}/test/*" && lcov -e ${module_name}.ut.info -o ${module_name}.ut.info "${prj_path}${module_name}/*"
genhtml -o result ${module_name}.ut.info
```
