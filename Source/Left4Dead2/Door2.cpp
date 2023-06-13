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

	// ��Ʈ��ũ ���� ��� Ȱ��ȭ
	bReplicates = true;
	SetReplicateMovement(true);
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void ADoor2::BeginPlay()
{
	Super::BeginPlay();

	// CurveFloat�� ��ȿ�� ��� ControlDoor �Լ��� Ÿ�Ӷ����� float ���� �̺�Ʈ�� ���ε�
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

	// ������ ���� ������ ����� ���ڿ��� �׷���
	FString value;
	UEnum::GetValueAsString(GetLocalRole(), value);
	DrawDebugString(GetWorld(), FVector(0, 0, 100), value, this, FColor::Green, DeltaTime);
}

void ADoor2::MyInteract_Implementation()
{
	// �������� �Լ��� ȣ��Ǵ��� Ȯ��
	if (HasAuthority())
	{
		OpenDoor();
	}
}

// Server_OpenDoor �Լ� ȣ��
void ADoor2::OpenDoor()
{
	Server_OpenDoor();
	UE_LOG(LogTemp, Warning, TEXT("Interacted with Door!"));
}

// MulticastSyncDoorState �Լ� ȣ���Ͽ� ���� �� ���� ����ȭ
void ADoor2::Server_OpenDoor_Implementation()
{
	MulticastSyncDoorState(bIsOpen);
}

// Server_OpenDoor �Լ��� ��ȿ���� ����
// Ŭ���̾�Ʈ�� �ش� �Լ��� ȣ���� ������ �ִ��� �����ϰ�, true�� ��ȯ
bool ADoor2::Server_OpenDoor_Validate()
{
	return true;
}

// �� ���� ������Ʈ �� ���¿� ���� Ÿ�Ӷ��� ���/�����
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

	// �� ���� ���
	bIsOpen = !bIsOpen;
}

// bIsOpen ������ �������� ����Ǿ��� �� MulticastSyncDoorState �Լ� ȣ��
void ADoor2::OnRep_bIsOpen()
{
	MulticastSyncDoorState(bIsOpen);
}

// �θ� Ŭ������ GetLifetimeReplicatedProps �Լ��� ȣ���Ͽ� �⺻ ���� ������ ������
void ADoor2::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// LeftDoor, RightDoor�� bIsOpen ���� ���� ����
	// DOREPLIFETIME ��ũ�δ� ������ �ش� ������ ���¸� �����ϵ��� �����ϴ� ����
	// ���� ������ �߰��ϸ� ������ Ŭ���̾�Ʈ ���� �ش� ������ ���� ���°� ����ȭ��
	DOREPLIFETIME(ADoor2, LeftDoor);
	DOREPLIFETIME(ADoor2, RightDoor);
	DOREPLIFETIME_CONDITION(ADoor2, bIsOpen, COND_SkipOwner);
	// COND_SkipOwner ������ �����Ͽ� ������ �� �����ڸ� ������ �ٸ� ��� Ŭ���̾�Ʈ���� ����ȭ
}

void ADoor2::ControlDoor(float Value)
{
	// ���� �ʱ� ȸ������ ��ǥ ȸ���� ����
	FRotator DoorInitialRotation1 = FRotator(0.f, 0.f, 0.f);
	FRotator DoorInitialRotation2 = FRotator(0.f, -180.f, 0.f);
	FRotator DoorTargetRoatation1 = FRotator(0.f, DoorRotateAngle, 0.f);
	FRotator DoorTargetRoatation2 = FRotator(0.f, -180.f -DoorRotateAngle, 0.f);

	// Value �Ű������� ����Ͽ� �ʱ� ȸ������ ��ǥ ȸ���� ���̸� ����
	FRotator Rot1 = FMath::Lerp(DoorInitialRotation1, DoorTargetRoatation1, Value);
	FRotator Rot2 = FMath::Lerp(DoorInitialRotation2, DoorTargetRoatation2, Value);

	// Door ������Ʈ�� ��� ȸ���� ����
	LeftDoor->SetRelativeRotation(Rot1);
	RightDoor->SetRelativeRotation(Rot2);
}