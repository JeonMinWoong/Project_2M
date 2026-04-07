// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TwoMinEnemyDummy.h"

#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "Components/WidgetComponent.h"
#include "Widgets/Enemy/TwoMinWidgetDummy.h"

ATwoMinEnemyDummy::ATwoMinEnemyDummy(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AIStateWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("AIStateWidgetComponent"));
	AIStateWidgetComponent->SetupAttachment(GetMesh());
}

void ATwoMinEnemyDummy::BeginPlay()
{
	Super::BeginPlay();
	
	if (UTwoMinWidgetDummy* AIStateWidget = Cast<UTwoMinWidgetDummy>(AIStateWidgetComponent->GetUserWidgetObject()))
	{
		AIStateWidget->InitEnemyIComponent(EnemyUIComponent);
	}
}
