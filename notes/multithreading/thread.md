# Thread

## Managing threads

1. 线程构造：callable对象。注意下面的情况

   ```c++
   class foo {
     public:
     foo(){}
     void operator()(){
       std::cout << "hello, world" << std::endl;
     }
   };
   
   std::thread t(foo()); // error,编译器理解成了函数声明，t.join() 会报错。
   std::thread t{foo()};
   ```

2. Join、detach的推荐用法：

   ```c++
   if(t.joinable()) t.join();  
   if(t.joinable()) t.detach();
   ```

   object is not joinable in any of these cases:

   - if it was *[default-constructed](http://www.cplusplus.com/thread::thread)*.
   - if it has been moved from (either *[constructing](http://www.cplusplus.com/thread::thread)* another [thread](http://www.cplusplus.com/thread) object, or *[assigning to it](http://www.cplusplus.com/thread::operator=)*).
   - if either of its members [join](http://www.cplusplus.com/thread::join) or [detach](http://www.cplusplus.com/thread::detach) has been called.

   不能joinable的thread，join、detach会异常退出。

3. 线程异常，如果生成线程的位置没有处理，就会往上抛，知道主函数，如果都没有处理就会使整个进程退出。

4. 确保std::thread对象被销毁前已调用join()或detach()函数。

   如果线程不需要等待返回结果，好的习惯是直接detach。

   ```c++
   void apple() {
       sleep(1);
       std::cout <<  std::endl;
   }
   
   void demo() {
       std::cout << "demo begin..." << std::endl;
       std::thread t(apple);
     	/* 如果此处不加detach，会导致程序异常退出。	
     	*libc++abi.dylib: terminating
   		*Abort trap: 6
       */
       if(t.joinable()) t.detach(); 
       std::cout << "demo done..." << std::endl;
   }
   ```

5. 线程最好不要引用临时变量。

   ```c++
   struct func {
       int& i;
       func(int& i_):i(i_){}
       void operator()() {
        for(unsigned j=0;j<1000000;++j)
        do_something(i); // Potential access to dangling reference
       }
   };
   void oops() {
       int some_local_state=0;
       func my_func(some_local_state);
       std::thread my_thread(my_func);
       my_thread.detach();
   }
   ```

   oops结束时，线程my_func可能还未结束，i就变成了dangling reference。

   如果非要引用临时变量，就要join等待线程结束或copy 临时变量到stuct func中。

6. thread_guard

   ```c++
   class thread_guard {
       std::thread& t;
   		public:
       explicit thread_guard(std::thread& t_):
           t(t_){}
   		~thread_guard() {
       	if(t.joinable()) {
   				t.join();
         }
   		}
   		thread_guard(thread_guard const&)=delete;
   		thread_guard& operator=(thread_guard const&)=delete;
   };
   struct func;
   void f() {
       int some_local_state=0;
       func my_func(some_local_state);
       std::thread t(my_func);
       thread_guard g(t);
       do_something_in_current_thread();
   }
   ```

7. 错误的参数传递

   ```c++
   #include <iostream>
   #include <thread>
   
   void talk(const std::string& words) {
     std::cout << words;
   }
   
   void doInThread(int param) {
     char buffer[10];
     sprintf(buffer, "%i", param);
     std::thread t(talk, buffer);
     t.detach();
   }
   
   int main() {
     doInThread(1);
     doInThread(2);
     doInThread(3);
   }
   ```

   该程序的输出取决于执行线程的顺序，但是在某些情况下，某些完全出乎预料的情况会发生。输出如下：*333*。这应该是不可能的，因为只有一个线程被告知要打印3。

   问题来自缓冲区变量。此变量是char *。当doInThread退出时，该指针的内存将被释放并可以再次使用。因为我们连续调用doInThread 3次，所以每次调用都会分配和释放相同的内存。它被分配的最后一个值是3。

   棘手的是，当我们创建线程时，**所有参数都被复制到线程上下文中**。在这种情况下，char* *被复制到线程中。在线程上下文中，*char *转换为std :: string并由talk函数使用。char *的复制会立即进行，但将来可能会转换为std :: string。在我们得到*333的情况下，直到缓冲区到3之后才进行转换，因此所有线程最终都转换为相同的值。

   在我看来，这种情况很难避免，因此在使用线程时保持这种自动转换可能是个好主意。进行以下明确的转换是比较安全的：

   ```c++
   std::thread t(talk, std::string(buffer)); 
   ```

   [引申1](https://blog.galowicz.de/2016/03/23/const_reference_to_temporary_object/):

   在C和C ++中，持有对临时创建的对象的引用（或C中的指针）不是理智的想法。但是实际上，C ++提供了一个有趣的功能，即可以将临时对象的生存期延长到指向它的引用的生存期。

   引申2:

   C++11 中引入 `std::ref` 用于取某个变量的引用，这个引入是为了解决一些传参问题。std::bind, std::thread。

8. std::thread进行所有权移动

   如 std::thread t ;

   t = std::thread(some_function);

   不需要调用std::move 来显式移动所有权。**临时对象进行移动是自动和隐式的。**

   