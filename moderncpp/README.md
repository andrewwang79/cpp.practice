# moderncpp
## 目录结构
| 目录文件 | 内容 | 说明 |
| - | - | - |
| main.cpp | 现代C++特性：智能指针，bind，右值，lambda，线程 <br> 内存泄漏的场景和示例 |  |
| DynamicCreate.cpp | 反射机制 |  |
| ExecPool模板类 | 任务池，有timeout任务 |  |

## 编译执行
```
cd moderncpp
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build -j$((`nproc`+1))
./build/moderncpp
./build/DynamicCreate
./build/ExecPoolTest
./build/ThreadTest
```