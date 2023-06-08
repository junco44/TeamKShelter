// Fill out your copyright notice in the Description page of Project Settings.


#include "ElevatorButton.h"
#include "Components/BoxComponent.h"
#include "CeilingLight.h"
#include "CollapsingFloor.h"
#include "Elevator.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AElevatorButton::AElevatorButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;

	Button = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button"));
	Button->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AElevatorButton::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> OutArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACeilingLight::StaticClass(), OutArray);
	for (AActor* Actor : OutArray)
	{
		LightArray.Add(Cast<ACeilingLight>(Actor));
	}

}

// Called every frame
void AElevatorButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AElevatorButton::MyInteract_Implementation()
{
	if (bDidTask)
	{
		if (LightArray.Num() > 0)
		{
			for (ACeilingLight* Light : LightArray)
			{
				Light->LightOff();
			}
		}

		if (CollapsingFloor)
		{
			CollapsingFloor->FloorCollapsing1();
			UE_LOG(LogTemp, Warning, TEXT("The Room4(1st)'s floor is Collapsed!"));
			UE_LOG(LogTemp, Warning, TEXT("You should solve the Task!"));
		}

		if (Elevator)
		{
			Elevator->LightOff();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Now you're good to go!"));

		if (Elevator)
		{
			Elevator->DoorTimeline->PlayFromStart();
		}
	}

}

void AElevatorButton::SetTask(bool bFlag)
{
	bDidTask = bFlag;

}

