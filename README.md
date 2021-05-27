# 最佳实践

## 目录结构
| 目录 | 内容 | 说明 |
| :----: | -- | -- |
| crash | 崩溃 |  |
| global | 全局变量引用 |  |
| leak | 内存泄漏 |  |
| log | 日志log4cplus |  |
| doxygen | doxygen文档 |  |
| gtest | 单元测试 | C++11和cmake3.14 |
| [ITK](https://tech.wangyaqi.cn/#/graphics/itk) | 图像处理库 | 在Windows开发和远程调试Linux服务器的CMake程序 |

## 其他的编译
```
mkdir -p out

// crash
gcc -o out/crash crash/main.c

// global
gcc -o out/global global/main_support.c global/main.c

// leak
gcc -o out/leakc leak/main.c
g++ -g -o out/leakcpp leak/main.cpp
valgrind --tool=memcheck --leak-check=full --show-reachable=yes ./out/leakcpp
valgrind --tool=massif --time-unit=B ./out/leakcpp && ms_print massif.out.30403

// log
// 获取log4cplus的头文件和lib，以下示例是通过conan
log4cplusPath=/root/.conan/data/log4cplus/2.0.4/Common/stable/package/0ab9fcf606068d4347207cc29edd400ceccbc944/
log4cplusHeaderPath=${log4cplusPath}/include/ && log4cplusLibPath=${log4cplusPath}/lib/release/
prj_path=/root/cpp.practice/log/
cd ${prj_path} && g++ -g -o log.exe main.cpp -I ${log4cplusHeaderPath} -L ${log4cplusLibPath} -llog4cplus
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${log4cplusLibPath} && ./log.exe
```

## 文档生成
```
函数定义参见DogService.h
主页参见main.cpp
新增导航树页面参见article.md
```

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

## ITK
```
// 用conan安装ITK
prj_path=/root/cpp.practice/itk/
module_name=itk
cd ${prj_path}
rm -rf build && mkdir build && cd build && conan install ../conanfile.txt -s arch=x86_64 -s os=Linux -r cloud --update && cd ..
cmake -S . -B build && cmake --build build && cd build/ && ./bin/${module_name}
```
