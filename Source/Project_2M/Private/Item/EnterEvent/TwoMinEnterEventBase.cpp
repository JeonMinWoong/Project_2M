

#include "Item/EnterEvent/TwoMinEnterEventBase.h"

#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "TwoMinDebugHelper.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/Combat/BaseCombatComponent.h"
#include "Components/BoxComponent.h"
#include "Item/Weapon/TwoMinWeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "Stateless/NiagaraStatelessDistribution.h"

class ALevelSequenceActor;

ATwoMinEnterEventBase::ATwoMinEnterEventBase()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBeginOverlap);
}

void ATwoMinEnterEventBase::OutPlayLevelSequence(AActor* PlayerActor)
{
	if (bIsEventActivated) return;
	
	CheckHideCharacters();
	PlayLevelSequence(PlayerActor);
}

void ATwoMinEnterEventBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsEventActivated || bIsIgnoreCollision) return;
	
	TwoMinDebugHelper::Print(TEXT("EnterEvent BeginOverlap"));
	
	CheckHideCharacters();
	PlayLevelSequence(OtherActor);
}

void ATwoMinEnterEventBase::CheckHideCharacters()
{
	HideEnemyCharacters.Empty();
	
	TArray<AActor*> FoundEnemyActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATwoMinEnemyCharacter::StaticClass(), FoundEnemyActors);
	for (auto FoundEnemyActor : FoundEnemyActors)
	{
		ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(FoundEnemyActor);
		if (!EnemyCharacter) continue;
		
		const FString LevelSequenceName = EnterLevelSequence->GetName();
		if (EnemyCharacter->GetHideCinematic(LevelSequenceName) == false) continue;
		if (EnemyCharacter->GetIsHideCharacter()) continue;
		
		HideEnemyCharacters.Emplace(EnemyCharacter);
	}
}

void ATwoMinEnterEventBase::PlayLevelSequence(AActor* OtherActor)
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(OtherActor);
	if (!PlayerCharacter) return;
	
	PlayerCharacter->SetActorHiddenInGame(true);
	PlayerCharacter->GetCombatComponent()->EnableWeaponsMesh(false);
	PlayerCharacter->GetHUDOverlay()->SetVisibility(ESlateVisibility::Hidden);
	
	for (auto HideCharacter : HideEnemyCharacters)
	{
		ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(HideCharacter);
		if (!EnemyCharacter) continue;
		
		EnemyCharacter->OnHideCharacter();
	}
	
	if (!EnterLevelSequence) return;
	
	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.bAutoPlay = true;
	PlaybackSettings.PlayRate = 1.0f;       
	
	ALevelSequenceActor* OutActor = nullptr;
	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		EnterLevelSequence,
		PlaybackSettings,
		OutActor
	);
	
	if (!LevelSequencePlayer) return;
	
	bIsEventActivated = true;
	OnIgnorePlayerInput(true);
	
	LevelSequencePlayer->Play();
	LevelSequencePlayer->OnFinished.AddUniqueDynamic(this, &ATwoMinEnterEventBase::OnFinishLevelSequence);

	LevelActorGroup.Empty();
	const FString LevelSequenceName = EnterLevelSequence->GetName();
	TArray<FMovieSceneBinding> Bindings = EnterLevelSequence->GetMovieScene()->GetBindings();
	for (auto Binding : Bindings)
	{
		FMovieSceneObjectBindingID BindingID(Binding.GetObjectGuid());
		TArray<UObject*> BoundObjects = LevelSequencePlayer->GetBoundObjects(BindingID);
		for (UObject* Obj : BoundObjects)
		{
			AActor* FoundActor = Cast<AActor>(Obj);
			if (IsValid(FoundActor))
			{
				LevelActorGroup.Emplace(FoundActor);
			}
		}
	}
	
	for (auto HideEnemyCharacter : HideEnemyCharacters)
	{
		if (HideEnemyCharacter->GetHideCinematic(LevelSequenceName) == false) continue;
		if (HideEnemyCharacter->GetCombatComponent()->GetIsAlive() == false) continue;
		
		const FString HideCharacterName = HideEnemyCharacter->GetSyncCinematicActorName(LevelSequenceName);
		if (HideCharacterName.IsEmpty()) continue;
		
		for (auto Actor : LevelActorGroup)
		{
			if (Actor->GetActorLabel() == HideCharacterName)
			{
				HideEnemyCharacter->SetCinematicSyncActor(Actor);
				break;
			}
		}
	}
}

void ATwoMinEnterEventBase::OnFinishLevelSequence()
{
	OnIgnorePlayerInput(false);
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(PC->GetPawn());
	if (!PlayerCharacter) return;
	
	PlayerCharacter->SetActorHiddenInGame(false);
	PlayerCharacter->GetCombatComponent()->EnableWeaponsMesh(true);
	PlayerCharacter->GetHUDOverlay()->SetVisibility(ESlateVisibility::Visible);
	
	for (auto HideCharacter : HideEnemyCharacters)
	{
		if (!HideCharacter) continue;
		if (HideCharacter->GetIsHideCharacter() == false) continue;
		
		HideCharacter->OnShowCharacter();
		AActor* FindActor = HideCharacter->GetCinematicSyncActor();
		
		for (auto Actor : LevelActorGroup)
		{
			if (!IsValid(Actor)) continue;
			if (FindActor != Actor) continue;
			
			if (Actor && Actor->GetWorld() && !Actor->IsPendingKillPending())
			{
				if (ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(Actor))
				{
					TArray<ATwoMinWeaponBase*> WeaponGroup = EnemyCharacter->GetCombatComponent()->GetCharacterCurrentEquippedWeapon();
					for (ATwoMinWeaponBase* WeaponBase : WeaponGroup)
					{
						WeaponBase->Destroy();
					}
				}
					
				const FVector Location = Actor->GetActorLocation();
				const FRotator Rotation = Actor->GetActorRotation();
				HideCharacter->SetActorLocationAndRotation(Location, Rotation);
			}
		}
	}

	for (auto Actor : LevelActorGroup)
	{
		Actor->Destroy();
	}
}

void ATwoMinEnterEventBase::OnIgnorePlayerInput(bool OnIgnore)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;
	
	PC->SetIgnoreMoveInput(OnIgnore);
	PC->SetIgnoreLookInput(OnIgnore);
	PC->FlushPressedKeys();
	
	if (OnIgnore)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		
		PC->SetInputMode(InputMode);	
	}
	else
	{
		PC->SetInputMode(FInputModeGameOnly());
	}
}



