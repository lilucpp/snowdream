# /Zc:threadSafeInit (Thread-safe Local Static Initialization)

**/Zc： threadSafeInit** 编译器选项告知编译器在线程安全的方式下初始化静态本地 (函数范围) 变量，从而无需手动同步。 只有初始化是线程安全的。 使用和修改多个线程的静态局部变量仍必须手动同步。 从 Visual Studio 2015 开始，此选项可用。 默认情况下，Visual Studio 启用此选项。



在 c + + 11 标准中，具有静态或线程存储持续时间的块范围变量必须在进行任何其他初始化之前都为零初始化。 当控件第一次通过变量的声明时发生初始化。 如果在初始化过程中引发了异常，则该变量将被视为未初始化，并将在下一次控件通过声明时重新尝试初始化。 如果控件同时与初始化一起进入声明，则在初始化完成时并发执行将被阻止。 如果控件在初始化过程中以递归方式重新输入声明，则行为是不确定的。 默认情况下，Visual Studio 从 Visual Studio 2015 开始实现此标准行为。 此行为可以通过设置 **/zc： threadSafeInit** 编译器选项显式指定。

默认情况下， **/zc： threadSafeInit** 编译器选项处于启用状态。 [/Permissive-](https://docs.microsoft.com/zh-cn/cpp/build/reference/permissive-standards-conformance?view=msvc-160)选项不影响 **/zc： threadSafeInit**。



线程安全的静态局部变量将内部 (TLS) 使用线程本地存储，以便在静态已初始化时提供高效执行。 此功能的实现依赖于 Windows Vista 和更高版本操作系统中 Windows 操作系统支持功能。 WindowsXP、Windows Server 2003 及更低版本的操作系统不支持此支持，因此它们不会获得效率优势。 这些操作系统还限制了可加载的 TLS 部分数量。 超过 TLS 部分限制可能导致崩溃。 如果这是代码中的问题，尤其是在必须在较早版本的操作系统上运行的代码中，请使用 **/zc:threadSafeInit-** 禁用线程安全的初始化代码

### To set this compiler option in the Visual Studio development environment

1. Open the project's **Property Pages** dialog box. For details, see [Set C++ compiler and build properties in Visual Studio](https://docs.microsoft.com/en-us/cpp/build/working-with-project-properties?view=msvc-160).
2. From the **Configurations** drop down menu, choose **All Configurations**.
3. Select the **Configuration Properties** > **C/C++** > **Command Line** property page.
4. Modify the **Additional Options** property to include **/Zc:threadSafeInit** or **/Zc:threadSafeInit-** and then choose **OK**.