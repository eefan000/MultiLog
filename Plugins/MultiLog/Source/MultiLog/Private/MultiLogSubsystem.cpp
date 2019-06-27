// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiLogSubsystem.h"

UMultiLogSubsystem* UMultiLogSubsystem::MultiLogSubsystem = nullptr;

UMultiLogSubsystem::~UMultiLogSubsystem()
{
	Deinitialize();
}

void UMultiLogSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	FDateTime CurrTime = FDateTime::Now();
	InitTime = CurrTime.ToString(TEXT("-%Y.%m.%d-%H.%M.%S"));
	LogWriteWorker.JoyInit();
}

void UMultiLogSubsystem::Deinitialize()
{
	// 关闭写入日志线程
	LogWriteWorker.Shutdown();
	// 释放日志文件句柄
	for (auto& K_V : MultiLogFile)
	{
		K_V.Value.File->Close();
	}
	MultiLogFile.Empty();
}
void UMultiLogSubsystem::AddLog_Inward(const FString& LogTypeName, FString& Line)
{
	if (LogFileInfo * Info = MultiLogFile.Find(LogTypeName))
	{
		LogWriteWorker.AddLog(LineLog(Line, Info));
	}
	else
	{
		FString FileName = LogTypeName + InitTime + ".log";
		FString FilePath = FPaths::Combine(FPaths::ProjectLogDir(), LogTypeName, FileName);
		IFileManager* FileManager = &IFileManager::Get();
		FArchive* FileArchive = FileManager->CreateFileWriter(*FilePath, FILEWRITE_AllowRead);

		LogFileInfo NewLogFileInfo;
		NewLogFileInfo.File = FileArchive;

		MultiLogFile.Add(LogTypeName, NewLogFileInfo);

		LogWriteWorker.AddLog(LineLog(Line, &NewLogFileInfo));
	}
}
