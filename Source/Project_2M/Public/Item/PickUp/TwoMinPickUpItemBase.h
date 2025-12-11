#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "TwoMinPickUpItemBase.generated.h"

class ATwoMinPlayerCharacter;

UCLASS()
class PROJECT_2M_API ATwoMinPickUpItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ATwoMinPickUpItemBase();

	virtual void BeginPlay() override;
	
	void MakeItemDropBox(TPair<int32, int32> Item);
	
	void GetUpItem(const ATwoMinPlayerCharacter* PlayerCharacter);
protected:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	virtual void Tick(float DeltaSeconds) override;
	
private:
	void StartItemPickUpAbility(const ATwoMinPlayerCharacter* PlayerCharacter);
	
	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, Category="PickUpRange")
	float PickUpRange = 50.f;

	UPROPERTY(EditDefaultsOnly, Category="EquipmentData")
	UDataTable* EquipmentDataTable;

	UPROPERTY(EditDefaultsOnly, Category="ConsumeData")
	UDataTable* ConsumeDataTable;

	UPROPERTY(EditDefaultsOnly, Category="EtcData")
	UDataTable* EtcDataTable;
	
	UPROPERTY(VisibleAnywhere)
	TArray<FItemEquipmentData> ItemEquipmentList;

	UPROPERTY(VisibleAnywhere)
	TArray<FItemConsumeData> ItemConsumeList;

	UPROPERTY(VisibleAnywhere)
	TArray<FItemEtcData> ItemEtcList;

	UPROPERTY()
	ATwoMinPlayerCharacter* CachedPlayerCharacter;

	bool bIsTryPickUpAbility = true;
};
