// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CeilingLight.generated.h"

UCLASS()
class LEFT4DEAD2_API ACeilingLight : public AActor
{
	GENERATED_BODY()

		friend class AElevatorButton;
	friend class AGenerator;
public:
	// Sets default values for this actor's properties
	ACeilingLight();

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		class UStaticMeshComponent* LightBody;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		class UStaticMeshComponent* Light;
	UPROPERTY(VisibleAnywhere, Category = "Light")
		class URectLightComponent* LightComp;
	UPROPERTY(VisibleAnywhere, Category = "Light")
		class URectLightComponent* LightComp2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UFUNCTION(BlueprintCallable, Category = "Function")
		void LightOff();
	UFUNCTION(BlueprintCallable, Category = "Function")
		void LightOn();

};
