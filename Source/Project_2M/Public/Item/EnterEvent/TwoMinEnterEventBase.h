
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TwoMinEnterEventBase.generated.h"

class ULevelSequencePlayer;
struct FMovieSceneObjectBindingID;
class ATwoMinEnemyCharacter;
class ULevelSequence;
class UBoxComponent;

UCLASS()
class PROJECT_2M_API ATwoMinEnterEventBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ATwoMinEnterEventBase();
	
	void OutPlayLevelSequence(AActor* PlayerActor);
	
private:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult);
	
	void CheckHideCharacters();
	void PlayLevelSequence(AActor* OtherActor);
	
	UFUNCTION()
	void OnFinishLevelSequence();

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* BoxComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "EnterEvent|LevelSequenceEvent")
	ULevelSequence* EnterLevelSequence;
	
	UPROPERTY()
	ULevelSequencePlayer* LevelSequencePlayer;
	
	UPROPERTY()
	TArray<ATwoMinEnemyCharacter*> HideEnemyCharacters;
	
	UPROPERTY()
	TArray<AActor*> LevelActorGroup;
	
	bool bIsEventActivated = false;
	
	UPROPERTY(EditAnywhere, Category = "EnterEvent|IsIgnoreCollision")
	bool bIsIgnoreCollision = false;
	
public:
	FORCEINLINE void SetIgnoreCollision(bool bOn) { bIsIgnoreCollision = bOn; };
};
