#pragma once
#include "MultiLogWriteThreads.h"
//Thread Worker Starts as NULL, prior to being instanced. This line is essential! Compiler error without it
FMultiLogWriteWorker* FMultiLogWriteWorker::Runnable = NULL;
//***********************************************************

void FMultiLogWriteWorker::WriteLogBuffToFile(bool IsFlush)
{
	LogInfo Temp;
	while (LogBuff.Dequeue(Temp))
	{
		FString LineLog = Temp.PrintLogTime.ToString(TEXT("[%Y.%m.%d-%H.%M.%S.%s] "));
		switch (Temp.LogLevel)
		{
		case EMultiLogLevel::Error:		LineLog.Append(TEXT("[Error] : "));		break;
		case EMultiLogLevel::Warning:	LineLog.Append(TEXT("[Warning] : "));	break;
		case EMultiLogLevel::Info:		LineLog.Append(TEXT("[Info] : "));		break;
		case EMultiLogLevel::DebugInfo:	LineLog.Append(TEXT("[DebugInfo] : "));	break;
		default: checkf(false, TEXT("UMultiLogSubsystem::AddLog Error!!!! EMultiLogLevel Mismatch!"));
		}
		LineLog.Append(Temp.LogContent);
		LineLog.AppendChar(TEXT('\n'));

		Temp.LogFile->Serialize(TCHAR_TO_UTF8(*LineLog), LineLog.Len());
		if (IsFlush)
		{
			Temp.LogFile->Flush();
		}
	}
}

void FMultiLogWriteWorker::AddLog(LogInfo&& Log)
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