// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SystemChar.h"
#include "ShootingPlayerState_CPP.generated.h"

/**
 * 
 */
UCLASS()
class LEFT4DEAD2_API AShootingPlayerState_CPP : public APlayerState
{
	GENERATED_BODY()
	
public:
	AShootingPlayerState_CPP();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Health")
	float MaxHP_CPP = 0.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Health", ReplicatedUsing = OnRep_CurHP_CPP)
	float CurHP_CPP = 100.0f;
	UPROPERTY(editAnyWhere, BlueprintReadWrite, Category = "Health")
	bool Player_IsDead;


	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetCurHP(float NewHP);

	void SetCurHP(float NewHP);

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Health")
	void OnRep_CurHP_CPP();

};
