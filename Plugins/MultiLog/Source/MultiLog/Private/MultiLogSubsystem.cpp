#include "MultiLogSubsystem.h"
#include "MultiLogWriteThreads.h"

// 注册控制台指令
FAutoConsoleCommand SetMultiLogLevel(
	TEXT("MultiLog.SetMultiLogLevel"),
	TEXT("MultiLog.SetMultiLogLevel LogTypeName LogLevel\n\nMultiLogLevel::NoLog = -1\nMultiLogLevel::Error = 1\nMultiLogLevel::Warning = 2\nMultiLogLevel::Info = 3\nMultiLogLevel::DebugInfo = 4"),
	FConsoleCommandWithWorldArgsAndOutputDeviceDelegate::CreateStatic(
		[](const TArray<FString>& Param, UWorld* _World, FOutputDevice& OutputDevice)
		{
			if (Param.Num() == 2)
			{
				FString LogTypeName = Param[0];
				int32 LogLeve = FCString::Atoi(*Param[1]);
				if (LogLeve > 0 || LogLeve == EMultiLogLevel::NoLog)
				{
					LogLeve = FMath::Min(LogLeve, (int32)EMultiLogLevel::DebugInfo);
					bool IsSet = UMultiLogSubsystem::SetMultiLogLeve(LogTypeName, (EMultiLogLevel)LogLeve);

					if (IsSet)
					{
						OutputDevice.Logf(TEXT("OK! LogTypeName=%s  LogLeve=%d"), *LogTypeName, LogLeve);
					}
					else
					{
						OutputDevice.Logf(TEXT("The specified LogTypeName was not found!!  LogTypeName=%s"), *LogTypeName);
					}
				}
				else
				{
					OutputDevice.Logf(TEXT("LogLevel error!\n\nMultiLogLevel::NoLog = -1\nMultiLogLevel::Error = 1\nMultiLogLevel::Warning = 2\nMultiLogLevel::Info = 3\nMultiLogLevel::DebugInfo = 4"));
				}
			}
			else
			{
				OutputDevice.Logf(TEXT("Insufficient parameters!"));
			}
		}
	)
);

UMultiLogSubsystem* UMultiLogSubsystem::MultiLogSubsystem = nullptr;

UMultiLogSubsystem::~UMultiLogSubsystem()
{
	Deinitialize();
};

void UMultiLogSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	FDateTime CurrTime = FDateTime::Now();
	InitTime = CurrTime.ToString(TEXT("-%Y.%m.%d-%H.%M.%S"));
	LogWriteWorker = FMultiLogWriteWorker::JoyInit();
	MultiLogSubsystem = this;
	AllLogFileInfo.Reserve(100);
};

void UMultiLogSubsystem::Deinitialize()
{
	// 关闭写入日志线程
	LogWriteWorker->Shutdown();
	// 释放日志文件句柄
	for (auto& Info : AllLogFileInfo)
	{
		Info.File->Close();
	}
	AllLogFileInfo.Empty();
};

void UMultiLogSubsystem::PrintLog(const FName& LogTypeName, const FString& Log, const EMultiLogLevel Level)
{
	AddLog(LogTypeName, Level, TEXT("%s"), *Log);
};

bool UMultiLogSubsystem::SetMultiLogLeve(const FString& LogTypeName, const EMultiLogLevel Level)
{
	if (IsValid(MultiLogSubsystem))
	{
		LogFileInfo* Info = MultiLogSubsystem->FindOrAdd_LogFileInfo(FName(*LogTypeName));
		if (Info)
		{
			FScopeLock ScopeLock(&MultiLogSubsystem->AllLogFileInfo_Lock);

			Info->NowLogLevel = Level;
			return true;
		}
	}
	return false;
}

LogFileInfo* UMultiLogSubsystem::FindOrAdd_LogFileInfo(const FName& LogTypeName)
{
	for (LogFileInfo& Info : AllLogFileInfo)
	{
		if (Info.LogTypeName == LogTypeName)
		{
			return &Info;
		}
	}

	{
		FScopeLock ScopeLock(&AllLogFileInfo_Lock);

		FString FileName = LogTypeName.ToString() + InitTime + ".log";
		FString FilePath = FPaths::Combine(FPaths::ProjectLogDir(), LogTypeName.ToString(), FileName);
		IFileManager* FileManager = &IFileManager::Get();
		FArchive* FileArchive = FileManager->CreateFileWriter(*FilePath, FILEWRITE_AllowRead);

		AllLogFileInfo.Emplace(LogFileInfo(AllLogFileInfo.Num(), LogTypeName, EMultiLogLevel::Info, FileArchive));

		return &AllLogFileInfo.Last();
	}
}
void UMultiLogSubsystem::AddLog_Private(LogInfo&& Log)
{
	LogWriteWorker->AddLog(MoveTemp(Log));
};