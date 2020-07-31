自旋锁：
	1.如果单核cpu，内核不支持抢占，自旋锁无效，及加锁和解锁会变编译成空操作。
	  此时不支持抢占，代码运行是安全的，所以加锁解锁没有意义
	2.单核cpu，内核支持抢占，加锁<=>禁止抢占，解锁<=>使能抢占
	3.多核cpu，

	注意：1.自死锁 2.ABBA死锁 3.不能锁不能睡眠 4.在程序或函数返回之前要解锁
	include/linux/spinlock.h
读写锁：
信号量：
	include/linux/semaphore.h
	加锁不成功会睡眠等待
互斥量：
	include/linux/mutex.h
完成量：
	include/linux/completion.h
seq锁：
	基于自旋锁，写者优先
