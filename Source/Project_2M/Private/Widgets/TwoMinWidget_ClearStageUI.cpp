
#include "Widgets/TwoMinWidget_ClearStageUI.h"

#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "GameModes/TwoMinBaseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/ItemDataManager.h"
#include "Managers/WorldStageManager.h"
#include "Widgets/TwoMinWidget_BaseButton.h"
#include "Widgets/TwoMinWidget_ClearItemSlot.h"

void UTwoMinWidget_ClearStageUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (!ClearItemGrid) return;
	
	TArray<UWidget*> ItemSlotGroup = ClearItemGrid->GetAllChildren();
	if (ItemSlotGroup.IsEmpty()) return;

	for (UWidget* ClearItemSlot : ItemSlotGroup)
	{
		UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(ClearItemSlot->Slot);
		if (!GridSlot) continue;
		
		if (UTwoMinWidget_ClearItemSlot* CastSlot = Cast<UTwoMinWidget_ClearItemSlot>(ClearItemSlot))
		{
			CastSlot->InitSlot();
			ClearItemSlots.Add(CastSlot);
		}
	}
	
	MaxItemSlotIndex = ClearItemSlots.Num();
	bIsGoingVillage = false;
	
	if (OnOpenClearStageAnim)
	{
		CompleteOpenClearStageAnimEvent.BindDynamic(this, &UTwoMinWidget_ClearStageUI::PlayClearCountAnim);
		BindToAnimationFinished(OnOpenClearStageAnim, CompleteOpenClearStageAnimEvent);
	}
}

FReply UTwoMinWidget_ClearStageUI::NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey InKey = InKeyEvent.GetKey();
	if (InKey == EKeys::Enter || InKey == EKeys::Gamepad_FaceButton_Bottom)
	{
		if (OnReturnButton() == false) return FReply::Unhandled();
		
		return FReply::Handled();
	}
	
	if (InKey == EKeys::P || InKey == EKeys::Gamepad_FaceButton_Right)
	{
		return FReply::Unhandled();
	}
	
	if (InKey == EKeys::Right || InKey == EKeys::D || InKey == EKeys::Gamepad_LeftStick_Right)
	{
		return FReply::Unhandled();
	}
	
	if (InKey == EKeys::Left || InKey == EKeys::A || InKey == EKeys::Gamepad_LeftStick_Left)
	{
		return FReply::Unhandled();
	}
	
	if (InKey == EKeys::Up || InKey == EKeys::W || InKey == EKeys::Gamepad_LeftStick_Up)
	{
		return FReply::Unhandled();
	}
	
	if (InKey == EKeys::Down || InKey == EKeys::S || InKey == EKeys::Gamepad_LeftStick_Down)
	{
		return FReply::Unhandled();
	}
	
	return Super::NativeOnPreviewKeyDown(MyGeometry, InKeyEvent);
}

void UTwoMinWidget_ClearStageUI::SettingClearStageUI()
{
	UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetGameInstance());
	if (!GI) return;
	
	FString CurRealStageName = GetWorld()->RemovePIEPrefix(GetWorld()->GetMapName());
	int32 Index = GI->StateManager->GetWorldStageIndex(CurRealStageName);
	if (Index < 0) return;

	const FString ShowStageName = GI->StateManager->GetIndexShowStageName(Index);
	ClearStageTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%s 토벌 완료."), *ShowStageName)));
	
	FWorldStageClearGainData* ClearData = GI->StateManager->GetCurrentWorldClearStageData(CurRealStageName);
	if (!ClearData) return;

	int32 GainGold = ClearData->GainGold;
	const FString GainGoldStr = FText::AsNumber(GainGold).ToString();
	FString Str = FString::Printf(TEXT("<ClearGold>+ %s</><img id=\"ClearGoldImage\"/>"), *GainGoldStr);
	GainGoldTextBlock->SetText(FText::FromString(Str));

	int32 GainExp = ClearData->GainExp;
	const FString GainExpStr = FText::AsNumber(GainExp).ToString();
	GainExpTextBlock->SetText(FText::FromString(FString::Printf(TEXT("EXP +%s"), *GainExpStr)));
	
	TMap<int32, int32> ClearRewordItem = GI->ItemDataManager->TryGetDropItems(CurRealStageName);
	if (ClearRewordItem.IsEmpty()) return;
	
	TArray<FItemEquipmentData> EquipmentList;
	TArray<FItemConsumeData> ConsumeList;
	TArray<FItemEtcData> EtcList;
	for (auto RewordItem : ClearRewordItem)
	{
		GI->ItemDataManager->GetDropItemList(RewordItem, EquipmentList, ConsumeList, EtcList);
	}
	
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!Character) return;
	
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(Character);
	if (!PlayerCharacter) return;
	
	UTwoMinAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
	if (!ASC) return;
	
	LockPlayerInput(true);
	PlayClearStageAnim();
	
	ASC->GiveGoldAmount(GainGold);
	ASC->GiveExperienceAmount(GainExp);
	GI->ItemDataManager->GiveToInventory(PlayerCharacter, EquipmentList, ConsumeList, EtcList, true);
	ShowItemSlots(EquipmentList, ConsumeList, EtcList);
}

