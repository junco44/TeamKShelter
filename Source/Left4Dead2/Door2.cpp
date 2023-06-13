// Fill out your copyright notice in the Description page of Project Settings.


#include "Door2.h"
#include "Components/BoxComponent.h"

// Sets default values
ADoor2::ADoor2()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	DoorFrame->SetupAttachment(RootComponent);

	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
	LeftDoor->SetupAttachment(DoorFrame);

	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
	RightDoor->SetupAttachment(DoorFrame);

	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));

	bReplicates = true;
	SetReplicateMovement(true);
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void ADoor2::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("ControlDoor"));
		DoorTimeline->AddInterpFloat(CurveFloat, TimelineProgress);
	}
}

// Called every frame
void ADoor2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FString value;
	//UEnum::GetValueAsString(GetLocalRole(), value);
	//DrawDebugString(GetWorld(), FVector(0, 0, 100), value, this, FColor::Green, DeltaTime);
}

void ADoor2::MyInteract_Implementation()
{
	if (HasAuthority())
	{
		OpenDoor();
	}
}

void ADoor2::OpenDoor()
{
	Server_OpenDoor();
	UE_LOG(LogTemp, Warning, TEXT("Interacted with Door!"));
}

void ADoor2::Server_OpenDoor_Implementation()
{
	MulticastSyncDoorState(bIsOpen);
}

bool ADoor2::Server_OpenDoor_Validate()
{
	return true;
}

void ADoor2::MulticastSyncDoorState_Implementation(bool bNewDoorState)
{
	bIsOpen = bNewDoorState;
	if (bIsOpen)
	{
		if (!DoorTimeline->IsPlaying())
		{
			DoorTimeline->PlayFromStart();
		}
	}
	else
	{
		if (!DoorTimeline->IsPlaying())
		{
			DoorTimeline->ReverseFromEnd();
		}
	}
	bIsOpen = !bIsOpen;
}

void ADoor2::OnRep_bIsOpen()
{
	MulticastSyncDoorState(bIsOpen);
}

void ADoor2::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoor2, LeftDoor);
	DOREPLIFETIME(ADoor2, RightDoor);
	DOREPLIFETIME_CONDITION(ADoor2, bIsOpen, COND_SkipOwner);
}

void ADoor2::ControlDoor(float Value)
{
	FRotator DoorInitialRotation1 = FRotator(0.f, 0.f, 0.f);
	FRotator DoorInitialRotation2 = FRotator(0.f, -180.f, 0.f);
	FRotator DoorTargetRoatation1 = FRotator(0.f, DoorRotateAngle, 0.f);
	FRotator DoorTargetRoatation2 = FRotator(0.f, -180.f -DoorRotateAngle, 0.f);

	FRotator Rot1 = FMath::Lerp(DoorInitialRotation1, DoorTargetRoatation1, Value);
	FRotator Rot2 = FMath::Lerp(DoorInitialRotation2, DoorTargetRoatation2, Value);

	LeftDoor->SetRelativeRotation(Rot1);
	RightDoor->SetRelativeRotation(Rot2);
}