// Fill out your copyright notice in the Description page of Project Settings.


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
	UMultiLogSubsystem::AddLog(FString(TEXT("test1")), MultiLogLevel::DebugInfo, TEXT("%02d:%02d"), 1, 2);
	UMultiLogSubsystem::AddLog(FString(TEXT("test1")), MultiLogLevel::Error, TEXT("%02d:%02d"), 1, 2);
	UMultiLogSubsystem::AddLog(FString(TEXT("test1")), MultiLogLevel::Info, TEXT("%02d:%02d"), 1, 2);
	UMultiLogSubsystem::AddLog(FString(TEXT("test1")), MultiLogLevel::Warning, TEXT("%02d:%02d"), 1, 2);

	UMultiLogSubsystem::AddLog(FString(TEXT("test2")), MultiLogLevel::DebugInfo, TEXT("%02d:%02d"), 1, 2);
	UMultiLogSubsystem::AddLog(FString(TEXT("test2")), MultiLogLevel::Error, TEXT("%02d:%02d"), 1, 2);
	UMultiLogSubsystem::AddLog(FString(TEXT("test2")), MultiLogLevel::Info, TEXT("%02d:%02d"), 1, 2);
	UMultiLogSubsystem::AddLog(FString(TEXT("test2")), MultiLogLevel::Warning, TEXT("%02d:%02d"), 1, 2);

	UMultiLogSubsystem::AddLog(FString(TEXT("test3")), MultiLogLevel::DebugInfo, TEXT("%02d:%02d"), 1, 2);
	UMultiLogSubsystem::AddLog(FString(TEXT("test3")), MultiLogLevel::Error, TEXT("%02d:%02d"), 1, 2);
	UMultiLogSubsystem::AddLog(FString(TEXT("test3")), MultiLogLevel::Info, TEXT("%02d:%02d"), 1, 2);
	UMultiLogSubsystem::AddLog(FString(TEXT("test3")), MultiLogLevel::Warning, TEXT("%02d:%02d"), 1, 2);

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

