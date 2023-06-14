// Fill out your copyright notice in the Description page of Project Settings.


#include "Car.h"
#include "Components/BoxComponent.h"

bool ACar::bIsHornSoundPlaying = false;

// Sets default valu
ACar::ACar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Car = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Car"));
    RootComponent = Car;

    // 초기화되지 않은 멤버 변수 초기화
    Sound = nullptr;
    AlarmTime = 0.0f;
}

// Called when the game starts or when spawned
void ACar::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACar::CarAlarm()
{
    if (!bIsHornSoundPlaying)
    {
        // 알람 소리의 최대 거리를 500 유닛으로 설정
        SetAlarmSoundAttenuation(2000.f);

        //UGameplayStatics::PlaySoundAtLocation(this, Sound, GetActorLocation(), 1.f);
        bIsHornSoundPlaying = true;

        FTimerHandle TimerHandle;
        GetWorldTimerManager().SetTimer(TimerHandle, this, &ACar::StopAlarm, AlarmTime, false);
    }
}

void ACar::SetAlarmSoundAttenuation(float MaxDistance)
{
    if (Sound)
    {
        USoundAttenuation* AttenuationSettings = NewObject<USoundAttenuation>(this, USoundAttenuation::StaticClass());
        AttenuationSettings->Attenuation.FalloffDistance = MaxDistance;
        Sound->AttenuationSettings = AttenuationSettings;

        UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, Sound, GetActorLocation());
    }
}

void ACar::StopAlarm()
{
    bIsHornSoundPlaying = false;
}