void UTwoMinWidget_ClearStageUI::ShowItemSlots(TArray<FItemEquipmentData> InItemEquipmentList, 
	TArray<FItemConsumeData> InItemConsumeList, TArray<FItemEtcData> InItemEtcList)
{
	for (int32 EquipmentIndex = 0; EquipmentIndex < InItemEquipmentList.Num(); ++EquipmentIndex)
	{
		if (CurItemSlotIndex >= MaxItemSlotIndex) return;
		
		ClearItemSlots[CurItemSlotIndex]->SetClearItemSlotInfo(InItemEquipmentList[EquipmentIndex].ItemDataBase);
		CurItemSlotIndex++;
	}
	
	for (int32 ConsumeIndex = 0; ConsumeIndex < InItemConsumeList.Num(); ++ConsumeIndex)
	{
		if (CurItemSlotIndex >= MaxItemSlotIndex) return;
		
		ClearItemSlots[CurItemSlotIndex]->SetClearItemSlotInfo(InItemConsumeList[ConsumeIndex].ItemDataBase);
		CurItemSlotIndex++;
	}
	
	for (int32 EtcIndex = 0; EtcIndex < InItemEtcList.Num(); ++EtcIndex)
	{
		if (CurItemSlotIndex >= MaxItemSlotIndex) return;
		
		ClearItemSlots[CurItemSlotIndex]->SetClearItemSlotInfo(InItemEtcList[EtcIndex].ItemDataBase);
		CurItemSlotIndex++;
	}
}

void UTwoMinWidget_ClearStageUI::PlayClearStageAnim()
{
	if (IsAnimationPlaying(OnOpenClearStageAnim))
	{
		StopAnimation(OnOpenClearStageAnim);
	}
	
	PlayAnimation(OnOpenClearStageAnim);
}

void UTwoMinWidget_ClearStageUI::PlayClearCountAnim()
{
	if (IsAnimationPlaying(OnStartClearCountAnim))
	{
		StopAnimation(OnStartClearCountAnim);
	}
	
	PlayAnimation(OnStartClearCountAnim);
	ReturnButton->SetFocus();
}

void UTwoMinWidget_ClearStageUI::Anim_OnUpdateClearCount()
{
	if (bIsGoingVillage) return;
	
	const int32 CountValue = MaxClearCountTime - CurClearCountTime++;
	const FString FinalStr = FString::Printf(TEXT("%d초 후 마을로 자동 이동"), CountValue);
	CountTextBlock->SetText(FText::FromString(FinalStr));
	
	FinishClearCountAnim();
}

void UTwoMinWidget_ClearStageUI::FinishClearCountAnim()
{
	if (MaxClearCountTime >= CurClearCountTime) return;
	
	if (IsAnimationPlaying(OnStartClearCountAnim))
	{
		StopAnimation(OnStartClearCountAnim);
	}
	
	OnReturnButton();
}

bool UTwoMinWidget_ClearStageUI::OnReturnButton()
{
	if (bIsGoingVillage) return false;
	
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
	
	bIsGoingVillage = true;
	GM->OpenStageProcess(GoStageName);
	return true;
}

void UTwoMinWidget_ClearStageUI::LockPlayerInput(const bool bLock)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;
	
	PC->SetIgnoreMoveInput(bLock);
	PC->SetIgnoreLookInput(bLock);
	if (bLock)
	{
		PC->SetInputMode(FInputModeUIOnly().SetWidgetToFocus(TakeWidget()));
	}
	else
	{
		PC->SetInputMode(FInputModeGameOnly());
	}
}
