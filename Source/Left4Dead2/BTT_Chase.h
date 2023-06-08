// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Chase.generated.h"

/**
 * 
 */
UCLASS()
class LEFT4DEAD2_API UBTT_Chase : public UBTTaskNode
{
	GENERATED_BODY()
	

public:
	UBTT_Chase();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
