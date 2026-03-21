// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TwoMinGameInstance.generated.h"

enum class EUISoundType : uint8;
class USoundManager;
class UWorldStageManager;
class UItemDataManager;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	
	void PlayUISound(const EUISoundType NewUISoundType) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced)
	UItemDataManager* ItemDataManager;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced)
	UWorldStageManager* StateManager;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced)
	USoundManager* SoundManager;
	
	UPROPERTY(VisibleAnywhere)
	bool bIsStageMoving = false;
	
	FTimerHandle ApplyInitSettingsTimerHandle;
	
private:
	void ApplyInitSettings();
	
	UPROPERTY()
	int32 GeneralValue = 1;
};
