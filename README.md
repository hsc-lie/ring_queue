### 环形队列
- 非线程安全

### 使用说明
- 1.只需要将ring_queue文件夹中的文件移动到自己的工程中即可
- 2.可参考test/ring_queue_test.c中的测试代码使用

### 编译当前工程的示例
Linux环境下

    mkdir build
    cd build
    cmake ..
    cmake --build .

Windows环境下，以MinGW64为例

    mkdir build
    cd build
    cmake -G "MinGW Makefiles" ..
    cmake --build .

输出的可执行文件和静态库均在./build/out文件夹内
