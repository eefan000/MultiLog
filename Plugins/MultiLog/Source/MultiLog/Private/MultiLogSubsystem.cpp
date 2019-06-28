#include "MultiLogSubsystem.h"
__pragma(optimize("", off))
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
	LogWriteWorker.JoyInit();
	MultiLogSubsystem = this;
};

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
};

void UMultiLogSubsystem::PrintLog(const FString& LogTypeName, const FString& Log, const EMultiLogLevel Level)
{
	AddLog(LogTypeName, Level, TEXT("%s"), *Log);
};

bool UMultiLogSubsystem::SetMultiLogLeve(const FString& LogTypeName, const EMultiLogLevel Level)
{
	LogFileInfo* Info = RegisterLogTypeName(LogTypeName);
	if (Info)
	{
		Info->NowLogLevel = Level;
		return true;
	}

	return false;
};

LogFileInfo* UMultiLogSubsystem::RegisterLogTypeName(const FString& LogTypeName)
{
	if (IsValid(MultiLogSubsystem))
	{
		LogFileInfo* Info = MultiLogSubsystem->MultiLogFile.Find(LogTypeName);
		if (Info == nullptr)
		{
			FString FileName = LogTypeName + MultiLogSubsystem->InitTime + ".log";
			FString FilePath = FPaths::Combine(FPaths::ProjectLogDir(), LogTypeName, FileName);
			IFileManager* FileManager = &IFileManager::Get();
			FArchive* FileArchive = FileManager->CreateFileWriter(*FilePath, FILEWRITE_AllowRead);
			
			LogFileInfo NewLogFileInfo;
			NewLogFileInfo.File = FileArchive;

			return &MultiLogSubsystem->MultiLogFile.Add(LogTypeName, NewLogFileInfo);
		}
		return Info;
	}
	return nullptr;
}

void UMultiLogSubsystem::AddLog_Inward(const FString& LogTypeName, FString& Line, const EMultiLogLevel Level)
{
	if (LogFileInfo * Info = MultiLogFile.Find(LogTypeName))
	{
		if (Level <= Info->NowLogLevel)
		{
			LogWriteWorker.AddLog(LineLog(Line, Info));
		}
	}
	else
	{
		LogFileInfo*  NewLogFileInfo = RegisterLogTypeName(LogTypeName);
		if (Level <= NewLogFileInfo->NowLogLevel)
		{
			LogWriteWorker.AddLog(LineLog(Line, NewLogFileInfo));
		}
	}
}
__pragma(optimize("", on))