// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Chase.h"
#include "AIController_CPP.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BlackBoard_Keys.h"

UBTTask_Chase::UBTTask_Chase(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("BTTTask_Chase_CPP");
}

EBTNodeResult::Type UBTTask_Chase::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	// AI 컨트롤러를 통해 TargetLocation 가져오기
	AAIController_CPP* const Controller = Cast<AAIController_CPP>(owner_comp.GetAIOwner());
	FVector const player_location = Controller->get_blackboard()->GetValueAsVector(BB_Keys::Target_Location);

	// 캐릭터 위치로 이동
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, player_location);

	// 성공으로 마무리

	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
