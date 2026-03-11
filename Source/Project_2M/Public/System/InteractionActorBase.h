// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "InteractionActorBase.generated.h"

class ATwoMinPlayerCharacter;
class USphereComponent;

UCLASS()
class PROJECT_2M_API AInteractionActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractionActorBase();

	virtual void Tick(float DeltaTime) override;
	virtual void Interact(ATwoMinPlayerCharacter* PlayerCharacter);

	bool IsPossibleInteraction() const;
		
	virtual void ResetInteractionProcess();
	virtual bool IsHiddenCondition();
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly, Category="Interaction|InteractionType")
	EInteractionType InteractionType = EInteractionType::None;
	
	UPROPERTY(EditDefaultsOnly, Category = "InteractionInfo|Radius")
	float InteractionRadius = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "InteractionInfo|Angle")
	float InteractionAngle = 60.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "InteractionInfo|AbilityTag")
	FGameplayTag InteractionTag;
	
private:
	UPROPERTY(VisibleAnywhere, Category = "InteractionInfo|Components")
	USphereComponent* SphereComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "InteractionInfo|OverlapCharacter")
	ATwoMinPlayerCharacter* OverlapCharacter;
	
public:
	FORCEINLINE EInteractionType GetInteractionType() const { return InteractionType; }
};
