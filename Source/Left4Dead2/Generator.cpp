// Fill out your copyright notice in the Description page of Project Settings.

#include "Generator.h"


// Sets default values
AGenerator::AGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;

	Generator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Generator"));
	Generator->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGenerator::BeginPlay()
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
void AGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGenerator::MyInteract_Implementation()
{
	GeneratorOn();

	if (CollapsingFloor)
	{
		CollapsingFloor->FloorCollapsing2();
		UE_LOG(LogTemp, Warning, TEXT("The floor(1st) is Collapsed!"));
	}

	if (TaskCheck)
	{
		TaskCheck->SetTask(false);
		UE_LOG(LogTemp, Warning, TEXT("TaskCheck"));
	}

	if (Elevator)
	{
		Elevator->LightOn();
	}
}

void AGenerator::GeneratorOn()
{
	if (LightArray.Num() > 0)
	{
		for (ACeilingLight* Light : LightArray)
		{
			Light->LightOn();
		}
	}
}