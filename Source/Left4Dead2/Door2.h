// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Math/UnrealMathUtility.h"
#include "InteractInterface.h"
#include "Net/UnrealNetwork.h"
#include "Door2.generated.h"

UCLASS()
class LEFT4DEAD2_API ADoor2 : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoor2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void MyInteract_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Door")
	void OpenDoor();

	UPROPERTY(EditAnywhere, Category = "Door")
	class UTimelineComponent* DoorTimeline;

private:


protected:
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UBoxComponent* BoxCollision;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* DoorFrame;
	UPROPERTY(VisibleAnywhere, Category = "Mesh", Replicated)
	UStaticMeshComponent* LeftDoor;
	UPROPERTY(VisibleAnywhere, Category = "Mesh", Replicated)
	UStaticMeshComponent* RightDoor;
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OpenDoor();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSyncDoorState(bool bNewDoorState);
	UFUNCTION()
	void OnRep_bIsOpen();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Door")
	UCurveFloat* CurveFloat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool bIsOpen = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	float DoorRotateAngle = 90.f;

	UFUNCTION()
	void ControlDoor(float Value);

};
