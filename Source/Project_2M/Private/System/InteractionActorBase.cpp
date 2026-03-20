// Fill out your copyright notice in the Description page of Project Settings.


#include "System/InteractionActorBase.h"

#include "TwoMinDebugHelper.h"
#include "TwoMinGameplayTag.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

AInteractionActorBase::AInteractionActorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->InitSphereRadius(InteractionRadius);
	SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnEndOverlap);
}

void AInteractionActorBase::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComponent->SetSphereRadius(InteractionRadius);
}

void AInteractionActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OverlapCharacter)
	{
		if (IsPossibleInteraction() == false)
		{
			ResetInteractionProcess();
			return;
		}
		UTwoMinAbilitySystemComponent* ASC = OverlapCharacter->GetAbilitySystemComponent();
		if (!ASC) return;
		
		if (ASC->IsPlayingAbility(InteractionTag) == false)
		{
			ASC->TryActivateAbilityByTag(InteractionTag);
		}
	}
}

void AInteractionActorBase::Interact(ATwoMinPlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->SetInteractionActor(this);
}

bool AInteractionActorBase::IsPossibleInteraction() const
{
	if (!OverlapCharacter) return false;
	
	const FVector ToTarget = (GetActorLocation() - OverlapCharacter->GetActorLocation()).GetSafeNormal2D();
	const FVector TargetForward = OverlapCharacter->GetActorForwardVector().GetSafeNormal2D();
	const float Angle = UKismetMathLibrary::DegAcos(FVector::DotProduct(ToTarget, TargetForward));
	bool bIsPossible = Angle <= InteractionAngle;
	return bIsPossible;
}

bool AInteractionActorBase::IsHiddenCondition()
{
	return false;
}


void AInteractionActorBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATwoMinPlayerCharacter* OverLappedHeroCharacter = Cast<ATwoMinPlayerCharacter>(OtherActor);
	if (!OverLappedHeroCharacter) return;
	
	OverlapCharacter = OverLappedHeroCharacter;
}

void AInteractionActorBase::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlapCharacter = nullptr;
}

void AInteractionActorBase::ResetInteractionProcess()
{
	
}
