# 最佳实践
* https://cpp.wangyaqi.cn/

## 目录结构
* 所有代码只在Linux验证通过

| 目录 | 内容 | 说明 |
| - | - | - |
| moderncpp | 现代C++特性，基于C++17 <br> 用cmake制作 |  |
| misc | 全局变量引用, 静态变量 |  |
| libLoad | 动态库动态加载 | 热更新 |
| [libtest](libtest/README.md) | 静态库和动态库的分析验证：链接顺序，依赖的多个库导出了相同符号时的冲突分析 | [参考来源](https://blog.csdn.net/sepnineth/article/details/49456889) |
| libDebug | dll在其他项目里能调试 | 当前是Windows |
| crash | 崩溃 |  |
| leak | 内存泄漏的场景和示例 <br> 指针使用方法 |  |
| log | 日志log4cplus |  |
| doxygen | doxygen文档 |  |
| gtest | 单元测试 | C++11和cmake3.14，含单元测试覆盖率lcov |
| [ITK](https://medical.wangyaqi.cn/#/graphics/itk) | 图像处理库 <br> ITK指针使用方法 | CMakeSettings.json是在Windows开发和远程调试Linux服务器的CMake文件 |
| fastdds | https://github.com/eProsima/Fast-DDS/tree/master/examples/cpp/hello_world |  |

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

## libDebug
```
lib/dll: libDebugDll\Debug\
头文件: libDebugDll\libDebugDll\
pdb: libDebugDll\Debug\
code: libDebugDll\
```

* 运行时使用dll：在Visual Studio的项目"属性" -> "调试"，设置"环境"项 PATH=%PATH%;..\..\libDebugDll\Debug\
* 启用调试时看到源代码：确保有PDB文件和源代码(路径如和PDB编译时不一致，需设置源代码文件路径)
    1. 关闭 : 在Visual Studio的"Debug" -> "Options" -> "Debugging" -> "General" 中，取消勾选选项 "Enable Just My Code"。
    1. 设置PDB文件路径 : 在Visual Studio的"Debug" -> "Options" -> "Debugging" -> "Symbols"页面中添加包含PDB文件的目录路径。
    1. 设置源代码文件路径 : 在Visual Studio的"Debug" -> "Options" -> "Debugging" -> "General" -> "Source file locations" 输入源代码文件的目录路径。

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

## fastdds
* 需要安装vcpkg和fastdds

```
cd fastdds
cmake -G "Visual Studio 15 2017 Win64" -S . -B build -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build
```