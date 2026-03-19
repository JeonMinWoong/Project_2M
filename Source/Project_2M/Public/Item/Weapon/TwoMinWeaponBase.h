// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "TwoMinWeaponBase.generated.h"

struct FTwoMinPlayerWeaponData;
class UBoxComponent;

DECLARE_DELEGATE_TwoParams(FOnTargetInteractedDelegate, AActor*, int32);

UCLASS()
class PROJECT_2M_API ATwoMinWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	ATwoMinWeaponBase();
	
	FOnTargetInteractedDelegate OnWeaponHitTarget;
	FOnTargetInteractedDelegate OnWeaponPulledFromTarget;

	virtual FTwoMinPlayerWeaponData GetWeaponData() const;
	void StartDissolveProcess();
	void UpdateDissolveMaterial();

	UPROPERTY(EditAnywhere, Category = "ToggleDamageType")
	EToggleDamageType ToggleDamageType = EToggleDamageType::None;
	
	FTimerHandle DissolveTimerHandle;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	UStaticMeshComponent* WeaponMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	UBoxComponent* WeaponCollisionBox;

	UFUNCTION()
	virtual void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(EditDefaultsOnly, Category = "WeaponIndex")
	int32 WeaponIndex = 0;
	
	UPROPERTY()
	float UpdateDissolveTime = 0.025f;
	
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> CachedDynamicMaterials;
	
	UPROPERTY()
	float CurDissolve = 0;
	
	UPROPERTY()
	float UpdateDissolveValue = 0.01;
	
public:
	FORCEINLINE UBoxComponent* GetWeaponCollisionBox() const { return WeaponCollisionBox; }
	FORCEINLINE UStaticMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE void EnableMesh(bool bShouldEnable) const { WeaponMesh->SetVisibility(bShouldEnable); }
};
