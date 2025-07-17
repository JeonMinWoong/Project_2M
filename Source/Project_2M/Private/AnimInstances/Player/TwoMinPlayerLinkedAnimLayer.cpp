// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Player/TwoMinPlayerLinkedAnimLayer.h"

#include "AnimInstances/Player/TwoMinPlayerAnimInstance.h"

UTwoMinPlayerAnimInstance* UTwoMinPlayerLinkedAnimLayer::GetPlayerAnimInstance() const
{
	return Cast<UTwoMinPlayerAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
