// Fill out your copyright notice in the Description page of Project Settings.


#include "TwoMinFunctionLibrary.h"

#include "GenericTeamAgentInterface.h"

bool UTwoMinFunctionLibrary::IsTargetPawnHostile(APawn* OwnerPawn, APawn* TargetPawn)
{
	IGenericTeamAgentInterface* OwnerTeamAgent = Cast<IGenericTeamAgentInterface>(OwnerPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (OwnerTeamAgent && TargetTeamAgent)
	{
		return OwnerTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}
	
	return false;
}
