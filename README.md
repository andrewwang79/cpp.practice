# 最佳实践

## 目录结构
| 目录 | 内容 | 说明 |
| :----: | -- | -- |
| crash | 崩溃 |  |
| global | 全局变量引用 |  |
| leak | 内存泄漏 |  |

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
