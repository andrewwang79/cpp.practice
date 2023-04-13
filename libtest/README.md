# README
* 以下提及的符号默认都是导出的(extern)
* [库的设计原理](https://cpp.wangyaqi.cn/#/material/lib)

## 文件说明

| 项 | 说明 |
| - | - |
| test.h | 被测试库的头文件：覆盖test1.c和test2.c |
| test1.c/test2.c | 被测试库：有2个相同符号的静态函数(实现不同)，1个相同符号的静态变量(值不同) |
| main.c | 测试程序 |
| mainX.c/testX.c | 只用于测试用例5 |

## 测试结果
* 相同符号的使用

| 测试用例 | 相同符号运行时的使用源 | 说明 |
| - | - | - |
| 1. 程序链接libtest1.so，libtest2.so | 使用先链接的动态库libtest1.so |  |
| 2. 程序链接libtest1.a，libtest2.a | 链接失败 | 错误信息：有多个相同符号存在，无法链接 |
| 3. 程序链接libtest1.a，libtest2.so | 使用静态库libtest1.a | 无视链接顺序 |
| 4. 程序链接libtest2.so，libtest1.a | 使用静态库libtest1.a | 无视链接顺序 |
| 5. 1个程序链接libtestX.so，libtestX.so链接libtest1.so(含变量)，启动这个程序的2个进程 | 程序和libtestX.o代码，用的是test1.so里的同一个变量 |  |

* 动态库(extern/无extern/通过extern函数)全局变量的进程间共享

| 测试用例 | 全局变量的进程间共享结果 | 说明 |
| - | - | - |
| 10. 1个程序链接libtest1.so，启动这个程序的2个进程 | 不能共享 |  |
| 11. 2个程序链接libtest1.so，分别启动2个程序的各自进程 | 不能共享 |  |
| 12. 1个程序链接libtest1.so，启动这个程序的2个进程 <br> 动态库采用技术[#pragma data_seg("")](https://juejin.cn/post/6969561751385800734) | 不能共享 | 这技术在Windows能有效共享 |

## 脚本
### 编译
```
cd libtest
clear && rm -rf out/*

echo compile test1.c to libtest1.a and libtest1.so
gcc -o out/test1.o -c test1.c && ar -v -q out/libtest1.a out/test1.o
gcc -fPIC -shared test1.c -o out/libtest1.so

echo compile test2.c to libtest2.a and libtest2.so
gcc -o out/test2.o -c test2.c && ar -v -q out/libtest2.a out/test2.o
gcc -fPIC -shared test2.c -o out/libtest2.so

gcc -fPIC -shared testX.c -o out/libtestX.so -L out -ltest1

export LD_LIBRARY_PATH=./out # cp -f out/*.so /usr/lib/
```

### 测试
* 测试用例1
```
echo compile main.c to main1, link libtest1.so first
gcc -o out/main1 main.c -L out -ltest1 -ltest2
ldd out/main1
echo compile main.c to main1, link libtest2.so first
gcc -o out/main2 main.c -L out -ltest2 -ltest1
ldd out/main2

echo execute should use libtest1.so, output is "add=7" and "testVal=11" and "round2 : testInnerVal = 112"
./out/main1
echo execute should use libtest2.so, output is "add=10" and "testVal=22" and "round2 : testInnerVal = 223"
./out/main2
```

* 测试用例2
```
rm out/libtest*.so

echo link fail
gcc -o out/main1 main.c -L out -ltest1 -ltest2
gcc -o out/main2 main.c -L out -ltest2 -ltest1
```

* 测试用例3
```
echo link libtest1.a and libtest2.so
rm out/libtest1.so
gcc -o out/main1 main.c -L out -ltest1 -ltest2
ldd out/main1
echo execute should use libtest1.a, output is "add=7" and "testVal=11" and "round2 : testInnerVal = 112"
./out/main1
```

* 测试用例4
```
echo link libtest1.so and libtest2.a
rm out/libtest2.so
gcc -o out/main1 main.c -L out -ltest1 -ltest2
ldd out/main1
echo execute should use libtest2.a, output is "add=10" and "testVal=22" and "round2 : testInnerVal = 223"
./out/main1
```

* 测试用例5
```
两种编译方法：
gcc -o out/mainX mainX.c -L out -ltest1 -ltestX
gcc -o out/mainX mainX.c -Wl,--copy-dt-needed-entries -L out -ltestX # 关闭显示链接(不推荐)，自动从testX找到test1并load
./out/mainX
```

* 测试用例10
```
gcc -o out/main1 main.c -L out -ltest1
echo output is "add=7" and "testVal=11" and "round2 : testInnerVal = 112"
./out/main1 10

新开终端，10秒内执行
echo output is "add=7" and "testVal=11" and "round2 : testInnerVal = 112"
./out/main1
```

* 测试用例11
```
gcc -o out/main1 main.c -L out -ltest1
gcc -o out/main2 main.c -L out -ltest1
echo output is "add=7" and "testVal=11" and "round2 : testInnerVal = 112"
./out/main1 10

新开终端，10秒内执行
echo output is "add=7" and "testVal=11" and "round2 : testInnerVal = 112"
./out/main2
```

* 测试用例12
```
gcc -o out/main1 main.c -L out -ltest1
echo output is "add=7" and "testOsVal=1027" and "testOsVal... actual value is 1028"
./out/main1 10

新开终端，10秒内执行
echo output is "add=7" and "testOsVal=1027" and "testOsVal... actual value is 1028"
./out/main1
```