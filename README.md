# 最佳实践
* https://cpp.wangyaqi.cn/

## 目录结构
| 目录 | 内容 | 说明 |
| :----: | -- | -- |
| moderncpp | 现代C++特性，DynamicCreate.cpp：反射机制 |  |
| global | 全局变量引用 |  |
| libLoad | lib动态加载 | 含cmake install |
| crash | 崩溃 |  |
| leak | 内存泄漏 |  |
| log | 日志log4cplus |  |
| doxygen | doxygen文档 |  |
| gtest | 单元测试 | C++11和cmake3.14，含单元测试覆盖率lcov |
| [ITK](https://medical.wangyaqi.cn/#/graphics/itk) | 图像处理库 | 在Windows开发和远程调试Linux服务器的CMake程序 |
| libtest | 静态库动态库调用验证 | 模拟不同链接顺序，的情况 |

## moderncpp
```
mkdir -p out
g++ -g -o out/moderncpp moderncpp/main.cpp
./out/moderncpp
```

## global
```
mkdir -p out
gcc -o out/global global/main_support.c global/main.c
./out/global
```

## libLoad
* Linux C/C++ 实现热更新：https://howardlau.me/programming/c-cpp-hot-reload.html，https://zhuanlan.zhihu.com/p/162366167
*
| 目录 | 说明 |
| :----: | -- |
| testLibX | 测试库，安装到/usr/ |
| compileDemo | demo加载同1个解决方案下的so |
| sysDemo | demo动态加载系统级的so。testLibX需先cmake install才可编译，默认关闭本项目 |
| dynamicDemo | demo动态加载so，可满足热更新。不需要头文件，要指定so的路径。 |

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
cmake -S . -B build && cmake --build build && cd build/ && ./bin/${module_name}
```

## libtest
```
clear
rm -rf out/*

echo compile
gcc -o out/test1.o -c test1.c
ar -v -q out/libtest1.a out/test1.o
gcc -fPIC -shared test1.c -o out/libtest1.so

gcc -o out/test2.o -c test2.c
ar -v -q out/libtest2.a out/test2.o
gcc -fPIC -shared test2.c -o out/libtest2.so
 
echo 把不同版本的库按照不同顺序链接，优先链接动态库
gcc -o out/main1 main.c -L bin -ltest1 -ltest2
gcc -o out/main2 main.c -L bin -ltest2 -ltest1

cp -f out/*.so /usr/lib
echo 观察输出结果：，main1输出"sum=7"，main2输出"sum=10"
./out/main1
./out/main2
```