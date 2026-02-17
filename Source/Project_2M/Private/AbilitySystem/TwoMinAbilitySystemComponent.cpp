// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TwoMinAbilitySystemComponent.h"

#include "IDetailTreeNode.h"
#include "TwoMinDebugHelper.h"
#include "TwoMinGameplayTag.h"
#include "AbilitySystem/TwoMinAttributeSet.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "Character/TwoMinEnemyDummy.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/InventoryComponent.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "Managers/ItemDataManager.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "Widgets/TwoMinWidget_InventoryUI.h"

class UTwoMinGameInstance;

void UTwoMinAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	if (CancelAndStartNextNewAction(InInputTag)) return;
	
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag)) continue;

		if (InInputTag.MatchesTag(TwoMinGameplayTag::InputTag_Toggle) && AbilitySpec.IsActive())
		{
			CancelAbilityHandle(AbilitySpec.Handle);
		}
		else
		{
			UTwoMinGameplayAbility* Ability = Cast<UTwoMinGameplayAbility>(AbilitySpec.Ability);
			FGameplayTag AbilityTag = GetActiveAbilityTag(Ability->AbilityTags);
			if (IsPlayingAbility(AbilityTag))
			{
				OnCancelAndReTriggerAbility(Ability, Ability->GetClass(), AbilityTag);
			}
			else
			{
				if (Ability->IsMustBeLikedToGameplayAbility()) return;
				
				TryActivateAbility(AbilitySpec.Handle);	
			}
		}
	}
}

void UTwoMinAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid() || !InInputTag.MatchesTag(TwoMinGameplayTag::InputTag_MustBeHold)) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag) && AbilitySpec.IsActive())
		{
			UTwoMinGameplayAbility* Ability = Cast<UTwoMinGameplayAbility>(AbilitySpec.Ability);
			if (Ability->IsPossibleMustBeHoldAbilityImmediatelyCancel())
			{
				CancelAbilityHandle(AbilitySpec.Handle);	
			}

			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

bool UTwoMinAbilitySystemComponent::CancelAndStartNextNewAction(const FGameplayTag& InInputTag)
{
	if (UTwoMinGameplayAbility* PlayingAbility = GetPlayingAbility())
	{
		UTwoMinGameplayAbility* NewAbility = GetActiveAbilityInputTag(InInputTag);
		if (PlayingAbility->IsPossibleCancelAbility(NewAbility))
		{
			UTwoMinGameplayAbility* CDOAbility =
				Cast<UTwoMinGameplayAbility>(PlayingAbility->GetClass()->GetDefaultObject());
			PlayingAbility->CustomCancelAbility();
			CancelAbility(CDOAbility);

			FTimerDelegate Delegate;
			Delegate.BindLambda([this, NewAbility]()
			{
				this->TryActivateAbilityByClass(NewAbility->GetClass());
			});

			this->GetWorld()->GetTimerManager().SetTimerForNextTick(Delegate);
			return true;
		}
	}
	
	return false;
}

void UTwoMinAbilitySystemComponent::OnCancelAndReTriggerAbility(UTwoMinGameplayAbility* InAbilityCDO,
	const TSubclassOf<UTwoMinGameplayAbility> InAbilityToReTrigger, const FGameplayTag& AbilityTag)
{
 	if (!InAbilityToReTrigger)
	{
		return;
	}

	UTwoMinGameplayAbility* ReTriggerAbility = GetPlayingAbilityTag(AbilityTag);
	if (ReTriggerAbility->GetAbilityInputType() == ETwoAbilityInputType::Only)
	{
		return;
	}
	
	if (!ReTriggerAbility->IsReTriggerActive())
	{
		return;
	}
	
	CancelAbility(Cast<UGameplayAbility>(InAbilityCDO));

	FTimerDelegate Delegate;
	Delegate.BindLambda([this, InAbilityToReTrigger]()
	{
		this->TryActivateAbilityByClass(InAbilityToReTrigger);
	});

	this->GetWorld()->GetTimerManager().SetTimerForNextTick(Delegate);
}

void UTwoMinAbilitySystemComponent::GrantHeroWeaponAbilities(
	const TArray<FTwoMinPlayerAbilitySet>& InDefaultWeaponAbilities, int ApplyLevel,
	TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (InDefaultWeaponAbilities.IsEmpty()) return;

	for (const FTwoMinPlayerAbilitySet& AbilitySet : InDefaultWeaponAbilities)
	{
		if (!AbilitySet.IsValid()) continue;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);
		
		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}
}

