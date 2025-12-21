// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_BuffSlot.h"

#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Components/Image.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/ItemDataManager.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "Widgets/TwoMinWidget_BuffWindow.h"

void UTwoMinWidget_BuffSlot::NativeConstruct()
{
	Super::NativeConstruct();
	
	BuffIconImage->SetVisibility(ESlateVisibility::Hidden);
	bHasScriptImplementedTick = true;
}

void UTwoMinWidget_BuffSlot::Init(UTwoMinWidget_BuffWindow* NewBuffWindow, int32 InSlotIndex)
{
	BuffWindow = NewBuffWindow;
	SlotIndex = InSlotIndex;
}

void UTwoMinWidget_BuffSlot::OnBuff(int32 ItemID)
{
	UTwoMinGameInstance* GI = GetWorld()->GetGameInstance<UTwoMinGameInstance>();
	if (!GI) return;
	
	const FItemConsumeData ConsumeData = GI->ItemDataManager->GetItemConsumeData(ItemID);
	
	bIsOnBuff = true;
	BuffItemID = ItemID;
	BuffIconImage->SetVisibility(ESlateVisibility::Visible);
	CachedMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(BuffIconMaterialInstance, this);
	CachedMaterialInstanceDynamic->SetTextureParameterValue(FName("SkillIcon"), ConsumeData.ItemDataBase.ItemTexture);
	BuffIconImage->SetBrushFromMaterial(CachedMaterialInstanceDynamic);
	
	CurrentTime = 0;
	BuffDuration = ConsumeData.BuffDuration;
}

void UTwoMinWidget_BuffSlot::OffBuff()
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn());
	if (!PlayerCharacter) return;
	
	UTwoMinAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
	if (!ASC) return;
	
	ASC->RemoveConsumeBuff(BuffItemID);
	
	bIsOnBuff = false;
	BuffItemID = 0;
	BuffIconImage->SetVisibility(ESlateVisibility::Hidden);
	CurrentTime = 0;
	BuffDuration = 0;
	
	BuffWindow->SortBuffSlots(SlotIndex);
}

void UTwoMinWidget_BuffSlot::OnCopyBuff(int32 ItemID, float RunningTime)
{
	OnBuff(ItemID);
	CurrentTime = RunningTime;
}

void UTwoMinWidget_BuffSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (bIsOnBuff == false) return;
	
	float GlobalTime = UGameplayStatics::GetGlobalTimeDilation(GetWorld());
	if (UTwoMinFunctionLibrary::IsNearFloatZero(GlobalTime)) return;
	
	// 0 이면 무제한.
	if (BuffDuration <= 0) return;
	
	CurrentTime += InDeltaTime;
	if (CurrentTime >= BuffDuration)
	{
		OffBuff();
		return;
	}
	
	// Update Buff Progress
	if (!CachedMaterialInstanceDynamic) return;
	
	CachedMaterialInstanceDynamic->SetScalarParameterValue(FName("CoolDownRemainingPercent"), 
		1 - CurrentTime / BuffDuration);
}
