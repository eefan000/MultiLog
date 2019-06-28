#pragma once
#include "MultiLogWriteThreads.h"
//Thread Worker Starts as NULL, prior to being instanced. This line is essential! Compiler error without it
FMultiLogWriteWorker* FMultiLogWriteWorker::Runnable = NULL;
//***********************************************************

void FMultiLogWriteWorker::WriteLogBuffToFile(bool IsFlush)
{
	LineLog Temp;
	while (LogBuff.Dequeue(Temp))
	{
		Temp.LogFile.File->Serialize(TCHAR_TO_UTF8(*Temp.Line), Temp.Line.Len());
		if (IsFlush)
		{
			Temp.LogFile.File->Flush();
		}
	}
}

void FMultiLogWriteWorker::AddLog(LineLog&& Log)
{
	LogBuff.Enqueue(MoveTemp(Log));
}

FMultiLogWriteWorker::FMultiLogWriteWorker()
{
	Thread = FRunnableThread::Create(this, TEXT("FMultiLogWriteWorker"), 0, TPri_BelowNormal);
}

FMultiLogWriteWorker::~FMultiLogWriteWorker()
{
	delete Thread;
	Thread = NULL;
}

//Init
bool FMultiLogWriteWorker::Init()
{
	return true;
}

//Run
uint32 FMultiLogWriteWorker::Run()
{
	//启动初始等待
	FPlatformProcess::Sleep(0.05);

	while (!StopTask)
	{
		WriteLogBuffToFile();
		FPlatformProcess::Sleep(0.05);
	}
	return 0;
}

//stop
void FMultiLogWriteWorker::Stop()
{
	StopTask = true;
}

void FMultiLogWriteWorker::Exit()
{
	WriteLogBuffToFile(true);
}

FMultiLogWriteWorker* FMultiLogWriteWorker::JoyInit()
{
	//Create new instance of thread if it does not exist and the platform supports multi threading!
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FMultiLogWriteWorker();
	}
	return Runnable;
}

void FMultiLogWriteWorker::EnsureCompletion()
{
	Stop();
	if (Thread)
	{
		Thread->WaitForCompletion();
	}
}

void FMultiLogWriteWorker::Shutdown()
{
	if (Runnable)
	{
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = NULL;
	}
}