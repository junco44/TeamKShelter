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

	// ��Ʈ��ũ ���� ��� Ȱ��ȭ
	bReplicates = true;
	SetReplicateMovement(true);
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
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
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// ������ ���� ������ ����� ���ڿ��� �׷���
	FString value;
	UEnum::GetValueAsString(GetLocalRole(), value);
	DrawDebugString(GetWorld(), FVector(0, 0, 100), value, this, FColor::Green, DeltaTime);
}

void ADoor::MyInteract_Implementation()
{
	// �������� �Լ��� ȣ��Ǵ��� Ȯ��
	if (HasAuthority())
	{
		OpenDoor();
	}
}

// Server_OpenDoor �Լ� ȣ��
void ADoor::OpenDoor()
{
	Server_OpenDoor();
	UE_LOG(LogTemp, Warning, TEXT("Interacted with Door!"));
}

// MulticastSyncDoorState �Լ� ȣ���Ͽ� ���� �� ���� ����ȭ
void ADoor::Server_OpenDoor_Implementation()
{
	MulticastSyncDoorState(bIsOpen);
}

// Server_OpenDoor �Լ��� ��ȿ���� ����
// Ŭ���̾�Ʈ�� �ش� �Լ��� ȣ���� ������ �ִ��� �����ϰ�, true�� ��ȯ
bool ADoor::Server_OpenDoor_Validate()
{
	return true;
}

// �� ���� ������Ʈ �� ���¿� ���� Ÿ�Ӷ��� ���/�����
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

	// �� ���� ���
	bIsOpen = !bIsOpen;
}

// bIsOpen ������ �������� ����Ǿ��� �� MulticastSyncDoorState �Լ� ȣ��
void ADoor::OnRep_bIsOpen()
{
	MulticastSyncDoorState(bIsOpen);
}

// �θ� Ŭ������ GetLifetimeReplicatedProps �Լ��� ȣ���Ͽ� �⺻ ���� ������ ������
void ADoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Door�� bIsOpen ���� ���� ����
	// DOREPLIFETIME ��ũ�δ� ������ �ش� ������ ���¸� �����ϵ��� �����ϴ� ����
	// ���� ������ �߰��ϸ� ������ Ŭ���̾�Ʈ ���� �ش� ������ ���� ���°� ����ȭ��
	DOREPLIFETIME(ADoor, Door);
	DOREPLIFETIME_CONDITION(ADoor, bIsOpen, COND_SkipOwner);
	// COND_SkipOwner ������ �����Ͽ� ������ �� �����ڸ� ������ �ٸ� ��� Ŭ���̾�Ʈ���� ����ȭ
}

void ADoor::ControlDoor(float Value)
{
	// ���� �ʱ� ȸ������ ��ǥ ȸ���� ����
	FRotator DoorInitialRotation = FRotator(0.f, 0.f, 0.f);
	FRotator DoorTargetRoatation = FRotator(0.f, DoorRotateAngle, 0.f);

	// Value �Ű������� ����Ͽ� �ʱ� ȸ������ ��ǥ ȸ���� ���̸� ����
	FRotator Rot = FMath::Lerp(DoorInitialRotation, DoorTargetRoatation, Value);

	// Door ������Ʈ�� ��� ȸ���� ����
	Door->SetRelativeRotation(Rot);
}

