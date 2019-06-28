#pragma once
#include "CoreMinimal.h"
#include "MultiLogType.generated.h"

UENUM(BlueprintType)
enum EMultiLogLevel
{
	// 这是用于关闭所有日志,不要用这个等级打印消息
	NoLog = -1 UMETA(DisplayName = "NoLog"),
	// 严重错误,会导致程序无法正常运行
	Error = 1 UMETA(DisplayName = "Error"),
	// 异常警告,出现了不正常情况,但是还不至于导致程序无法运行
	Warning UMETA(DisplayName = "Warning"),
	// 正常运行信息,有助于出问题时查找流程,但是不要太多,只在一些不太频繁而且比较关键的地方
	Info UMETA(DisplayName = "Info"),
	// 功能开发时用的临时调试信息
	DebugInfo UMETA(DisplayName = "DebugInfo"),
};

struct LogFileInfo
{
	// 当前日志等级,默认只打印 LogLevel::Info及以上
	EMultiLogLevel NowLogLevel = EMultiLogLevel::Info;
	// 当前日志文件句柄
	FArchive* File;
};

struct LineLog
{
	FString Line;
	LogFileInfo LogFile;

	LineLog() {};
	LineLog(FString& Line, LogFileInfo* LogFile) : Line(Line), LogFile(*LogFile) {};
};