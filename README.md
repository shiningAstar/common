# common 基础功能组件

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