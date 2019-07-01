#pragma once
#include "Core.h"
#include "Containers/Queue.h"
#include "MultiLogType.h"

/*
不断的将日志写入多个目标文件
*/
class FMultiLogWriteWorker : public FRunnable
{
	/** 写入线程单例 */
	static  FMultiLogWriteWorker* Runnable;

	/** 实际线程,用来执行FMultiLogWriteWorker */
	FRunnableThread* Thread;

	/** 是否提前结束写入任务 */
	bool StopTask = false;

private:
	/** 待写入文件的日志 */
	TQueue<LogInfo, EQueueMode::Mpsc> LogBuff;

	void WriteLogBuffToFile(bool IsFlush = false);
public:
	void AddLog(LogInfo&& Log);
public:
	// 是否完成所有写入工作
	bool IsFinished() const
	{
		return !LogBuff.IsEmpty();
	}

	//~~~ Thread Core Functions ~~~

	//Constructor / Destructor
	FMultiLogWriteWorker();
	virtual ~FMultiLogWriteWorker();

	// Begin FRunnable interface.
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	virtual void Exit();
	// End FRunnable interface

	/** 等待线程结束 */
	void EnsureCompletion();



	//~~~ Starting and Stopping Thread ~~~

	/* 启动写入线程 */
	static FMultiLogWriteWorker* JoyInit();

	/** 关闭写入线程 */
	static void Shutdown();
};