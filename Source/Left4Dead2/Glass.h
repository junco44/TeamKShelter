// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Glass.generated.h"

UCLASS()
class LEFT4DEAD2_API AGlass : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGlass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		class UStaticMeshComponent* Glass;
	UPROPERTY(VisibleAnywhere, Category = "Collision")
		class UBoxComponent* BoxCollision;

protected:

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
