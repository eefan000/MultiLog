#pragma once
#include "CoreMinimal.h"

enum MultiLogLevel
{
	// 严重错误,会导致程序无法正常运行
	Error = 0,
	// 异常警告,出现了不正常情况,但是还不至于导致程序无法运行
	Warning,
	// 正常运行信息,有助于出问题时查找流程,但是不要太多,只在一些不太频繁而且比较关键的地方
	Info,
	// 功能开发时用的临时调试信息
	DebugInfo,
};

struct LogFileInfo
{
	// 当前日志等级,默认只打印 LogLevel::Info及以上
	MultiLogLevel NowLogLevel = MultiLogLevel::Info;
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