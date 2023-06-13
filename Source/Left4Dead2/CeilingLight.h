// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/RectLightComponent.h"
#include "Net/UnrealNetwork.h"
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Function")
	void LightOff();
	
	UFUNCTION(BlueprintCallable, Category = "Function")
	void LightOn();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* LightBody;
	UPROPERTY(VisibleAnywhere, Category = "Mesh", Replicated)
	class UStaticMeshComponent* Light;
	UPROPERTY(VisibleAnywhere, Category = "Light", Replicated)
	class URectLightComponent* LightComp;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_LightOff();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_LightOff();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_LightOn();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_LightOn();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
