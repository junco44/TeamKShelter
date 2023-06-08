// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FindRandomPatrolLocation.h"
#include "BasicZombie.h"
#include "AIController_BasicZombie.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTT_FindRandomPatrolLocation::UBTT_FindRandomPatrolLocation()
{
	NodeName = TEXT("FindRandomPatrol_CPP");
}

EBTNodeResult::Type UBTT_FindRandomPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	UE_LOG(LogTemp, Warning, TEXT("Start patrol : Called"));

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot Find Vaild Pawn : Called"));

		return EBTNodeResult::Failed;
	}

	ABasicZombie* BasicZombie = Cast<ABasicZombie>(OwnerComp.GetAIOwner()->GetPawn());
	BasicZombie->UpdateSpeed(150.0f);

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot Find NavMesh : Called"));

		return EBTNodeResult::Failed;
	}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AAIController_BasicZombie::Key_PatrolLocation_Home);
	FNavLocation NextPatrol;
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 2000.0f, NextPatrol))
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Vaild Location : Called"));
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AAIController_BasicZombie::Key_PatrolLocation_Target, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;


	//return EBTNodeResult::Type();
}
