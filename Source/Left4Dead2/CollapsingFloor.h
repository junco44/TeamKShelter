// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Net/UnrealNetwork.h"
#include "CollapsingFloor.generated.h"

UCLASS()
class LEFT4DEAD2_API ACollapsingFloor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACollapsingFloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FloorCollapsing();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Mesh", Replicated)
	UStaticMeshComponent* Floor;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_FloorCollapsing();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_FloorCollapsing();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
