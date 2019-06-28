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
	建议: 建议将这个接口用宏封装起来,比如 LOG_Error()...
	*/
	template<typename FmtType, typename ...Types>
	static void AddLog(const FString& LogTypeName, const EMultiLogLevel Level, const FmtType& Format, Types ...Args)
	{
		if (IsValid(MultiLogSubsystem))
		{
			FString Log = FString::Printf(Format, Args...);

			FDateTime CurrTime = FDateTime::Now();
			FString LineLog = CurrTime.ToString(TEXT("[%Y.%m.%d-%H.%M.%S.%s] "));
			switch (Level)
			{
			case EMultiLogLevel::Error:		LineLog.Append(TEXT("[Error] : "));		break;
			case EMultiLogLevel::Warning:	LineLog.Append(TEXT("[Warning] : "));	break;
			case EMultiLogLevel::Info:		LineLog.Append(TEXT("[Info] : "));		break;
			case EMultiLogLevel::DebugInfo:	LineLog.Append(TEXT("[DebugInfo] : "));	break;
			default: checkf(false, TEXT("UMultiLogSubsystem::AddLog Error!!!! EMultiLogLevel Mismatch!"));
			}
			LineLog.Append(Log);
			LineLog.AppendChar(TEXT('\n'));

			MultiLogSubsystem->AddLog_Inward(LogTypeName, LineLog, Level);
		}
	};

	/* 蓝图调用得打印日志接口 */
	UFUNCTION(BlueprintCallable, Category = "MultiLogSubsystem")
	void PrintLog(const FString& LogTypeName, const FString& Log, const EMultiLogLevel Level);

	/* 设置日志等级 */
	static bool SetMultiLogLeve(const FString& LogTypeName, const EMultiLogLevel Level);

	/* 注册日志文件类型 */
	static LogFileInfo* RegisterLogTypeName(const FString& LogTypeName);

private:
	void AddLog_Inward(const FString& LogTypeName, FString& LineLog, const EMultiLogLevel Level);

	static UMultiLogSubsystem* MultiLogSubsystem;

	// 子系统启动时间
	FString InitTime;

	FMultiLogWriteWorker LogWriteWorker;
	TMap<FString, LogFileInfo> MultiLogFile;
};
