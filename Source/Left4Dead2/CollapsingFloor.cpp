// Fill out your copyright notice in the Description page of Project Settings.


#include "CollapsingFloor.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
ACollapsingFloor::ACollapsingFloor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	RootComponent = Floor;

	CollapsedFloor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollapsedFloor"));
	CollapsedFloor->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACollapsingFloor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACollapsingFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollapsingFloor::FloorCollapsing1()
{
	Floor->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Floor->SetVisibility(false);
}

void ACollapsingFloor::FloorCollapsing2()
{
	Floor->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Floor->SetVisibility(false);
}

