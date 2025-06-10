// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Player/TwoMinPlayerAnimInstance.h"

#include "Character/TwoMinPlayerCharacter.h"

void UTwoMinPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwningCharacter)
	{
		OwningPlayerCharacter = Cast<ATwoMinPlayerCharacter>(OwningCharacter);
	}
}

void UTwoMinPlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (OwningPlayerCharacter)
	{
		bIsRun = OwningPlayerCharacter->GetIsRunning();
	}
}
