# common 基础功能组件

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