// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_BasicZombie.h"
#include "SystemChar.h"
#include "BasicZombie.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Damage.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Hearing.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "EngineGlobals.h"



const FName AAIController_BasicZombie::Key_HasLineOfSight(TEXT("HasLineOfSight"));
const FName AAIController_BasicZombie::Key_EnemyActor(TEXT("EnemyActor"));
const FName AAIController_BasicZombie::Key_PatrolLocation_Home(TEXT("PatrolLocation_Home"));
const FName AAIController_BasicZombie::Key_PatrolLocation_Target(TEXT("PatrolLocation_Target"));


void AAIController_BasicZombie::BeginPlay()
{
	Super::BeginPlay();
}

AAIController_BasicZombie::AAIController_BasicZombie(const FObjectInitializer& ObjectInitializer)
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
	
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	Sight->SightRadius = 1000.0f;
	Sight->LoseSightRadius = Sight->SightRadius + 500.0f;
	Sight->PeripheralVisionAngleDegrees = 80.0f;
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComponent->ConfigureSense(*Sight);
	AIPerceptionComponent->SetDominantSense(Sight->GetSenseImplementation());

	static ConstructorHelpers::FObjectFinder<UBlackboardData>BBObject(TEXT("/Game/TEAM_K/Left4Dead/KIM_YoonHan/BlackBoard/BlackboardData_BasicZombie.BlackboardData_BasicZombie"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>BTObject(TEXT("/Game/TEAM_K/Left4Dead/KIM_YoonHan/Behaviour_Tree/BehaviorTree_BasicZombie.BehaviorTree_BasicZombie"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}


}

void AAIController_BasicZombie::OnPerception(AActor* Actor, FAIStimulus Stimulus)
{
	ASystemChar* PlayerCharacter = Cast<ASystemChar>(Actor);

	if (PlayerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter Not Found : Called"));
		BlackboardComp->SetValueAsBool(Key_HasLineOfSight, false);
		BlackboardComp->SetValueAsObject(Key_EnemyActor, nullptr);

		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter Found : Called"));

		SetFocus(Stimulus.WasSuccessfullySensed() ? PlayerCharacter : nullptr);
		if (Stimulus.WasSuccessfullySensed() != NULL)
		{
			if (PlayerCharacter->ActorHasTag("Player"))
			{ 
				BlackboardComp->SetValueAsBool(Key_HasLineOfSight, true);
				BlackboardComp->SetValueAsObject(Key_EnemyActor, PlayerCharacter);
			}
		}
		else
		{

			BlackboardComp->SetValueAsBool(Key_HasLineOfSight, false);
			BlackboardComp->SetValueAsObject(Key_EnemyActor, nullptr);

		}
	}
}

void AAIController_BasicZombie::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		BlackboardComp->SetValueAsVector(Key_PatrolLocation_Home, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Warning, TEXT("AIController couldn't run behavior tree!"));
		}
	}

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIController_BasicZombie::OnPerception);

}





