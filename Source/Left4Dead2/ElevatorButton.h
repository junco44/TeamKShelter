// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "EngineUtils.h"
#include "ElevatorButton.generated.h"

class ACeilingLight;
class ACollapsingFloor;
class AElevator;

UCLASS()
class LEFT4DEAD2_API AElevatorButton : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AElevatorButton();

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		class UStaticMeshComponent* Button;
	UPROPERTY(VisibleAnywhere, Category = "Collision")
		class UBoxComponent* BoxCollision;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void MyInteract_Implementation() override;

	void SetTask(bool bFlag);

private:
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, meta = (AllowPrivateAccess = true))
		TArray<ACeilingLight*> LightArray;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, meta = (AllowPrivateAccess = true))
		ACollapsingFloor* CollapsingFloor;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, meta = (AllowPrivateAccess = true))
		AElevator* Elevator;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, meta = (AllowPrivateAccess = true))
		bool bDidTask = true;
};
