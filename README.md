# MultiLog
简易的多文件日志打印    Simple Multi-file Log Printing

使用方式

1.将插件导入到需要使用的模块

  PublicDependencyModuleNames.AddRange(new string[] { "MultiLog" });
  
2.导入头文件

  #include "MultiLogSubsystem.h"
  
3.打印日志 (建议用宏封装起来)

  UMultiLogSubsystem::AddLog(FString(TEXT("test1")), MultiLogLevel::DebugInfo, TEXT("%02d:%02d"), 1, 2);
