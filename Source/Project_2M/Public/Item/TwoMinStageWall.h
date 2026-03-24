
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TwoMinStageWall.generated.h"

class UNiagaraComponent;

UCLASS()
class PROJECT_2M_API ATwoMinStageWall : public AActor
{
	GENERATED_BODY()
	
public:	
	ATwoMinStageWall();

	virtual void BeginPlay() override;
	
	void OpenStageWall();
	void CloseStageWall();
	
private:
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* NiagaraComponent;

};
