# Ŀ¼�ṹ
| Ŀ¼ | ���� | ˵�� |
| :----: | -- | -- |
| crash | ���� |  |
| global | ȫ�ֱ������� |  |
| leak | �ڴ�й© |  |

# ����
```
mkdir -p out

gcc -o out/crash crash/main.c

gcc -o out/global global/main_support.c global/main.c

gcc -o out/leakc leak/main.c
g++ -g -o out/leakcpp leak/main.cpp
valgrind --tool=memcheck --leak-check=full --show-reachable=yes ./out/leakcpp
valgrind --tool=massif --time-unit=B ./out/leakcpp && ms_print massif.out.30403
```