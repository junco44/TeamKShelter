// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Components/BoxComponent.h"

// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;
	BoxCollision->SetBoxExtent(FVector(10.0f, 53.0f, 110.0f));

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	DoorFrame->SetupAttachment(RootComponent);

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(DoorFrame);

	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));

	bReplicates = true;
	SetReplicateMovement(true);
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
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
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//FString value;
	//UEnum::GetValueAsString(GetLocalRole(), value);
	//DrawDebugString(GetWorld(), FVector(0, 0, 100), value, this, FColor::Green, DeltaTime);
}

void ADoor::MyInteract_Implementation()
{
	if (HasAuthority())
	{
		OpenDoor();
	}
	UE_LOG(LogTemp, Warning, TEXT("Interacted with Door!"));
}

void ADoor::OpenDoor()
{
	Server_OpenDoor();
}

void ADoor::Server_OpenDoor_Implementation()
{
	MulticastSyncDoorState(bIsOpen);
}

bool ADoor::Server_OpenDoor_Validate()
{
	return true;
}

void ADoor::MulticastSyncDoorState_Implementation(bool bNewDoorState)
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

void ADoor::OnRep_bIsOpen()
{
	MulticastSyncDoorState(bIsOpen);
}

void ADoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoor, Door);
	DOREPLIFETIME_CONDITION(ADoor, bIsOpen, COND_SkipOwner);
}

void ADoor::ControlDoor(float Value)
{
	FRotator DoorInitialRotation = FRotator(0.f, 0.f, 0.f);
	FRotator DoorTargetRoatation = FRotator(0.f, DoorRotateAngle, 0.f);

	FRotator Rot = FMath::Lerp(DoorInitialRotation, DoorTargetRoatation, Value);

	Door->SetRelativeRotation(Rot);
}

