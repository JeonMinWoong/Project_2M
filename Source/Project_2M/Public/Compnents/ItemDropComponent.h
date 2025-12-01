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

	TMap<int32, int32> TryGetDropItems() const;

private:
	void CalculateDropProbability(const FItemDropData* ItemDropData, int32& OutItemCode, int32& OutDropCount) const;
	
	UPROPERTY(EditDefaultsOnly, Category = "DropTable")
	UDataTable* DropTable;

	UPROPERTY(EditDefaultsOnly, Category = "DropTableOwner")
	FString DropTableOwner;
};
