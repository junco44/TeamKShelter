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

	bReplicates = true;
	bAlwaysRelevant = true;
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

void ACollapsingFloor::FloorCollapsing()
{
	if (HasAuthority())
	{
		Server_FloorCollapsing();
	}
}

void ACollapsingFloor::Server_FloorCollapsing_Implementation()
{
	Multicast_FloorCollapsing();
}

bool ACollapsingFloor::Server_FloorCollapsing_Validate()
{
	return true;
}

void ACollapsingFloor::Multicast_FloorCollapsing_Implementation()
{
	Floor->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Floor->SetVisibility(false);
}

void ACollapsingFloor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACollapsingFloor, Floor);
}