UTwoMinGameplayAbility* UTwoMinAbilitySystemComponent::GetPlayingAbility()
{
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.IsActive() && AbilitySpec.Ability)
		{
			// CDO 가 없어서 정지가 안되는 듯.
			if (UTwoMinGameplayAbility* TwoMinGameplayAbility = Cast<UTwoMinGameplayAbility>(AbilitySpec.GetPrimaryInstance()))
			{
				if (TwoMinGameplayAbility->GetActivationPolicy() != EToMinAbilityActivationPolicy::OnGiven)
				{
					return TwoMinGameplayAbility;
				}
			}
		}
	}

	return nullptr;
}

bool UTwoMinAbilitySystemComponent::IsPlayingAbility(const FGameplayTag& AbilityTag)
{
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability->AbilityTags.HasTagExact(AbilityTag) == false)
		{
			continue;
		}
		
		if (AbilitySpec.IsActive() && AbilitySpec.Ability)
		{
			return true;
		}
	}
	
	return false;
}

UTwoMinGameplayAbility* UTwoMinAbilitySystemComponent::GetPlayingAbilityTag(const FGameplayTag& AbilityTag)
{
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability->AbilityTags.HasTagExact(AbilityTag) == false)
		{
			continue;
		}
		
		if (AbilitySpec.IsActive() && AbilitySpec.Ability)
		{
			if (UTwoMinGameplayAbility* FindAbility = Cast<UTwoMinGameplayAbility>(AbilitySpec.GetPrimaryInstance()))
			{
				return FindAbility;	
			}
		}
	}
	
	return nullptr;
}

FGameplayTag UTwoMinAbilitySystemComponent::GetActiveAbilityTag(FGameplayTagContainer GameplayTagContainer) const
{
	if (GameplayTagContainer.IsEmpty())
	{
		return FGameplayTag::EmptyTag;
	}
	
	return GameplayTagContainer.First();
}

UTwoMinGameplayAbility* UTwoMinAbilitySystemComponent::GetActiveAbilityInputTag(const FGameplayTag& InInputTag)
{
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag)) continue;

		if (AbilitySpec.Ability)
		{
			return Cast<UTwoMinGameplayAbility>(AbilitySpec.GetPrimaryInstance());
		}
	}

	return nullptr;
}

UTwoMinGameplayAbility* UTwoMinAbilitySystemComponent::GetActiveAbility(const FGameplayTag& AbilityTag)
{
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability->AbilityTags.HasTagExact(AbilityTag) == false)
		{
			continue;
		}

		return Cast<UTwoMinGameplayAbility>(AbilitySpec.GetPrimaryInstance());
	}

	return nullptr;
}

bool UTwoMinAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{
	TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(), 
		FoundAbilitySpecs);

	int32 AbilityIndex = 0;
	if (ATwoMinEnemyDummy* EnemyDummy = Cast<ATwoMinEnemyDummy>(GetAvatarActor()))
	{
		AbilityIndex = EnemyDummy->GetAttackIndex();
		if (AbilityIndex <= 0 || EnemyDummy->GetAutoAction() == false)
		{
			return false;
		}
	}
	
	if (!FoundAbilitySpecs.IsEmpty())
	{
		const int32 RandomAbilityIndex =
			AbilityIndex != 0 ? FMath::Min(AbilityIndex - 1, FoundAbilitySpecs.Num() - 1) :
			FMath::RandRange(0, FoundAbilitySpecs.Num() - 1);
		const FGameplayAbilitySpec* SpecToActivate = FoundAbilitySpecs[RandomAbilityIndex];
    
		if (!SpecToActivate->IsActive())
		{
			return TryActivateAbility(SpecToActivate->Handle);
		}
	}
    	
	return false;
}

void UTwoMinAbilitySystemComponent::GiveExperience(ATwoMinEnemyCharacter* InEnemyCharacter)
{
	UTwoMinAbilitySystemComponent* TargetASC = InEnemyCharacter->GetAbilitySystemComponent();
	GiveExperienceAmount(TargetASC->GetNumericAttribute(UTwoMinAttributeSet::GetGiveExperienceAttribute()));
}

void UTwoMinAbilitySystemComponent::GiveExperienceAmount(int32 InExperienceAmount)
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActor());
	if (!PlayerCharacter) return;
	
	FGameplayEffectSpecHandle Spec = MakeOutgoingSpec(
		PlayerCharacter->GetExperienceGainEffect()->GetClass(),
		1,
		MakeEffectContext()
	);

	Spec.Data->SetSetByCallerMagnitude(TwoMinGameplayTag::Data_Gain_Experience, InExperienceAmount);
	ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}

