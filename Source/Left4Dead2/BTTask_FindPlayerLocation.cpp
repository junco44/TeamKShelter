// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "AIController_CPP.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "BlackBoard_Keys.h"


UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("FindPlayerLocation_CPP");
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	// 플레이어 캐릭터 & AI Controller 가져오기 
	auto const Controller = Cast<AAIController_CPP>(owner_comp.GetAIOwner());
	auto const Enemy = Controller->GetCharacter();

	if (Enemy && Enemy->IsA<ABasicZombie>())
	{
		BasicZombie = Cast<ABasicZombie>(Enemy);
		BasicZombie->UpdateSpeed(700.0f);
	}

	ACharacter* TargetCharacter = (ACharacter*)Controller->get_blackboard()->GetValueAsObject(BB_Keys::Nearest_Player);

	//ACharacter* const PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), *PlayerIndex);

	// 플레이어 캐릭터의 위치 가져오기
	FVector const player_location = TargetCharacter->GetActorLocation();
	if (search_random)
	{
		FNavLocation NavLocation;

		// 내비게이션 시스템 가저오기 & 내비메쉬 위의 플레이어 캐릭터 근처로 이동
		UNavigationSystemV1* const nav_system = UNavigationSystemV1::GetCurrent(GetWorld());
		if (nav_system->GetRandomPointInNavigableRadius(player_location, search_radius, NavLocation, nullptr))
		{
			Controller->get_blackboard()->SetValueAsVector(BB_Keys::Target_Location, NavLocation.Location);
		}
	}
	else
	{
		Controller->get_blackboard()->SetValueAsVector(BB_Keys::Target_Location, player_location);
	}

	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
