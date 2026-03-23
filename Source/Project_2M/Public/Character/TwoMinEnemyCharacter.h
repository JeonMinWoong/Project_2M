// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TwoMinConstant.h"
#include "Character/TwoMinBaseCharacter.h"
#include "Components/WidgetComponent.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "Widgets/Enemy/TwoMinWidgetEnemy.h"
#include "TwoMinEnemyCharacter.generated.h"

class UTwoMinWidgetEnemy;
class ATwoMinEnterEventBase;
class AAIController;
struct FCinematicCharacterData;
class UTwoMinWidgetBoss;
class ATwoMinPickUpItemBase;
class UItemDropComponent;
class UWidgetComponent;
class UEnemyCombatComponent;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API ATwoMinEnemyCharacter : public ATwoMinBaseCharacter
{
	GENERATED_BODY()

public:
	ATwoMinEnemyCharacter();

	virtual void Tick(float DeltaSeconds) override;
	virtual UBaseCombatComponent* GetCombatComponent() const override;
	virtual UBaseUIComponent* GetBaseUIComponent() const override;
	virtual UEnemyUIComponent* GetEnemyUIComponent() const override;
	UTwoMinWidgetEnemy* GetEnemyHealthWidget() const;
	
	virtual void BeforeDeathProcess() override;
	virtual void AfterDeathProcess() override;
	virtual void StartDissolveProcess() override;
	void UpdateDissolveMaterial();
	virtual void OnDestroyedProcess() override;
	
	void ClearStageProcess() const;
	
	void OnShowCharacter();
	void OnHideCharacter();
	
	bool GetHideCinematic(const FString& PlayLevelSequenceName) const;
	FString GetSyncCinematicActorName(const FString& PlayLevelSequenceName) const;
	void BossDetectProcess();

	FTimerHandle DecreaseGroggyTimerHandle;
	FTimerHandle PhaseConversionTimerHandle;
	FTimerHandle DissolveTimerHandle;
	
protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	virtual void PostInitializeComponents() override;
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UEnemyUIComponent* EnemyUIComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> BossCharacterStartUpData;
	
private:
	void InitCheckCinematic();
	void InitEnemyHealthWidget();
	void InitPhaseConversion(EBossPhaseType NewBossPhase);
	TSoftObjectPtr<UDataAsset_StartUpDataBase> GetCharacterStartUpData() const;
	/** Components **/
	UPROPERTY(VisibleAnywhere)
	UEnemyCombatComponent* EnemyCombatComponent;

	UPROPERTY(VisibleAnywhere)
	UItemDropComponent* ItemDropComponent;
	
	UPROPERTY(VisibleAnywhere, Category= "UI")
	UWidgetComponent* EnemyHealthWidgetComponent;

	UPROPERTY(VisibleAnywhere, Category= "UI")
	UWidgetComponent* EnemyExecutionWidgetComponent;

	/** CharacterInfo **/
	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|Name")
	FString MonsterName = "Monster";
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|MonsterType")
	EMonsterType MonsterType = EMonsterType::Normal;

	UPROPERTY(VisibleAnywhere, Category = "CharacterInfo|MonsterLevel")
	int32 MonsterLevel = 1;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|CharacterMovement")
	float MaxWalkSpeed = 300.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|CharacterMovement")
	FRotator CharacterRotationRate = FRotator(0.f, 500.f, 0.f);

	UPROPERTY(EditDefaultsOnly, Category = "ItemDropClass")
	TSubclassOf<ATwoMinPickUpItemBase> ItemDropClass; 
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|BossInfo|IsBossHealthBar")
	bool bUseBossHealthBar = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|BossInfo|BossBGMType")
	EBGMSoundType BossBGMSound = EBGMSoundType::None;
	
	UPROPERTY()
	bool bIsNeedChangeBossBGMSound = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|BossInfo|BossPaseType", meta = (EditCondition = "bUseBossHealthBar"))
	EBossPhaseType BossPhase = EBossPhaseType::Phase_Finish;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|BossInfo|BossPhaseTarget", meta = (EditCondition = "bUseBossHealthBar"))
	TMap<FString, FPhaseConversionData> PhaseTargetCharacter;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|BossInfo|PhaseConversionDelay", meta = (EditCondition = "bUseBossHealthBar"))
	float PhaseConversionDelay;

	// 시네마틱 캐릭터 인 지.
	UPROPERTY(EditAnywhere, Category = "CharacterInfo|IsCinematic")
	bool bIsCinematic = false;
	
	UPROPERTY(EditAnywhere, Category = "CharacterInfo|IsCinematic", meta= (EditCondition = "bIsCinematic"))
	FString CinematicSyncName;
	
	// 시네마틱 중 안 보이게 할 지.
	UPROPERTY(VisibleAnywhere, Category = "CharacterInfo|IsHideCinematicing")
	TMap<FString, FCinematicCharacterData> HideCinematicMap;
	
	UPROPERTY(VisibleAnywhere, Category = "CharacterInfo|IsHideCharacter")
	bool bIsHideCharacter = false;
	
	UPROPERTY()
	AActor* CinematicSyncActor;
	
	UPROPERTY()
	float UpdateDissolveTime = 0.025f;
	
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> CachedDynamicMaterials;
	
	UPROPERTY()
	float CurDissolve = 0;
	
	UPROPERTY()
	float UpdateDissolveValue = 0.01;
	
public:
	FORCEINLINE EMonsterType GetMonsterType() const { return MonsterType; };
	FORCEINLINE FString GetMonsterName() const { return MonsterName; };
	FORCEINLINE void SetMonsterLevel(int32 InLevel)
	{
		InLevel = FMath::Clamp(InLevel, TwoMinConstant::MaxMonsterLevel, TwoMinConstant::MaxMonsterLevel);
		MonsterLevel = InLevel;
	};
	
	FORCEINLINE void EnableExecutionWidget(bool bIsEnable) const { EnemyExecutionWidgetComponent->SetVisibility(bIsEnable); }
	
	FORCEINLINE void SetCharacterCinematicData(const TMap<FString, FCinematicCharacterData>& InHideCharacterMap)
	{ HideCinematicMap = InHideCharacterMap; };
	
	FORCEINLINE bool GetIsHideCharacter() const { return bIsHideCharacter; };
	
	FORCEINLINE void SetCinematicSyncActor(AActor* InCinematicSyncActor)
	{ CinematicSyncActor = InCinematicSyncActor; };
	
	FORCEINLINE AActor* GetCinematicSyncActor() const { return CinematicSyncActor; };
	
	FORCEINLINE void SetUseBossHealthBar(const bool bIsUse) { bUseBossHealthBar =  bIsUse; }
	FORCEINLINE bool IsUseBossHealthBar() const { return bUseBossHealthBar; }
	FORCEINLINE void SetBossPhaseType(EBossPhaseType NewBossPhaseType) { BossPhase = NewBossPhaseType; };
	
	FORCEINLINE bool IsSameCinematicSyncActorName(const FString& InMachName) const
	{
		if (bIsCinematic == false) return false;
		
		return CinematicSyncName.Equals(InMachName);
	}
	
};
