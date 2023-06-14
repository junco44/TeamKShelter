// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicZombie.h"
#include "SystemChar.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Net/UnrealNetwork.h>
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"

void ABasicZombie::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABasicZombie, CurHP, COND_None);
	DOREPLIFETIME_CONDITION(ABasicZombie, Timer, COND_None);
	DOREPLIFETIME_CONDITION(ABasicZombie, AttackTimer, COND_None);
	DOREPLIFETIME_CONDITION(ABasicZombie, StartCount, COND_None);
	DOREPLIFETIME_CONDITION(ABasicZombie, flag, COND_None);
	DOREPLIFETIME_CONDITION(ABasicZombie, HasOverlapped, COND_None);
}

// Sets default values
ABasicZombie::ABasicZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;


	ActorHasTag("Zombie");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));
	Body->SetupAttachment(GetMesh());

	LeftArm = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftArm"));
	LeftArm->SetupAttachment(Body);
	LeftArm->AddLocalOffset(FVector(80.0f, 60.0f, 10.0f));

	RightArm = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightArm"));
	RightArm->SetupAttachment(Body);
	RightArm->AddLocalOffset(FVector(80.0f, -60.0f, 10.0f));

	Leg = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Leg"));
	Leg->SetupAttachment(Body);
	Leg->AddLocalOffset(FVector(-0.0f, 0.0f, -55.0f));

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBox"));
	Box->SetupAttachment(Body);
	Box->AddLocalOffset(FVector(168.8f, -0.0f, 10.4f));
	Box->OnComponentBeginOverlap.AddDynamic(this, &ABasicZombie::OnOverlapBegin);
	Box->OnComponentEndOverlap.AddDynamic(this, &ABasicZombie::OnOverlapEnd);

}

void ABasicZombie::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	for (int32 i = 0; i < 4; i++)
	{
		ACharacter* const PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), i);
		
		TargetCharacter = PlayerCharacter;
	}

	if (OtherActor != this)
	{
		if (OtherActor && TargetCharacter && TargetCharacter->IsA<ASystemChar>())
		{
			HasOverlapped = true;
			UE_LOG(LogTemp, Warning, TEXT("Begin Overlap : Called"));

			do
			{
				UGameplayStatics::ApplyDamage(TargetCharacter, 3.0f, ABasicZombie::GetController(), nullptr, NULL);

				//UE_LOG(LogTemp, Warning, TEXT("Damage Applied to Player : Called"));
			} while (AttackTimer >= 1.0f);
		}
	}
}

void ABasicZombie::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	for (int32 i = 0; i < 4; i++)
	{
		ACharacter* const PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), i);

		TargetCharacter = PlayerCharacter;
	}

	if (OtherActor != this)
	{
		if (OtherActor && TargetCharacter && TargetCharacter->IsA<ASystemChar>())
		HasOverlapped = false;
		UE_LOG(LogTemp, Warning, TEXT("End Overlap : Called"));
	}

}

//FGenericTeamId ABP_Basic_Zombie::GetGenericTeamId() const
//{
//	return TeamID_Zombie;
//}


// Called when the game starts or when spawned
void ABasicZombie::BeginPlay()
{
	Super::BeginPlay();
}

float ABasicZombie::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	for (int32 i = 0; i < 4; i++)
	{
		ACharacter* const PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), i);

		TargetCharacter = PlayerCharacter;
	}

	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	
	if (EventInstigator && TargetCharacter && TargetCharacter->IsA<ASystemChar>())
	{
		
		CurHP = CurHP - Damage;

		//UE_LOG(LogTemp, Warning, TEXT("CurHP = CurHP - Damage : Called"));
		
		if (CurHP <= 0.0f && flag == false)
		{
			//UE_LOG(LogTemp, Warning, TEXT("CurHP = 0.0, flag == true : Called"));
			StartCount = true;
			flag = true;

			CheckHP(StartCount, flag);
		}
	}

	return 0.0f;
}

// Called every frame
void ABasicZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (StartCount == true)
	{
		Timer = Timer + DeltaTime;

		if (Timer >= 2)
		{
			Destroy(false, true);
			StartCount = false;
			Timer = 0;
		}
	}

	if (HasOverlapped == true)
	{
		AttackTimer = AttackTimer + DeltaTime;

		if (AttackTimer >= 1.0f)
		{
			AttackTimer = 0.0f;
		}
	}

}

// Called to bind functionality to input
void ABasicZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ABasicZombie::Server_CheckHP_Validate(bool CheckCount, bool Checkflag)
{
	return true;
}

void ABasicZombie::Server_CheckHP_Implementation(bool CheckCount, bool Checkflag)
{
	if (CheckCount && Checkflag)
	{
		Multicast_CheckHP(CheckCount, Checkflag);
	}
}

void ABasicZombie::Multicast_CheckHP_Implementation(bool CheckCount, bool Checkflag)
{
	StartCount = CheckCount;
	flag = Checkflag;

	if (StartCount == true && flag == true)
	{
		Body->SetSimulatePhysics(true);
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->StopActiveMovement();
	}
}

void ABasicZombie::CheckHP(bool CheckCount, bool Checkflag)
{
	if (HasAuthority())
	{
		Server_CheckHP(CheckCount, Checkflag);
	}
	else
	{
		StartCount = CheckCount;
		flag = Checkflag;
	}
}


void ABasicZombie::UpdateSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}