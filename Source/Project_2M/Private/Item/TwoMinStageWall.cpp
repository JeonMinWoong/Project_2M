
#include "Item/TwoMinStageWall.h"

ATwoMinStageWall::ATwoMinStageWall()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ATwoMinStageWall::OpenStageWall()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ATwoMinStageWall::CloseStageWall()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

