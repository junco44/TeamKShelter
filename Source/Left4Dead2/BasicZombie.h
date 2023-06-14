// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SystemChar.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "BasicZombie.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class LEFT4DEAD2_API ABasicZombie : public ACharacter //public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicZombie();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Variable")
	float MaxHP = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Variable")
	float CurHP = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Variable")
	bool HasOverlapped = false;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Replicated, Category = "Variable")
	float Timer = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Replicated, Category = "Variable")
	float AttackTimer = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Replicated, Category = "Variable")
	bool StartCount = false;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Replicated, Category = "Variable")
	bool flag = false;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_CheckHP(bool CheckCount, bool Checkflag);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CheckHP(bool CheckCount, bool Checkflag);

	void CheckHP(bool CheckCount, bool Checkflag);

	UFUNCTION(BlueprintCallable, Category = "Function")
	void UpdateSpeed(UPARAM(DisplayName = "Speed") float Speed);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int32 ID = 0;

	//virtual FGenericTeamId GetGenericTeamId() const override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	//FGenericTeamId TeamID_Zombie;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	ABasicZombie* BasicZombie_CPP;
	ASystemChar* PlayerCharacter_CPP;
	ACharacter* TargetCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Body;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* LeftArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* RightArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Leg;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* Box;

};
