// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShootingPlayerState_CPP.h"
#include "AIController.h"
#include "SystemChar.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIController_CPP.generated.h"

/**
 * 
 */
UCLASS()
class LEFT4DEAD2_API AAIController_CPP : public AAIController
{
	GENERATED_BODY()
public:

	AAIController_CPP(FObjectInitializer const& object_initializer = FObjectInitializer::Get());
	void BeginPlay() override;
	void OnPossess(APawn* const pawn) override;

	int32 Detected_Actor = 0;
	AShootingPlayerState_CPP* PlayerState_CPP;
	ASystemChar* PlayerCharacter;

	class UBlackboardComponent* get_blackboard() const;

private:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* behavior_tree_component;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* bTree;

	class UBlackboardComponent* blackboard;

	class UAISenseConfig_Sight* sight_config;

	UFUNCTION()
	void on_target_detected(AActor* actor, FAIStimulus const stimulus);

	void setup_perception_system();


};