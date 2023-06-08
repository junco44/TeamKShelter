// Fill out your copyright notice in the Description page of Project Settings.


#include "Door2.h"
//#include "Juno_CPPCharacter.h"
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

}

// Called when the game starts or when spawned
void ADoor2::BeginPlay()
{
	Super::BeginPlay();
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADoor2::OpenDoor);
		Timeline.AddInterpFloat(CurveFloat, TimelineProgress);
	}
}

// Called every frame
void ADoor2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);
}

void ADoor2::MyInteract_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Interacted with Door!"));

	if (bIsDoorClosed)
	{
		SetDoorOnSameSide();
		Timeline.Play();
	}
	else
	{
		Timeline.Reverse();
	}

	bIsDoorClosed = !bIsDoorClosed;
}

void ADoor2::OpenDoor(float Value)
{
	float Angle = bDoorOnSameSide ? -DoorRotateAngle : DoorRotateAngle;

	FRotator Rot1 = FRotator(0.f, Angle * Value, 0.f);
	FRotator Rot2 = FRotator(0.f, 180 - Angle * Value, 0.f);

	LeftDoor->SetRelativeRotation(Rot1);
	RightDoor->SetRelativeRotation(Rot2);
	//UE_LOG(LogTemp, Display, TEXT("Rotate"));
}

void ADoor2::SetDoorOnSameSide()
{
	/*if (Character)
	{
		FVector CharacterFV = Character->GetActorForwardVector();
		FVector DoorFV = GetActorForwardVector();
		bDoorOnSameSide = FVector::DotProduct(CharacterFV, DoorFV) >= 0;
	}*/
}
