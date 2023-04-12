# 最佳实践
* https://cpp.wangyaqi.cn/

## 目录结构
* 所有代码只在Linux验证通过

| 目录 | 内容 | 说明 |
| - | - | -- |
| moderncpp | 反射机制(DynamicCreate.cpp) <br> 智能指针使用方法 <br> 内存泄漏的场景和示例 <br> | 现代C++特性，基于C++17 <br> 用cmake制作 |
| misc | 全局变量引用, 静态变量 |  |
| libLoad | 动态库动态加载 | 热更新 |
| crash | 崩溃 |  |
| leak | 内存泄漏的场景和示例 <br> 指针使用方法 |  |
| log | 日志log4cplus |  |
| doxygen | doxygen文档 |  |
| gtest | 单元测试 | C++11和cmake3.14，含单元测试覆盖率lcov |
| [ITK](https://medical.wangyaqi.cn/#/graphics/itk) | 图像处理库 <br> ITK指针使用方法 | CMakeSettings.json是在Windows开发和远程调试Linux服务器的CMake程序 |
| libtest | 静态库和动态库的分析验证：链接顺序，依赖的多个库导出了相同符号时的冲突分析 | [参考来源](https://blog.csdn.net/sepnineth/article/details/49456889) |

## moderncpp
```
cd moderncpp
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build -j$((`nproc`+1))
./build/moderncpp
./build/DynamicCreate
```

## misc
```
mkdir -p out
gcc -o out/misc misc/main_support.c misc/main.c
./out/misc
```

## libLoad
* [Linux下C/C++的热更新](https://howardlau.me/programming/c-cpp-hot-reload.html)，[资料2](https://zhuanlan.zhihu.com/p/162366167)

| 目录 | 说明 |
| - | - |
| testLibX | 被依赖的动态库，默认会安装到/usr/ |
| compileDemo | **通过CMake加载当前解决方案**的动态库testLibX <br> 头文件通过相对路径加载 |
| sysDemo | **通过CMake加载系统目录**的动态库testLibX <br> 需先make install安装testLibX到系统目录(头文件，so) <br> CMakeLists默认不编译本项目 |
| dynamicDemo | 基于动态库路径(系统目录)，**通过代码动态加载**动态库testLibX <br> 满足**热更新**，不需要头文件 |

```
cd libLoad
buildDirName=build
cmake -S . -B ${buildDirName} && cmake --build ${buildDirName} -j$((`nproc`+1)) && make install -C ${buildDirName}
./${buildDirName}/dynamicDemo/dynamicDemo
```

## crash
```
mkdir -p out
gcc -g -o out/crash crash/main.c
./out/crash

mkdir -p out
gcc -g -o out/fork_crash crash/fork_main.c
./out/fork_crash
```

## leak
```
gcc -o out/leakc leak/main.c
g++ -g -o out/leakcpp leak/main.cpp
valgrind --tool=memcheck --leak-check=full --show-reachable=yes ./out/leakcpp
valgrind --tool=massif --time-unit=B ./out/leakcpp && ms_print massif.out.30403
```

## log
```
// 获取log4cplus的头文件和lib，以下示例是通过conan
log4cplusPath=/root/.conan/data/log4cplus/2.0.4/Common/stable/package/0ab9fcf606068d4347207cc29edd400ceccbc944/
log4cplusHeaderPath=${log4cplusPath}/include/ && log4cplusLibPath=${log4cplusPath}/lib/release/
prj_path=/root/cpp.practice/log/
cd ${prj_path} && g++ -g -o log main.cpp -I ${log4cplusHeaderPath} -L ${log4cplusLibPath} -llog4cplus
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${log4cplusLibPath} && ./log
```

## doxygen
* 文档编写
```
函数定义参见DogService.h
主页参见main.cpp
新增导航树页面参见article.md
```
* 生成命令
  * cd doxygen && sh gen.sh doxygenDemo 1.0.0.1 ./ ./doc doxyfile.tpl

## gtest
* https://google.github.io/googletest/quickstart-cmake.html

```
# lcov的参数注意事项：路径必须100%匹配，比如多个斜杠就会错误
# 除了build目录和test目录，其他模块内容都会进行分析
# 执行：
安装cmake3.14，gtest放到服务器/root/cpp.practice/gtest/
prj_path=/root/cpp.practice/gtest/
module_name=demo
cd ${prj_path}
rm -rf build && cmake -S . -B build -DENABLE_UT=ON && cmake --build build -j$((`nproc`+1))
# 只有so没有UT，验证使用。rm -rf build && cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build -j$((`nproc`+1))
rm -rf build && cmake -S . -B build && cmake --build build -j$((`nproc`+1))
cd build/${module_name} && ./${module_name}
lcov -d . -t unittest -o ${module_name}.ut.info -b . -c
lcov -r ${module_name}.ut.info -o ${module_name}.ut.info "${prj_path}build/*" "${prj_path}${module_name}/test/*" && lcov -e ${module_name}.ut.info -o ${module_name}.ut.info "${prj_path}${module_name}/*"
genhtml -o result ${module_name}.ut.info
```

## ITK
```
// 用conan安装ITK
prj_path=/root/cpp.practice/itk/
module_name=itk
cd ${prj_path}
rm -rf build && mkdir build && cd build && conan install ../conanfile.txt -s arch=x86_64 -s os=Linux -r cloud --update && cd ..
cmake -S . -B build && cmake --build build -j$((`nproc`+1)) && cd build/ && ./bin/${module_name}
```

## libtest
* 以下提及的符号默认都是导出的(extern)

### 文件说明

| 项 | 说明 |
| - | - |
| test.h | 被测试库的头文件：覆盖test1.c和test2.c |
| test1.c/test2.c | 被测试库：有2个相同符号的静态函数(实现不同)，1个相同符号的静态变量(值不同) |
| main.c | 测试程序 |

### 分析验证结果
* [编译原理](https://cpp.wangyaqi.cn/#/material/compile)

| 测试用例 | 相同符号运行时的使用源 | 说明 |
| - | - | - |
| 1. 程序链接libtest1.so，libtest2.so | 使用先链接的动态库libtest1.so |  |
| 2. 程序链接libtest1.a，libtest2.a | 链接失败 | 错误信息：有多个相同符号存在，无法链接 |
| 3. 程序链接libtest1.a，libtest2.so | 使用静态库libtest1.a | 无视链接顺序 |
| 4. 程序链接libtest2.so，libtest1.a | 使用静态库libtest1.a | 无视链接顺序 |

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
