// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "ItemDropComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_2M_API UItemDropComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UItemDropComponent();

	TMap<int32, int32> TryGetCharacterDropItems() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "DropTableOwner")
	FString DropTableOwner;
};
