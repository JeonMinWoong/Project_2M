// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/TwoMinPlayerController.h"

ATwoMinPlayerController::ATwoMinPlayerController()
{
	PlayerTeamId = FGenericTeamId(0);
}

FGenericTeamId ATwoMinPlayerController::GetGenericTeamId() const
{
	return PlayerTeamId;
}
