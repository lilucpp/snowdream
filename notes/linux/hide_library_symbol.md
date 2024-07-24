1. ## **隐藏你的库的符号：**

   在编译时，使用 `-fvisibility=hidden` 选项来隐藏你的库的符号。这将使得库默认情况下将符号隐藏，除非你明确地指定某个符号需要被导出。

   ```
   bashCopy code
   gcc -fvisibility=hidden -o your_library.so -shared your_source.c
   ```

   这将隐藏你自己库中的符号，确保它们不会在动态链接时暴露给其他程序。

2. ## **导出需要的符号：**

   如果有一些你希望在库外可见的符号，你可以使用 `__attribute__((visibility("default")))` 来显式地导出它们。例如：

   ```
   cCopy code__attribute__((visibility("default"))) void my_function() {
       // 函数实现
   }
   ```

3. ## **隐藏第三方库的符号：**

   如果你想要隐藏第三方库的符号，你可以在链接时使用 `-Wl,--exclude-libs,ALL` 选项。这会告诉链接器排除所有库的符号，除了你明确指定的那些。请注意，这可能会对程序的正常运行产生影响，因此谨慎使用。

   ```
   bashCopy code
   gcc -Wl,--exclude-libs,ALL -o your_executable your_objects.o -lyour_library -lthird_party_library
   ```

   请替换 `your_library` 和 `third_party_library` 为实际的库名字。

请注意，这些选项可能会因为不同的编译器版本和系统而略有不同，你可能需要查阅你使用的编译器和系统的文档来获取更准确的信息。