void UTwoMinAbilitySystemComponent::GiveGold(ATwoMinEnemyCharacter* InEnemyCharacter)
{
	UTwoMinAbilitySystemComponent* TargetASC = InEnemyCharacter->GetAbilitySystemComponent();
	GiveGoldAmount(TargetASC->GetNumericAttribute(UTwoMinAttributeSet::GetGiveGoldAttribute()));
}

void UTwoMinAbilitySystemComponent::GiveGoldAmount(int32 InGoldAmount)
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActor());
	if (!PlayerCharacter) return;
	
	FGameplayEffectSpecHandle Spec = MakeOutgoingSpec(
		PlayerCharacter->GetGoldGainEffect()->GetClass(),
		1,
		MakeEffectContext()
	);

	PlayerCharacter->SetGainGold(InGoldAmount);
	Spec.Data->SetSetByCallerMagnitude(TwoMinGameplayTag::Data_Gain_Gold, InGoldAmount);
	ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}

void UTwoMinAbilitySystemComponent::GiveHealthPercent(float InHealthPercent)
{
	ATwoMinBaseCharacter* Character = Cast<ATwoMinBaseCharacter>(GetAvatarActor());
	if (!Character) return;
	
	FGameplayEffectSpecHandle Spec = MakeOutgoingSpec(
		Character->GetHealthGainEffect()->GetClass(),
		1,
		MakeEffectContext()
	);
	
	int32 GiveHealthAmount = 
		FMath::FloorToInt32(GetNumericAttribute(UTwoMinAttributeSet::GetMaxHealthAttribute()) * InHealthPercent);

	Spec.Data->SetSetByCallerMagnitude(TwoMinGameplayTag::Data_Gain_Health, GiveHealthAmount);
	ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	
	FString Str = FString::Printf(TEXT("Give Health : %d"), GiveHealthAmount);
	TwoMinDebugHelper::Print(Str, FColor::Green);
}

void UTwoMinAbilitySystemComponent::GiveFightValue(FName InFightRowName)
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActor());
	if (!PlayerCharacter) return;
	
	FGameplayEffectSpecHandle Spec = MakeOutgoingSpec(
		PlayerCharacter->GetFightGainEffect()->GetClass(),
		1,
		MakeEffectContext()
	);

	UCurveTable* CurveTable = PlayerCharacter->GetFightCurveTable();
	const FRealCurve* Curve = CurveTable->FindCurve(InFightRowName, TEXT(""));
	int32 GainFightValue = Curve->Eval(1);
	
	Spec.Data->SetSetByCallerMagnitude(TwoMinGameplayTag::Data_Gain_Fight, GainFightValue);
	ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	
	StartDecreaseFightValue(PlayerCharacter);
}

void UTwoMinAbilitySystemComponent::StartDecreaseFightValue(ATwoMinPlayerCharacter* PlayerCharacter)
{
	UCurveTable* CurveTable = PlayerCharacter->GetFightCurveTable();
	FTimerManager& TimerManager = PlayerCharacter->GetWorldTimerManager();
	if (TimerManager.IsTimerActive(PlayerCharacter->FightDecreaseTimerHandle))
	{
		TimerManager.ClearTimer(PlayerCharacter->FightDecreaseTimerHandle);
	}
	
	EndDecreaseFightValue();
	
	float DecreaseDelay = CurveTable->FindCurve(FName("Player.DecDelay.Fight"), TEXT(""))->Eval(1);
	PlayerCharacter->GetWorldTimerManager().SetTimer(PlayerCharacter->FightDecreaseTimerHandle,
		[this, PlayerCharacter]()
	{
			if (FightDecreaseHandle.IsValid()) return;
			
			FGameplayEffectSpecHandle Spec = MakeOutgoingSpec(
				PlayerCharacter->GetFightDecreaseEffect()->GetClass(), 
				1.f, 
				MakeEffectContext()
			);
			
			FightDecreaseHandle = ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}, DecreaseDelay, false);
}

void UTwoMinAbilitySystemComponent::EndDecreaseFightValue()
{
	if (FightDecreaseHandle.IsValid())
	{
		RemoveActiveGameplayEffect(FightDecreaseHandle);
		FightDecreaseHandle.Invalidate();
	}
}

