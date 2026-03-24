
#include "Item/TwoMinStageWall.h"

#include "NiagaraComponent.h"

ATwoMinStageWall::ATwoMinStageWall()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ATwoMinStageWall::BeginPlay()
{
	Super::BeginPlay();
	
	NiagaraComponent = Cast<UNiagaraComponent>(GetDefaultSubobjectByName(FName("NiagaraComp")));
}

void ATwoMinStageWall::OpenStageWall()
{
	if (NiagaraComponent)
	{
		NiagaraComponent->Deactivate();	
	}
	
	SetActorEnableCollision(false);
}

void ATwoMinStageWall::CloseStageWall()
{
	if (NiagaraComponent)
	{
		NiagaraComponent->Activate(true);	
	}
	
	SetActorEnableCollision(true);
}

