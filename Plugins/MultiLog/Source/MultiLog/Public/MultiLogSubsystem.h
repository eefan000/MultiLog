// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "MultiLogType.h"
#include "MultiLogWriteThreads.h"
#include "MultiLogSubsystem.generated.h"

UCLASS()
class MULTILOG_API UMultiLogSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
	~UMultiLogSubsystem();
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
public:
	/* 用户接口 */
	/**
	根据日志类型名,将日志写入到不同的文件

	注意: 在UMultiLogSubsystem子系统还没启动之前调用将丢失日志信息
	*/
	template<typename FmtType, typename ...Types>
	static void AddLog(const FString& LogTypeName, const MultiLogLevel Level, const FmtType& Format, Types ...Args)
	{
		if (IsValid(MultiLogSubsystem))
		{
			FString Log = FString::Printf(Format, Args...);

			FDateTime CurrTime = FDateTime::Now();
			FString LineLog = CurrTime.ToString(TEXT("[%Y.%m.%d-%H.%M.%S.%s] "));
			switch (Level)
			{
			case MultiLogLevel::Error:		LineLog.Append(TEXT("[Error] : "));		break;
			case MultiLogLevel::Warning:	LineLog.Append(TEXT("[Warning] : "));	break;
			case MultiLogLevel::Info:		LineLog.Append(TEXT("[Info] : "));		break;
			case MultiLogLevel::DebugInfo:	LineLog.Append(TEXT("[DebugInfo] : "));	break;
			}
			LineLog.Append(Log);
			LineLog.AppendChar(TEXT('\n'));

			MultiLogSubsystem->AddLog_Inward(LogTypeName, LineLog);
		}
		else
		{
			// 尝试获取MultiLogSubsystem
			MultiLogSubsystem = GEngine->GetEngineSubsystem<UMultiLogSubsystem>();
			if (IsValid(MultiLogSubsystem))
			{
				AddLog(LogTypeName, Level, Format, Args...);
			}
		}
	};

private:
	void AddLog_Inward(const FString& LogTypeName, FString& LineLog);

	static UMultiLogSubsystem* MultiLogSubsystem;

	// 子系统启动时间
	FString InitTime;

	FMultiLogWriteWorker LogWriteWorker;
	TMap<FString, LogFileInfo> MultiLogFile;
};
