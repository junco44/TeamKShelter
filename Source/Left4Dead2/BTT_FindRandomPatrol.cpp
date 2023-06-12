// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FindRandomPatrol.h"
#include "BasicZombie.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "AIController_CPP.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Blackboard_Keys.h"

UBTT_FindRandomPatrol::UBTT_FindRandomPatrol(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("FindRandomPatrol_CPP");
}

EBTNodeResult::Type UBTT_FindRandomPatrol::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	// Ai 컨트롤러 & Ai 컨트롤러를 소유한 캐릭터(폰) 찾기
	auto const Controller = Cast<AAIController_CPP>(owner_comp.GetAIOwner());
	auto const Enemy = Controller->GetCharacter();

	if (Enemy && Enemy->IsA<ABasicZombie>())
	{
		BasicZombie = Cast<ABasicZombie>(Enemy);
		BasicZombie->UpdateSpeed(150.0f);
	}
	// Ai 현재 위치 가져오기
	FVector const origin = Enemy->GetActorLocation();
	FNavLocation NavLocation;

	// 내비게이션 시스템 가져오기 & 월드에 배치된 NavMesh에서 랜덤한 위치 생성
	UNavigationSystemV1* const nav_system = UNavigationSystemV1::GetCurrent(GetWorld());
	if (nav_system->GetRandomPointInNavigableRadius(origin, search_radius, NavLocation, nullptr))
	{
		Controller->get_blackboard()->SetValueAsVector(BB_Keys::Target_Location, NavLocation.Location);
	}

	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
