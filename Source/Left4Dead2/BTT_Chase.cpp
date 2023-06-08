// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Chase.h"
#include "BasicZombie.h"
#include "SystemChar.h"
#include "AIController_BasicZombie.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTT_Chase::UBTT_Chase()
{
	NodeName = TEXT("Chase_CPP");
}

EBTNodeResult::Type UBTT_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	UE_LOG(LogTemp, Warning, TEXT("Start Chase : Called"));

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	ABasicZombie* BasicZombie = Cast<ABasicZombie>(OwnerComp.GetAIOwner()->GetPawn());
	BasicZombie->UpdateSpeed(700.0f);

	ASystemChar* PlayerCharactor = Cast<ASystemChar>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AAIController_BasicZombie::Key_EnemyActor));
	if (nullptr == PlayerCharactor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find Player : Called"));
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;

	//return EBTNodeResult::Type();
}
