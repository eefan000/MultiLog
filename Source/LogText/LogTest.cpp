#include "LogTest.h"
#include "MultiLogSubsystem.h"

// Sets default values
ALogTest::ALogTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALogTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALogTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	static FName test1(TEXT("test1"));
	UMultiLogSubsystem::AddLog(test1, EMultiLogLevel::DebugInfo, TEXT("%02d:%02d"), 1, 2);
	UMultiLogSubsystem::AddLog(test1, EMultiLogLevel::Error, TEXT("%02d:%02d"), 1, 2);
	UMultiLogSubsystem::AddLog(test1, EMultiLogLevel::Info, TEXT("%02d:%02d"), 1, 2);
	UMultiLogSubsystem::AddLog(test1, EMultiLogLevel::Warning, TEXT("%02d:%02d"), 1, 2);

	static FName test2(TEXT("test2"));
	UMultiLogSubsystem::AddLog(test2, EMultiLogLevel::DebugInfo, TEXT("%02d:%02d"), 1, 2);
	UMultiLogSubsystem::AddLog(test2, EMultiLogLevel::Error, TEXT("%02d:%02d"), 1, 2);
	UMultiLogSubsystem::AddLog(test2, EMultiLogLevel::Info, TEXT("%02d:%02d"), 1, 2);
	UMultiLogSubsystem::AddLog(test2, EMultiLogLevel::Warning, TEXT("%02d:%02d"), 1, 2);

	static FName test3(TEXT("test3"));
	UMultiLogSubsystem::AddLog(test3, EMultiLogLevel::DebugInfo, TEXT("%02d:%02d"), 1, 2);
	UMultiLogSubsystem::AddLog(test3, EMultiLogLevel::Error, TEXT("%02d:%02d"), 1, 2);
	UMultiLogSubsystem::AddLog(test3, EMultiLogLevel::Info, TEXT("%02d:%02d"), 1, 2);
	UMultiLogSubsystem::AddLog(test3, EMultiLogLevel::Warning, TEXT("%02d:%02d"), 1, 2);

	/*UE_LOG(LogTemp, Log, TEXT("%02d:%02d"), 1, 2);
	UE_LOG(LogTemp, Log, TEXT("%02d:%02d"), 1, 2);
	UE_LOG(LogTemp, Log, TEXT("%02d:%02d"), 1, 2);
	UE_LOG(LogTemp, Log, TEXT("%02d:%02d"), 1, 2);

	UE_LOG(LogTemp, Log, TEXT("%02d:%02d"), 1, 2);
	UE_LOG(LogTemp, Log, TEXT("%02d:%02d"), 1, 2);
	UE_LOG(LogTemp, Log, TEXT("%02d:%02d"), 1, 2);
	UE_LOG(LogTemp, Log, TEXT("%02d:%02d"), 1, 2);

	UE_LOG(LogTemp, Log, TEXT("%02d:%02d"), 1, 2);
	UE_LOG(LogTemp, Log, TEXT("%02d:%02d"), 1, 2);
	UE_LOG(LogTemp, Log, TEXT("%02d:%02d"), 1, 2);
	UE_LOG(LogTemp, Log, TEXT("%02d:%02d"), 1, 2);*/
}

