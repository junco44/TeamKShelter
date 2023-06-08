// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Math/UnrealMathUtility.h"
#include "InteractInterface.h"
#include "Components/RectLightComponent.h"
#include "Elevator.generated.h"

UCLASS()
class LEFT4DEAD2_API AElevator : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AElevator();

	UPROPERTY(VisibleAnywhere, Category = "Scene")
		class USceneComponent* Elevator;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		class UStaticMeshComponent* Floor;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		UStaticMeshComponent* Front1;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		UStaticMeshComponent* Front2;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		UStaticMeshComponent* InnerButton;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		UStaticMeshComponent* Door1;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		UStaticMeshComponent* Door2;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		UStaticMeshComponent* Wall1;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		UStaticMeshComponent* Wall2;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		UStaticMeshComponent* Mirror;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		UStaticMeshComponent* Ceiling;
	UPROPERTY(VisibleAnywhere, Category = "Collision")
		class UBoxComponent* FloorCollision;
	UPROPERTY(VisibleAnywhere, Category = "Collision")
		UBoxComponent* InnerCollision;
	UPROPERTY(VisibleAnywhere, Category = "Light1")
		class URectLightComponent* Light1;
	UPROPERTY(VisibleAnywhere, Category = "Light2")
		class URectLightComponent* Light2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void MyInteract_Implementation() override;

	UPROPERTY(EditAnywhere, Category = "Elevator")
		class UTimelineComponent* DoorTimeline;

	UPROPERTY(EditAnywhere, Category = "Elevator")
		class UTimelineComponent* ElevatorTimeline;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UCurveFloat* ElevatorDoorCurveFloat;

	UFUNCTION(BlueprintCallable, Category = "Elevator")
		void OpenDoor();

	UFUNCTION(BlueprintCallable, Category = "Elevator")
		void CloseDoor();

	UFUNCTION(BlueprintCallable, Category = "Elevator")
		void GoUp();

	UFUNCTION(BlueprintCallable, Category = "Elevator")
		void LightOff();
	UFUNCTION(BlueprintCallable, Category = "Elevator")
		void LightOn();

private:

	FVector InitialLocation;

	FTimerHandle TimerHandle1;
	FTimerHandle TimerHandle2;

protected:


	//FTimeline Timeline;

	FTimeline Timeline;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UCurveFloat* ElevatorCurveFloat;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void ControlElevator(float Value);
	UFUNCTION()
		void ControlDoor(float Value);

	float Height = 955.f;
	float Location = 160.f;

};