void UTwoMinAbilitySystemComponent::AddAngerBuffEffect(FGameplayTag ApplyBuffTag, bool bIsPercent, float BuffAmount)
{
	const ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActor());
	if (!PlayerCharacter) return;
	
	FGameplayEffectSpecHandle Spec = 
		MakeOutgoingSpec(
			PlayerCharacter->GetConsumeStatusEffect(),
			1.f,
			MakeEffectContext()
		);
	
	if (!Spec.IsValid()) return;
	
	UCurveTable* CurveTable = PlayerCharacter->GetNeedToLevelUp_ExperienceCurveTable();
	const int32 CurLevel = GetNumericAttribute(UTwoMinAttributeSet::GetCurrentLevelAttribute());
	
	int32 AttackPower = 0;
	int32 DefensePower = 0;
	int32 MaxHealth = 0;
	int32 MaxStamina = 0;
	if (ApplyBuffTag == TwoMinGameplayTag::Data_Buff_AttackPower)
	{
		const FRealCurve* Curve = CurveTable->FindCurve(FName("Player.AttackPower"), TEXT(""));
		if (!Curve) return;
	
		int32 BaseStatusValue = Curve->Eval(CurLevel);
		AttackPower = bIsPercent ? FMath::FloorToInt32(BaseStatusValue * BuffAmount) : FMath::FloorToInt32(BuffAmount);
	}
	else if (ApplyBuffTag == TwoMinGameplayTag::Data_Buff_DefensePower)
	{
		const FRealCurve* Curve = CurveTable->FindCurve(FName("Player.DefensePower"), TEXT(""));
		if (!Curve) return;
	
		int32 BaseStatusValue = Curve->Eval(CurLevel);
		DefensePower = bIsPercent ? FMath::FloorToInt32(BaseStatusValue * BuffAmount) : FMath::FloorToInt32(BuffAmount);
	}
	else if (ApplyBuffTag == TwoMinGameplayTag::Data_Buff_MaxHealth)
	{
		const FRealCurve* Curve = CurveTable->FindCurve(FName("Player.MaxHealth"), TEXT(""));
		if (!Curve) return;
	
		int32 BaseStatusValue = Curve->Eval(CurLevel);
		MaxHealth = bIsPercent ? FMath::FloorToInt32(BaseStatusValue * BuffAmount) : FMath::FloorToInt32(BuffAmount);
	}
	else if (ApplyBuffTag == TwoMinGameplayTag::Data_Buff_MaxStamina)
	{
		const FRealCurve* Curve = CurveTable->FindCurve( FName("Player.MaxStamina"), TEXT(""));
		if (!Curve) return;
	
		int32 BaseStatusValue = Curve->Eval(CurLevel);
		MaxStamina = bIsPercent ? FMath::FloorToInt32(BaseStatusValue * BuffAmount) : FMath::FloorToInt32(BuffAmount);
	}
	
	Spec.Data->SetSetByCallerMagnitude(TwoMinGameplayTag::Data_Buff_AttackPower, AttackPower);
	Spec.Data->SetSetByCallerMagnitude(TwoMinGameplayTag::Data_Buff_DefensePower, DefensePower);
	Spec.Data->SetSetByCallerMagnitude(TwoMinGameplayTag::Data_Buff_MaxHealth, MaxHealth);
	Spec.Data->SetSetByCallerMagnitude(TwoMinGameplayTag::Data_Buff_MaxStamina, MaxStamina);
	
	FActiveGameplayEffectHandle Handle = ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	AngerBuffEffectMap.Add(ApplyBuffTag, Handle);
	PlayerCharacter->GetInventoryComponent()->GetInventoryUI()->GetEquipmentWindow()->UpdateStatusText();
}

void UTwoMinAbilitySystemComponent::RemoveAngerBuffEffect(FGameplayTag RemoveBuffTag)
{
	const ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActor());
	if (!PlayerCharacter) return;
	
	FActiveGameplayEffectHandle* Handle = AngerBuffEffectMap.Find(RemoveBuffTag);
	if (!Handle) return;
		
	RemoveActiveGameplayEffect(*Handle);
	AngerBuffEffectMap.Remove(RemoveBuffTag);
	PlayerCharacter->GetInventoryComponent()->GetInventoryUI()->GetEquipmentWindow()->UpdateStatusText();
}

void UTwoMinAbilitySystemComponent::AddEquippedItemEffect(int32 ItemID, FActiveGameplayEffectHandle InEffectHandle)
{
	EquippedItemEffectMap.Add(ItemID, InEffectHandle);
}

