// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
//#include "Juno_CPPCharacter.h"
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

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADoor::OpenDoor);
		Timeline.AddInterpFloat(CurveFloat, TimelineProgress);
	}
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);
}

void ADoor::MyInteract_Implementation()
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

void ADoor::OpenDoor(float Value)
{
	float Angle = bDoorOnSameSide ? -DoorRotateAngle : DoorRotateAngle;

	FRotator Rot = FRotator(0.f, Angle * Value, 0.f);

	Door->SetRelativeRotation(Rot);

}

void ADoor::SetDoorOnSameSide()
{
	//if (Character)
	//{
		//FVector CharacterFV = Character->GetActorForwardVector();
		//FVector DoorFV = GetActorForwardVector();
		//bDoorOnSameSide = FVector::DotProduct(CharacterFV, DoorFV) >= 0;
	//}
	UE_LOG(LogTemp, Warning, TEXT("%d"), bDoorOnSameSide);
}

