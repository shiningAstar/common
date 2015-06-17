# common 基础功能组件

v 0.1.1.6

做了以下调整，主要测试windows版本，linux版本编译通过，功能待测
1. 加入链表结构类型
2. NorMap映射类型，加入add与del映射对函数
3. 加入链表获得数据节点宏 list_entry
4. 添加了代码注释与函数功能说明，添加了List与ListT类的构造与析构函数，添加了二者的函数定义
5. 修改了ShareMemory带参数的构造函数
6. 修改了MutexLock中的MutexLockGuard类,将传入的引用变量变为指针。
7. 修改SemaphoreinProcessPV类中的init函数，添加了win32的宏定义，用来处理windows中的sem_init中的共享标志位必须为0的问题，
8. 修改SemaphoreoutProcessPV类中的init函数中的sem_open创建失败返回值的判断，增加了返回-1或0xffffffff是错误判断值。
9. 修改了CurrentThread命名空间中tid()函数，将其中的GerCurrentThread函数换为GerCurrentThreadId()函数。
10. 修改了MutexLock.h中的MutexLockOutProcess类，添加了linux的条件编译，添加了windows下的变量，
    h_mutex用于接收句柄。添加了enum类型的标志
11. 修改了SemaphorePV.h中的SemvphoreoutPorcessPV类，添加了windows和Linux的调按键编译，添加了
    windows下的变量h_sema用于接收句柄，添加了对tryp(),getvalue()的条件编译
12. 修改了MutexLock.cpp中的MutexLockOutProcess类，添加了linux的条件编译，修改了带参数的构造函数,
    修改了init函数，添加了windows条件下的互斥锁的初始化，修改了lock函数添加了windows下的互斥锁的实现，
    修改了unlock函数，添加了windows下互斥锁的解锁，修改了析构函数。添加了windows下的句柄关闭。
13. 修改了SemaphorePV.cpp中的SemvphoreoutPorcessPV类的成员函数，修改了带参数与不带参数的构造函数，修改了
    析构函数，修改了p(),p(sec,nsec)函数，v(),v(value)函数，init函数，对这些函数，添加了条件编译，增加了windows下的跨进程信号量实现代码。
    同时添加了对getvalue函数的条件编译使其不再windows下出现
14. 修改了SemvphoreoutPorcessPV与MutexLockOutProcess的init函数中Create存在时未关闭句柄的bug。
15. 调整了MySocket的Create接口，与一些与地址、协议类型有关的函数，使得可以在创建时指定domain，从而实现创建本地socket
16. 调整了Thread类型的状态，存储是否需要在线程启动时detach线程，以提供unjoined线程可以在其他线程join等待(调用waittoStop)，或者选择
    detach线程而自动回收资源，使得join不再起作用。
17. 修改BlockingInterruptor类型，使初始化端口随机。
18. 修改SemaphoreOutProcessPV的V(int res_count)函数，使条件编译仅windows可用
19. 修改NorMap.h，加入了查找接口
20. 修改NorMap的删除过程，加入了删除后的索引与游标的重置

v 0.1.1.5

做了windows版本的下列调整
1. 修改Thread类，加入等待结束函数
2. 加入crc校验16位，32位函数
3. 加入字符-指针映射类 FuncPtrMap
4. 加入打断阻塞类 BlockingInterruptor
5. 修改FileConfig，修正读取EOF后的行为
6. 修改ConfigBase，修正根据index查询时返回值指针问题引起的段错误
7. 加入字符-函数指针表、字符-数字id表类型，FuncPtrMap, CmdIdMap，并实现按index遍历接口
8. 加入了泛型支持通用映射类NorMap
9. 加入了切割字符串函数splite_string
10. 修改split_string,解决二维指针参数传递问题，第四个参数调用时(char*)split_str或&split_str[0][0]
11. 完成了NorMap的测试，修改了其中string 赋值给const
	char*的类型不匹配问题。添加了一些对函数参数的注释，修改后，测试已通过
12. 完成了split_str函数的测试，添加了对函数参数的的描述性注释，测试已通过
13. 修改crc16位与32位的生成函数与验证函数
14. 加入modbus的crc16算法
15. 修改了crc16位校验中的函数使用错误，讲memcpy()改为了memcmp
16. 修改了crc校验函数，添加了update_crc16_by_string 和update_crc32_by_string来更新Crc校验码。
17. 修改了crc校验函数，添加了int_crc16 和inti_crc32函数来初始化crc校验码，将crc生成分为了一次性生成函数与分段生成函数，其中make与verify配合使用,init与update配合使用

v 0.1.1.4

主要工作：适应linux下编译的跨平台的差异性
1. 修改linux下logFile类的strlen函数的头文件引用
2. 修改linux下MySocket类，从字符串得到网络地址静态函数（暂时去掉）
3. 适应linux下pthread_mutex_t类型与windows差别，初始化时方式。
4. linux下gettid的调用
5. 加入判断字符串是否合法函数
6. 修改ConfigBase，加入迭代方式，实现通过索引查询键值对接口
7. 适应linux下sem_t初始化方法，修改判断定义了SEM_VALUE_MAX
8. 适应linux下pthread_attr_t类型，修改操作方法，加入string.h头文件，调整调用syscall方法，加入#define
    _GNU_SOURCE
      #include "unistd.h"
      #include "sys/syscall.h"
      #include "sys/types.h"
9. 修改ShareMemory的munmap调用，加入头文件sys/mman.h，文件操作加入fcntl.h
    unistd.h，fstat函数头文件sys/fstat.h
10. 修改Makefile适应linux编译
11. 修改main.cpp，使windows.h根据平台条件include


v 0.1.1.3

1. windows下Makefile改名，加上.windows后缀，以适应linux/windows编译


v 0.1.1.2

1. 修改配置Config类，解决stl map在dll之间传递可能的不匹配问题
2. 加入配置Config类，根据索引查找数据的接口，用于查找与遍历


v 0.1.1.1

修正不能编译的问题


v 0.1.1

1. 加入线程状态与线程等待结束函数


v 0.1.0

1. 日志 异常处理 组件
   日志基类 logBase 派生所有日志类
   基于终端控制台输出的日志类 logTerminal
   向文件输出日志类 logFile
   向控件自定义输出的日志类 logControl
   日志功能引用组件头文件 log.h
   
2. 配置管理 组件
   配置管理基类 ConfigBase
   文件配置管理类 FileConfig
   
3. 文件操作 组件
   文件操作类 File
   
4. 线程 组件
   线程操作类 Thread
   
5. 信号量 组件
   信号量基类 SemaphoreBase
   进程内无名信号量 SemaphoreOutProcessPV
   
6. 互斥锁 组件
   互斥锁基类 MutexLockBase
   进程内互斥锁 MutexLockInProcess
   
7. 共享内存 组件
   共享内存类 SharedMemory
   
8. 网络通信 组件
   网络通信Socket类 MySocket