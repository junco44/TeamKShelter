// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "Components/BoxComponent.h"
#include "CeilingLight.h"
#include "CollapsingFloor.h"
#include "ElevatorButton.h"
#include "Elevator.h"
#include "Kismet/GameplayStatics.h"
#include "Generator.generated.h"

class ACeilingLight;
class ACollapsingFloor;
class AElevatorButton;
class AElevator;

UCLASS()
class LEFT4DEAD2_API AGenerator : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGenerator();

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* Generator;
	UPROPERTY(VisibleAnywhere, Category = "BoxCollision")
	class UBoxComponent* BoxCollision;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void MyInteract_Implementation() override;

	void GeneratorOn();

private:
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, meta = (AllowPrivateAccess = true))
	TArray<ACeilingLight*> LightArray;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, meta = (AllowPrivateAccess = true))
	ACollapsingFloor* CollapsingFloor;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, meta = (AllowPrivateAccess = true))
	AElevatorButton* TaskCheck;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, meta = (AllowPrivateAccess = true))
	AElevator* Elevator;
};

