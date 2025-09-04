// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/BaseUIInterface.h"

// Add default functionality here for any IIUIBaseInterface functions that are not pure virtual.
UPlayerUIComponent* IBaseUIInterface::GetPlayerUIComponent() const
{
	return nullptr;
}

UEnemyUIComponent* IBaseUIInterface::GetEnemyUIComponent() const
{
	return nullptr;
}
