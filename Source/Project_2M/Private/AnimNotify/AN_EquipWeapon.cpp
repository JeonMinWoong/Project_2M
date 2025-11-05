// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AN_EquipWeapon.h"

#include "Item/Weapon/TwoMinWeaponBase.h"

void UAN_EquipWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                             const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	TArray<AActor*> AttachedActors;
	MeshComp->GetOwner()->GetAttachedActors(AttachedActors);

	ATwoMinWeaponBase* Weapon = nullptr;
	for (AActor* Child : AttachedActors)
	{
		if (Child->GetAttachParentSocketName() == UnequipSocket)
		{
			Weapon = Cast<ATwoMinWeaponBase>(Child);
		}
	}

	if (!Weapon) return;
	
	
	FAttachmentTransformRules AttachmentRules(
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::KeepRelative,
				EAttachmentRule::KeepWorld,
				true);
			
	Weapon->AttachToComponent(
		MeshComp,
		AttachmentRules,
		EquipSocketName
	);
}
