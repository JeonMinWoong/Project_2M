// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_DefeatStageUI.h"

#include "Components/TextBlock.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "GameModes/TwoMinBaseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/WorldStageManager.h"
#include "Widgets/TwoMinWidget_BaseButton.h"

void UTwoMinWidget_DefeatStageUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (OnOpenDefeatStageAnim)
	{
		CompleteOpenDefeatStageAnimEvent.BindDynamic(this, &UTwoMinWidget_DefeatStageUI::PlayDefeatCountAnim);
		BindToAnimationFinished(OnOpenDefeatStageAnim, CompleteOpenDefeatStageAnimEvent);
	}
	
	CheckDefeatType = EDefeatType::None;
	CurFocusIndex = 0;
}

FReply UTwoMinWidget_DefeatStageUI::NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey InKey = InKeyEvent.GetKey();
	if (InKey == EKeys::Enter || InKey == EKeys::F || InKey == EKeys::Gamepad_FaceButton_Bottom)
	{
		if (CurFocusIndex == 0)
		{
			if (OnReturnButton() == false) return FReply::Unhandled();	
		}
		else if (CurFocusIndex == 1)
		{
			if (OnRetryButton() == false) return FReply::Unhandled();
		}
		
		return FReply::Handled();
	}
	
	if (InKey == EKeys::Right || InKey == EKeys::D || InKey == EKeys::Gamepad_LeftStick_Right)
	{
		if (IsButtonDown()) return FReply::Unhandled();
		
		int32 NextFocusIndex = CurFocusIndex + 1;
		if (NextFocusIndex > 1) return FReply::Unhandled();
		
		CurFocusIndex++;
		RetryButton->SetFocus();
		
		return FReply::Unhandled();
	}
	
	if (InKey == EKeys::Left || InKey == EKeys::A || InKey == EKeys::Gamepad_LeftStick_Left)
	{
		if (IsButtonDown()) return FReply::Unhandled();
		
		int32 NextFocusIndex = CurFocusIndex - 1;
		if (NextFocusIndex < 0) return FReply::Unhandled();
		
		CurFocusIndex--;
		ReturnButton->SetFocus();
		
		return FReply::Unhandled();
	}
	
	return Super::NativeOnPreviewKeyDown(MyGeometry, InKeyEvent);
}


void UTwoMinWidget_DefeatStageUI::SettingDefeatStageUI()
{
	ATwoMinBaseGameMode* GM = Cast<ATwoMinBaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GM) return;
	
	GM->LockPlayerInput(true, this);
	
	PlayDefeatStageAnim();
}

void UTwoMinWidget_DefeatStageUI::PlayDefeatStageAnim()
{
	if (IsAnimationPlaying(OnOpenDefeatStageAnim))
	{
		StopAnimation(OnOpenDefeatStageAnim);
	}
	
	PlayAnimation(OnOpenDefeatStageAnim);
}

void UTwoMinWidget_DefeatStageUI::PlayDefeatCountAnim()
{
	if (IsAnimationPlaying(OnStartDefeatCountAnim))
	{
		StopAnimation(OnStartDefeatCountAnim);
	}
	
	PlayAnimation(OnStartDefeatCountAnim);
	ReturnButton->SetFocus();
}

void UTwoMinWidget_DefeatStageUI::Anim_OnUpdateDefeatCount()
{
	if (IsButtonDown()) return;
	
	const int32 CountValue = MaxClearCountTime - CurClearCountTime++;
	const FString FinalStr = FString::Printf(TEXT("%d초 후 마을로 자동 이동"), CountValue);
	CountTextBlock->SetText(FText::FromString(FinalStr));
	
	FinishDefeatCountAnim();
}

void UTwoMinWidget_DefeatStageUI::FinishDefeatCountAnim()
{
	if (MaxClearCountTime >= CurClearCountTime) return;
	
	if (IsAnimationPlaying(OnStartDefeatCountAnim))
	{
		StopAnimation(OnStartDefeatCountAnim);
	}
	
	OnReturnButton();
}

bool UTwoMinWidget_DefeatStageUI::OnReturnButton()
{
	if (IsButtonDown()) return false;
	
	ATwoMinBaseGameMode* GM = Cast<ATwoMinBaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GM) return false;
	
	UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetGameInstance());
	if (!GI) return false;

	FString CurRealStageName = GetWorld()->RemovePIEPrefix(GetWorld()->GetMapName());

	const int32 CurStageIndex = GI->StateManager->GetWorldStageIndex(CurRealStageName);
	FName GoStageName = FName(*GI->StateManager->GetIndexRealStageName(0));
	if (CurStageIndex != 0)
	{
		GoStageName = FName(*GI->StateManager->GetVillageName());
	}
	
	CheckDefeatType = EDefeatType::Return;
	GM->OpenStageProcess(GoStageName);
	return true;
}

bool UTwoMinWidget_DefeatStageUI::OnRetryButton()
{
	if (IsButtonDown()) return false;
	
	ATwoMinBaseGameMode* GM = Cast<ATwoMinBaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GM) return false;
	
	UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetGameInstance());
	if (!GI) return false;

	FString CurRealStageName = GetWorld()->RemovePIEPrefix(GetWorld()->GetMapName());
	if (GI->StateManager->GetWorldStage(CurRealStageName) == false)
	{
		if (GI->StateManager->GetWorldStageIndex(CurRealStageName) == -1)
		{
			return false;	
		}
	}
	CheckDefeatType = EDefeatType::Retry;
	GM->OpenStageProcess(FName(CurRealStageName));
	return true;
}

bool UTwoMinWidget_DefeatStageUI::IsButtonDown() const
{
	return CheckDefeatType != EDefeatType::None;
}
