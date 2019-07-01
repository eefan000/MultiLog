// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "MultiLogType.h"
#include "HAL/CriticalSection.h"
#include "MultiLogSubsystem.generated.h"

class FMultiLogWriteWorker;

UCLASS()
class MULTILOG_API UMultiLogSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
	~UMultiLogSubsystem();
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
private:
	//void AddLog_Inward(LogFileInfo* Info, const FString& LogTypeName, FString& LineLog, const EMultiLogLevel Level);
	LogFileInfo* FindOrAdd_LogFileInfo(const FName& LogTypeName);

	static UMultiLogSubsystem* MultiLogSubsystem;

	// 子系统启动时间
	FString InitTime;

	FMultiLogWriteWorker* LogWriteWorker;

	FCriticalSection AllLogFileInfo_Lock;
	/* 日志类型应该比较少,大概不会超过100, 100以内搜索速度比TMap更快 */
	TArray<LogFileInfo> AllLogFileInfo;

	void AddLog_Private(LogInfo&& Log);

public:
	/* 用户接口 */
	/**
	根据日志类型名,将日志写入到不同的文件

	注意: 在UMultiLogSubsystem子系统还没启动之前调用将丢失日志信息
	建议: 建议将这个接口用宏封装起来,比如 LOG_Error()...
	*/
	template<typename FmtType, typename ...Types>
	static void AddLog(const FName& LogTypeName, const EMultiLogLevel Level, const FmtType& Format, Types ...Args)
	{
		if (IsValid(MultiLogSubsystem))
		{
			LogFileInfo* Info = MultiLogSubsystem->FindOrAdd_LogFileInfo(LogTypeName);
			if (Level <= Info->NowLogLevel)
			{
				FString LogContent = FString::Printf(Format, Args...);
				MultiLogSubsystem->AddLog_Private( LogInfo(Info->AllLogFileInfo_Index, MoveTemp(LogContent), Info->File, Level) );
			}
		}
	};

	/* 蓝图调用得打印日志接口 */
	UFUNCTION(BlueprintCallable, Category = "MultiLogSubsystem")
	void PrintLog(const FName& LogTypeName, const FString& Log, const EMultiLogLevel Level);

	/* 设置日志等级 */
	static bool SetMultiLogLeve(const FString& LogTypeName, const EMultiLogLevel Level);
};