// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "AudioDevice.h"
#include "Car.generated.h"

UCLASS()
class LEFT4DEAD2_API ACar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void CarAlarm();

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void SetAlarmSoundAttenuation(float MaxDistance);

private:
	static bool bIsHornSoundPlaying;

	UFUNCTION()
	void StopAlarm();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* Car;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundWave* Sound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	float AlarmTime;

};
