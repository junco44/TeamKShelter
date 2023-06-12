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
	ACharacter* const PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (OtherActor != this)
	{
		if (OtherActor && PlayerCharacter && PlayerCharacter->IsA<ASystemChar>())
		{
			HasOverlapped = true;
			UE_LOG(LogTemp, Warning, TEXT("Begin Overlap : Called"));

			do
			{
				UGameplayStatics::ApplyDamage(PlayerCharacter, 3.0f, ABasicZombie::GetController(), nullptr, NULL);

				UE_LOG(LogTemp, Warning, TEXT("Damage Applied to Player : Called"));
			} while (AttackTimer >= 0.8f);
		}
	}
}

void ABasicZombie::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* const PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (OtherActor != this)
	{
		if (OtherActor && PlayerCharacter && PlayerCharacter->IsA<ASystemChar>())
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
	ACharacter* const PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AController* const PlayerController = PlayerCharacter->GetController();
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	
	if (DamageCauser && PlayerCharacter && PlayerCharacter->IsA<ASystemChar>() && EventInstigator && PlayerController)
	{
		//UGameplayStatics::GetPlayerController(PlayerCharacter->GetController(), 0);
		
		CurHP = CurHP - Damage;

		UE_LOG(LogTemp, Warning, TEXT("CurHP = CurHP - Damage : Called"));
		
		do
		{
			UE_LOG(LogTemp, Warning, TEXT("CurHP = 0.0, flag == true : Called"));
			StartCount = true;
			flag = true;

			Body->SetSimulatePhysics(true);
			GetCharacterMovement()->StopMovementImmediately();
			GetCharacterMovement()->StopActiveMovement();

		} while (CurHP <= 0.0f && flag == false);
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

		if (AttackTimer >= 0.8f)
		{
			AttackTimer = 0;
		}
	}

}

// Called to bind functionality to input
void ABasicZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ABasicZombie::CheckHP_Validate(bool Check)
{
	Check = StartCount;
	if (Check == true)
	{
		return true;
	}
	return false;
}

void ABasicZombie::CheckHP_Implementation(bool Check)
{
	Check = StartCount;
	if (Check == true && flag == false)
	{
		flag = true;
		Body->SetSimulatePhysics(true);
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->StopActiveMovement();
	}
}

void ABasicZombie::UpdateSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}