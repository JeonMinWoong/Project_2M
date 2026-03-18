// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/TwoMinBaseGameMode.h"

#include "TwoMinFunctionLibrary.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/TwoMinAttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/InventoryComponent.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/SoundManager.h"
#include "Managers/WorldStageManager.h"
#include "Spawner/SpawnMonsterPointGroup.h"
#include "Widgets/TwoMinWidget_ClearStageUI.h"
#include "Widgets/TwoMinWidget_DefeatStageUI.h"
#include "Widgets/GameMode/TwoMinWidget_ScreenFadeInOut.h"

void ATwoMinBaseGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* FindSpawnGroup = UGameplayStatics::GetActorOfClass(GetWorld(), ASpawnMonsterPointGroup::StaticClass());
	SpawnMonsterPointGroup = Cast<ASpawnMonsterPointGroup>(FindSpawnGroup);
	
	FSaveGameData LoadSaveGameData;
	if (UTwoMinFunctionLibrary::TryLoadGame(LoadSaveGameData))
	{
		// Load 성공 시 해야할 것 들.
		LoadSaveDataProcess(LoadSaveGameData);
		GetWorldTimerManager().SetTimerForNextTick(this, &ATwoMinBaseGameMode::AfterBeginPlay);
	}
	
	UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetGameInstance());
	if (!GI) return;
	
	if (!GI->bIsStageMoving) return;
	
	GI->bIsStageMoving = false;
	
	if (!FadeInOutWidgetClass) return;
	
	if (!FadeInOutWidget)
	{
		FadeInOutWidget = CreateWidget<UTwoMinWidget_ScreenFadeInOut>(GetWorld(), FadeInOutWidgetClass);	
	}
	
	FadeInOutWidget->AddToViewport();
	FadeInOutWidget->StartFadeIn();
	
	EBGMSoundType BGMSoundType = GI->StateManager->IsVillageMap() ? EBGMSoundType::Village : EBGMSoundType::Dungeon;
	BGMSoundType = GI->StateManager->IsDevelopMap() ? EBGMSoundType::None : BGMSoundType;
	GI->SoundManager->PlayBGMSound(BGMSoundType);
}

void ATwoMinBaseGameMode::LoadSaveDataProcess(FSaveGameData& LoadSaveGameData)
{
	bIsCompleteLoadSaveData = true;
	UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetGameInstance());
	if (!GI) return;

	for (const auto WorldStateData : LoadSaveGameData.WorldStageMap)
	{
		GI->StateManager->SetWorldStage(WorldStateData.Key, WorldStateData.Value);
	}
	
	const APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;
	
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(PC->GetPawn());
	if (!PlayerCharacter) return;
	
	UTwoMinAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
	if (!ASC) return;

	int32 TotalExperience = 0;
	for (int32 LevelStep = 1; LevelStep < LoadSaveGameData.PlayerLevel; ++LevelStep)
	{
		TotalExperience += UTwoMinAttributeSet::NeedToExperienceValue(PlayerCharacter, LevelStep);
	}
	
	TotalExperience += LoadSaveGameData.PlayerCurrentExp;
	ASC->GiveExperienceAmount(TotalExperience);
	ASC->GiveGoldAmount(LoadSaveGameData.PlayerCurrentGold);
	
	PlayerCharacter->GetInventoryComponent()->SetInventory(LoadSaveGameData.PlayerCurrentItems);
}

void ATwoMinBaseGameMode::AfterBeginPlay()
{
	bIsCompleteLoadSaveData = false;
}

void ATwoMinBaseGameMode::OpenStageProcess(const FName StageName, bool bIsSaveData)
{
	if (IsOpeningStage()) return;

	if (!FadeInOutWidgetClass) return;
	
	if (!FadeInOutWidget)
	{
		FadeInOutWidget = CreateWidget<UTwoMinWidget_ScreenFadeInOut>(GetWorld(), FadeInOutWidgetClass);	
	}
	
	FadeInOutWidget->AddToViewport(1000);
	FadeInOutWidget->StartFadeOut(StageName);
	
	if (bIsSaveData)
	{
		FSaveGameData NewSaveGameData;
		CreateNewSaveGameData(NewSaveGameData);
	
		UTwoMinFunctionLibrary::SaveGame(NewSaveGameData);	
	}
}

void ATwoMinBaseGameMode::CreateNewSaveGameData(FSaveGameData& NewSaveGameData) const
{
	UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetGameInstance());
	if (!GI) return;
	
	NewSaveGameData.WorldStageMap = GI->StateManager->GetWorldStageMap();
	const APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;
	
	const ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(PC->GetPawn());
	if (!PlayerCharacter) return;
	
	const UTwoMinAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
	if (!ASC) return;
	
	NewSaveGameData.PlayerLevel = ASC->GetNumericAttribute(UTwoMinAttributeSet::GetCurrentLevelAttribute());
	NewSaveGameData.PlayerCurrentExp = ASC->GetNumericAttribute(UTwoMinAttributeSet::GetCurrentExperienceAttribute());
	NewSaveGameData.PlayerCurrentGold = ASC->GetNumericAttribute(UTwoMinAttributeSet::GetCurrentGoldAttribute());
	NewSaveGameData.PlayerCurrentItems = PlayerCharacter->GetInventoryComponent()->GetInventory();
}

bool ATwoMinBaseGameMode::IsOpeningStage() const
{
	return FadeInOutWidget && FadeInOutWidget->IsPlayingFadeOut();
}

void ATwoMinBaseGameMode::ShowClearStageUI()
{
	if (!ClearStageWidgetClass) return;
	
	if (!ClearStageWidget)
	{
		ClearStageWidget = CreateWidget<UTwoMinWidget_ClearStageUI>(GetWorld(), ClearStageWidgetClass);	
	}
	
	ClearStageWidget->AddToViewport();
	ClearStageWidget->SettingClearStageUI();
	
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0);
}

void ATwoMinBaseGameMode::ShowDefeatStageUI()
{
	if (!DefeatStageWidgetClass) return;
	
	if (!DefeatStageWidget)
	{
		DefeatStageWidget = CreateWidget<UTwoMinWidget_DefeatStageUI>(GetWorld(), DefeatStageWidgetClass);	
	}
	
	DefeatStageWidget->AddToViewport();
	DefeatStageWidget->SettingDefeatStageUI();
	
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0);
}

void ATwoMinBaseGameMode::LockPlayerInput(bool bLock, UTwoMinWidgetBase* InWidget) const
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;
	
	PC->SetIgnoreMoveInput(bLock);
	PC->SetIgnoreLookInput(bLock);
	if (bLock)
	{
		PC->SetInputMode(FInputModeUIOnly().SetWidgetToFocus(InWidget->TakeWidget()));
	}
	else
	{
		PC->SetInputMode(FInputModeGameOnly());
	}
}
