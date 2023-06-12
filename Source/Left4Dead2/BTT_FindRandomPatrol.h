// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicZombie.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UObject/UObjectGlobals.h"
#include "BTT_FindRandomPatrol.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class LEFT4DEAD2_API UBTT_FindRandomPatrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UBTT_FindRandomPatrol(FObjectInitializer const& object_initializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory);

	ABasicZombie* BasicZombie;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowProtectedAccess = "true"))
	float search_radius = 1500.0f;

};
