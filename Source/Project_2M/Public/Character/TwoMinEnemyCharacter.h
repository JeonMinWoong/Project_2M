// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TwoMinBaseCharacter.h"
#include "Components/WidgetComponent.h"
#include "TwoMinEnemyCharacter.generated.h"

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
	virtual void BeforeDeathProcess() override;
	virtual void AfterDeathProcess() override;
	virtual void OnDestroyedProcess() override;
	
	void ClearStageProcess() const;
	
	FTimerHandle DecreaseGroggyTimerHandle;
	
protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UEnemyUIComponent* EnemyUIComponent;
	
private:
	void InitEnemyHealthWidget();
	
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

	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|CharacterMovement")
	float MaxWalkSpeed = 300.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|CharacterMovement")
	FRotator CharacterRotationRate = FRotator(0.f, 500.f, 0.f);

	UPROPERTY(EditDefaultsOnly, Category = "ItemDropClass")
	TSubclassOf<ATwoMinPickUpItemBase> ItemDropClass; 
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|IsBossHealthBar")
	bool bUseBossHealthBar = false;
	
public:
	EMonsterType GetMonsterType() const { return MonsterType; };
	FString GetMonsterName() const { return MonsterName; };
	
	FORCEINLINE void EnableExecutionWidget(bool bIsEnable) const { EnemyExecutionWidgetComponent->SetVisibility(bIsEnable); }
	
	FORCEINLINE void SetUseBossHealthBar(const bool bIsUse) { bUseBossHealthBar =  bIsUse; }
	FORCEINLINE bool IsUseBossHealthBar() const { return bUseBossHealthBar; }
	
};
