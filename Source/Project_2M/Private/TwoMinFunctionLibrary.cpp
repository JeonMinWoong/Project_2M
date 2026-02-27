// Fill out your copyright notice in the Description page of Project Settings.


#include "TwoMinFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "TwoMinDebugHelper.h"
#include "TwoMinGameplayTag.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "Character/TwoMinBaseCharacter.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "GameFramework/InputSettings.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "GameModes/TwoMinBaseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/WorldStageManager.h"
#include "SaveGame/TwoMinSaveGame.h"

bool UTwoMinFunctionLibrary::IsUsingGamePad(const UWorld* World, FPlatformUserId UserId)
{
	if (!World) return false;

	const UGameInstance* GI = World->GetGameInstance();
	if (!GI) return false;
	
	const UInputDeviceSubsystem* InputDeviceSubsystem =
		GI->GetEngine()->GetEngineSubsystem<UInputDeviceSubsystem>();
	
	if (InputDeviceSubsystem)
	{
		FHardwareDeviceIdentifier MostRecentDevice = InputDeviceSubsystem->GetMostRecentlyUsedHardwareDevice(UserId);
		if (MostRecentDevice.PrimaryDeviceType == EHardwareDevicePrimaryType::Gamepad)
		{
			return true;
		}
	}
	
	return false;
}

bool UTwoMinFunctionLibrary::IsTargetPawnHostile(APawn* OwnerPawn, APawn* TargetPawn)
{
	IGenericTeamAgentInterface* OwnerTeamAgent = Cast<IGenericTeamAgentInterface>(OwnerPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (OwnerTeamAgent && TargetTeamAgent)
	{
		return OwnerTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}
	
	return false;
}

bool UTwoMinFunctionLibrary::HasGameplayTag(AActor* InActor, FGameplayTag InTag)
{
	if (!InActor) return false;

	UTwoMinAbilitySystemComponent* ASC =
		Cast<UTwoMinAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
	if (ASC)
	{
		return ASC->HasMatchingGameplayTag(InTag);
	}

	return false;
	
}

void UTwoMinFunctionLibrary::SendToGameplayEffectEvent(AActor* InActor, FGameplayTag InEventTag,
	const FGameplayEventData& InEventData)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		InActor,
		InEventTag,
		InEventData
	);
}

void UTwoMinFunctionLibrary::AddGameplayTagToActor(AActor* InActor, FGameplayTag TagToAdd)
{
	if (!InActor) return;
	if (!InActor->IsValidLowLevel()) return;
	
	ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(InActor);
	if (!MyCharacter) return;
	
	UTwoMinAbilitySystemComponent* Asc = MyCharacter->GetAbilitySystemComponent();
	if (!Asc) return;
	
	if (!Asc->HasMatchingGameplayTag(TagToAdd))
	{
		Asc->AddLooseGameplayTag(TagToAdd);
	}
}

void UTwoMinFunctionLibrary::RemoveGameplayTagToActor(AActor* InActor, FGameplayTag TagToAdd)
{
	if (!InActor) return;
	if (!InActor->IsValidLowLevel()) return;
	
	ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(InActor);
	if (!MyCharacter) return;
	
	UTwoMinAbilitySystemComponent* Asc = MyCharacter->GetAbilitySystemComponent();
	if (!Asc) return;
	
	if (Asc->HasMatchingGameplayTag(TagToAdd))
	{
		Asc->RemoveLooseGameplayTag(TagToAdd);
	}
}

float UTwoMinFunctionLibrary::AttackTypeChangeToAmount(EAttackType AttackType)
{
	switch (AttackType)
	{
	case EAttackType::Light:
		return 10;
	case EAttackType::Heavy:
		return 20;
	default:
		return 0;
	}
}

EItemType UTwoMinFunctionLibrary::GetItemType(int32 ItemID)
{
	int32 Value = ItemID;

	while (Value >= 10)
	{
		Value /= 10;
	}
	
	if (Value < static_cast<int8>(EItemType::Consume)) return EItemType::Equipment;
	if (Value < static_cast<int8>(EItemType::Etc)) return EItemType::Consume;
	if (Value < static_cast<int8>(EItemType::Unknown)) return EItemType::Etc;

	return EItemType::None;
}

constexpr float EPS = 0.0001f;
bool UTwoMinFunctionLibrary::IsNearFloatZero(float Value)
{
	return FMath::IsNearlyZero(Value, EPS);
}

bool UTwoMinFunctionLibrary::IsNearFloatEqual(float Value, float EqualValue)
{
	return FMath::IsNearlyEqual(Value, EqualValue);
}

bool UTwoMinFunctionLibrary::IsVillageMap(const UWorld* World)
{
	if (!World) return false;

	const UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(World->GetGameInstance());
	if (!GI) return false;
	
	return GI->StateManager->IsVillageMap();
}

void UTwoMinFunctionLibrary::SaveGame(const FSaveGameData& NewSaveGameData)
{
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(UTwoMinSaveGame::StaticClass());
	UTwoMinSaveGame* TwoMinSaveGame = Cast<UTwoMinSaveGame>(SaveGameObject);
	if (!TwoMinSaveGame) return;
	
	TwoMinSaveGame->SaveGameData(NewSaveGameData);
	
	const bool bWasSaved = UGameplayStatics::SaveGameToSlot(
		TwoMinSaveGame,
		TwoMinGameplayTag::Data_SaveGame_Slot_1.GetTag().ToString(),
		0
	);

	const FString SaveStr = bWasSaved ? TEXT("저장 완료.") : TEXT("저장 실패.");
	TwoMinDebugHelper::Print(SaveStr, FColor::Green);
}

bool UTwoMinFunctionLibrary::TryLoadGame(FSaveGameData& OutSaveGameData)
{
	if (IsExistSaveGameData() == false) return false;
	
	const FString SlotName = TwoMinGameplayTag::Data_SaveGame_Slot_1.GetTag().ToString();
	USaveGame* SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, 0);
	UTwoMinSaveGame* TwoMinSaveGame = Cast<UTwoMinSaveGame>(SaveGameObject);
	if (!TwoMinSaveGame) return false;
	
	OutSaveGameData = TwoMinSaveGame->LoadGameData();

	if (OutSaveGameData.PlayerLevel <= 0)
	{
		TwoMinDebugHelper::Print(TEXT("불러오기 실패"), FColor::Red);
		return false;
	}
	
	TwoMinDebugHelper::Print(TEXT("불러오기 성공"), FColor::Green);
	return true;
}

bool UTwoMinFunctionLibrary::IsExistSaveGameData()
{
	const FString SlotName = TwoMinGameplayTag::Data_SaveGame_Slot_1.GetTag().ToString();
	const bool IsFindSaveData = UGameplayStatics::DoesSaveGameExist(SlotName, 0);
	
	return IsFindSaveData;
}

void UTwoMinFunctionLibrary::RemoveSaveGameData()
{
	if (IsExistSaveGameData() == false) return;
	
	const FString SlotName = TwoMinGameplayTag::Data_SaveGame_Slot_1.GetTag().ToString();
	UGameplayStatics::DeleteGameInSlot(SlotName, 0);
}

bool UTwoMinFunctionLibrary::IsLoadingData(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return false;

	const UWorld* World = WorldContextObject->GetWorld();
	if (!World) return false;

	const ATwoMinBaseGameMode* GM = Cast<ATwoMinBaseGameMode>(UGameplayStatics::GetGameMode(World));
	if (!GM) return false;
	
	return GM->IsLoadData();
}
