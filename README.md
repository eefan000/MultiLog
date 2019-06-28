<a name="dWsLc"></a>
## 简介
- 简易的多文件日志打印 Simple Multi-file Log Printing
<a name="XM9kU"></a>
## 使用方式

1. 将MultiLog插件放入Plugins目录
1. 在需要用到得模块(xx.Build.cs)中导入 PublicDependencyModuleNames.AddRange("MultiLog")
1. 打印日志
  -- 蓝图中打印日志
    1. ![image.png](https://cdn.nlark.com/yuque/0/2019/png/166447/1561686913406-de9264d3-99e2-46f0-83aa-86d77979bb5e.png#align=left&display=inline&height=268&name=image.png&originHeight=268&originWidth=540&size=39974&status=done&width=540)
  -- c++中打印日志
    - `#include "MultiLogSubsystem.h"`
    - `UMultiLogSubsystem::AddLog(FString(TEXT("test1")), EMultiLogLevel::DebugInfo, TEXT("%02d:%02d"), 1, 2);`
  -- 打印结果
    - ![image.png](https://cdn.nlark.com/yuque/0/2019/png/166447/1561687093389-79398c0c-1c7c-4df7-b8f5-bfe55b826703.png#align=left&display=inline&height=114&name=image.png&originHeight=114&originWidth=415&size=8611&status=done&width=415)
    - ![image.png](https://cdn.nlark.com/yuque/0/2019/png/166447/1561687126829-fc107869-0bee-4708-b218-3dc08c846588.png#align=left&display=inline&height=46&name=image.png&originHeight=46&originWidth=289&size=2605&status=done&width=289)
    - ![image.png](https://cdn.nlark.com/yuque/0/2019/png/166447/1561687146417-5c08918c-b815-4c37-a0f0-9d8e390932f4.png#align=left&display=inline&height=158&name=image.png&originHeight=158&originWidth=476&size=20814&status=done&width=476)
4. 控制台命令 ConsoleCommand
  - MultiLog.SetMultiLogLevel LogTypeName LogLevel
    - MultiLogLevel::Error = 1
    - MultiLogLevel::Warning = 2
    - MultiLogLevel::Info = 3
    - MultiLogLevel::DebugInfo = 4
   
