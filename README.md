# KKThreadMonitor
轻量级线程监控工具，当线程数量过多或线程爆炸时候，就打印所有线程堆栈。

# 效果
当线程爆炸或线程数量过多时候，控制台打印所有的线程堆栈~
```
🔥💥💥💥💥💥一秒钟开启 28 条线程！💥💥💥💥💥🔥

👇👇👇👇👇👇👇堆栈👇👇👇👇👇👇👇
2020-04-12 12:36:29.270755+0800 BaiduIphoneVideo[55732:6928996] 
当前线程数量：43

callStack of thread: 9219
libsystem_kernel.dylib         0x18022cc38 semaphore_wait_trap  +  8
libdispatch.dylib              0x00003928 _dispatch_sema4_wait  +  28
BaiduIphoneVideo               0x101285be0 -[TDXXXX脱敏🤣处理（去掉真实类名）XXXXager sendRequestWithBody:withURL:flag:]  +  440
BaiduIphoneVideo               0x10128535c __29-[TDAXXXX脱敏🤣处理（去掉真实类名）XXXXnager sendMessage]_block_invoke  +  804
libdispatch.dylib              0x00001dfc _dispatch_call_block_and_release  +  32
... //省略中间线程堆栈
callStack of thread: 8707
libsystem_kernel.dylib         0x18022cc38 semaphore_wait_trap  +  8
libdispatch.dylib              0x00003928 _dispatch_sema4_wait  +  28
BaiduIphoneVideo               0x1026d2cd4 +[BaiduMobStatDeviceInfo testDeviceId]  +  56
BaiduIphoneVideo               0x1026ca8a4 -[BaiduMobStatLogManager checkHeaderBeforeSendLog:]  +  3384
BaiduIphoneVideo               0x1026cefdc -[BaiduMobStatLogManager _syncSendAllLog]  +  536
BaiduIphoneVideo               0x1026c6398 __33-[BaiduMobStatController sendLog]_block_invoke  +  56
libdispatch.dylib              0x00001dfc _dispatch_call_block_and_release  +  32
... //省略中间线程堆栈
callStack of thread: 80387
libsystem_kernel.dylib         0x18024ea60 __open_nocancel  +  8
libsystem_kernel.dylib         0x1802356e0 open$NOCANCEL  +  20
BaiduIphoneVideo               0x101dcd488 -[XXXX脱敏🤣处理（去掉真实类名）XXXX recursiveCalculateAtPath:isAbnormal:isOutdated:needCheckIgnorePath:]  +  1360
BaiduIphoneVideo               0x101dcd488 -[XXXX脱敏🤣处理（去掉真实类名）XXXX recursiveCalculateAtPath:isAbnormal:isOutdated:needCheckIgnorePath:]  +  1360
BaiduIphoneVideo               0x101dcd488 -[XXXX脱敏🤣处理（去掉真实类名）XXXX recursiveCalculateAtPath:isAbnormal:isOutdated:needCheckIgnorePath:]  +  1360
BaiduIphoneVideo               0x101dccd90 -[XXXX脱敏🤣处理（去掉真实类名）XXXX initWithOutdatedDays:abnormalFolderSize:abnormalFolderFileNumber:ignoreRelativePathes:checkSparseFile:sparseFileLeastDifferPercentage:sparseFileLeastDifferSize:visitors:]  +  576
BaiduIphoneVideo               0x101dcd274 +[XXXX脱敏🤣处理（去掉真实类名）XXXX folderSizeAtPath:]  +  72
BaiduIphoneVideo               0x101de3900 __56-[HMDToBCrashTracker(Environment) environmentInfoUpdate]_block_invoke_2  +  88
libdispatch.dylib              0x00001dfc _dispatch_call_block_and_release  +  32

👆👆👆👆👆👆👆堆栈👆👆👆👆👆👆👆
```

# 用法
```
//在main函数里，或者任何你想开始监控的地方调用startMonitor，就可以了
//一般在main，或者application:didFinishLaunchingWithOptions:函数里。
[KKThreadMonitor startMonitor];

//在KKThreadMonitor.m文件里，可根据需求修改这两个值
#define KK_THRESHOLD 40   //表示线程数量超过40，就打印所有线程堆栈(根据自己项目来定！！！)
static const int threadIncreaseThreshold = 10;  //表示一秒钟新增加的线程数量（新建的线程数量 - 销毁的线程数量）超过10，就打印所有的线程堆栈
```

# 说明
打印线程堆栈，我目前是用自己另外一个库[KKCallStack](https://github.com/maniackk/KKCallStack)。

# 原理
[博客](https://juejin.im/post/5e92a113e51d4547134bdadb)