FActiveGameplayEffectHandle* UTwoMinAbilitySystemComponent::FindEquippedItemEffect(int32 ItemID)
{
	return EquippedItemEffectMap.Find(ItemID);
}

void UTwoMinAbilitySystemComponent::RemoveEquippedItemEffect(int32 ItemID)
{
	EquippedItemEffectMap.Remove(ItemID);
}

void UTwoMinAbilitySystemComponent::AddConsumeBuff(int32 ItemID)
{
	const ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActor());
	if (!PlayerCharacter) return;
	
	UTwoMinGameInstance* GI = GetWorld()->GetGameInstance<UTwoMinGameInstance>();
	FItemConsumeData Item = GI->ItemDataManager->GetItemConsumeData(ItemID);

	FGameplayEffectSpecHandle Spec = 
		MakeOutgoingSpec(
			PlayerCharacter->GetConsumeStatusEffect(),
			1.f,
			MakeEffectContext()
		);
	
	if (!Spec.IsValid()) return;
	
	if (Item.ConsumePower.IsEmpty()) return;

	int32 AttackPower = 0;
	int32 DefensePower = 0;
	int32 MaxHealth = 0;
	int32 MaxStamina = 0;
	UCurveTable* CurveTable = PlayerCharacter->GetNeedToLevelUp_ExperienceCurveTable();
	const int32 CurLevel = GetNumericAttribute(UTwoMinAttributeSet::GetCurrentLevelAttribute());
	
	for (auto ConsumePower : Item.ConsumePower)
	{
		if (ConsumePower.Key == EStatusType::Attack)
		{
			const FRealCurve* Curve = CurveTable->FindCurve(FName("Player.AttackPower"), TEXT(""));
			int32 BaseStatusValue = Curve->Eval(CurLevel);
			AttackPower = FMath::FloorToInt32(BaseStatusValue * ConsumePower.Value);
		}
		else if (ConsumePower.Key == EStatusType::Defense)
		{
			const FRealCurve* Curve = CurveTable->FindCurve(FName("Player.DefensePower"), TEXT(""));
			int32 BaseStatusValue = Curve->Eval(CurLevel);
			DefensePower = FMath::FloorToInt32(BaseStatusValue * ConsumePower.Value);
		}
		else if (ConsumePower.Key == EStatusType::MaxHealth)
		{
			const FRealCurve* Curve = CurveTable->FindCurve(FName("Player.MaxHealth"), TEXT(""));
			int32 BaseStatusValue = Curve->Eval(CurLevel);
			MaxHealth = FMath::FloorToInt32(BaseStatusValue * ConsumePower.Value);
		}
		else if (ConsumePower.Key == EStatusType::MaxStamina)
		{
			const FRealCurve* Curve = CurveTable->FindCurve(FName("Player.MaxStamina"), TEXT(""));
			int32 BaseStatusValue = Curve->Eval(CurLevel);
			MaxStamina = FMath::FloorToInt32(BaseStatusValue * ConsumePower.Value);
		}
	}
	
	Spec.Data->SetSetByCallerMagnitude(TwoMinGameplayTag::Data_Buff_AttackPower, AttackPower);
	Spec.Data->SetSetByCallerMagnitude(TwoMinGameplayTag::Data_Buff_DefensePower, DefensePower);
	Spec.Data->SetSetByCallerMagnitude(TwoMinGameplayTag::Data_Buff_MaxHealth, MaxHealth);
	Spec.Data->SetSetByCallerMagnitude(TwoMinGameplayTag::Data_Buff_MaxStamina, MaxStamina);
	
	FActiveGameplayEffectHandle Handle = ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	BuffItemEffectMap.Add(Item.ItemDataBase.ItemID, Handle);
	PlayerCharacter->GetInventoryComponent()->GetInventoryUI()->GetEquipmentWindow()->UpdateStatusText();
}

void UTwoMinAbilitySystemComponent::RemoveConsumeBuff(int32 ItemID)
{
	const ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActor());
	if (!PlayerCharacter) return;
	
	FActiveGameplayEffectHandle* Handle = BuffItemEffectMap.Find(ItemID);
	if (!Handle) return;
		
	RemoveActiveGameplayEffect(*Handle);
	BuffItemEffectMap.Remove(ItemID);
	PlayerCharacter->GetInventoryComponent()->GetInventoryUI()->GetEquipmentWindow()->UpdateStatusText();
}
