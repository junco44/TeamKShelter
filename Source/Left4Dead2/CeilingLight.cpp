// Fill out your copyright notice in the Description page of Project Settings.


#include "CeilingLight.h"
#include "Components/MeshComponent.h"

// Sets default values
ACeilingLight::ACeilingLight()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LightBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightBody"));
	RootComponent = LightBody;

	Light = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Light"));
	Light->SetupAttachment(RootComponent);

	LightComp = CreateDefaultSubobject<URectLightComponent>(TEXT("LightComp"));
	LightComp->SetupAttachment(RootComponent);

	LightComp2 = CreateDefaultSubobject<URectLightComponent>(TEXT("LightComp2"));
	LightComp2->SetupAttachment(LightComp);

}

// Called when the game starts or when spawned
void ACeilingLight::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACeilingLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACeilingLight::LightOff()
{
	Server_LightOff();
}

void ACeilingLight::Server_LightOff_Implementation()
{
	Multicast_LightOff();
}

bool ACeilingLight::Server_LightOff_Validate()
{
	return true;
}

void ACeilingLight::Multicast_LightOff_Implementation()
{
	LightComp->SetIntensity(0.0f);
	Light->SetVisibility(false);
}

void ACeilingLight::LightOn()
{
	Server_LightOn();
}

void ACeilingLight::Server_LightOn_Implementation()
{
	Multicast_LightOn();
}

bool ACeilingLight::Server_LightOn_Validate()
{
	return true;
}

void ACeilingLight::Multicast_LightOn_Implementation()
{
	LightComp->SetIntensity(5000.0f);
	Light->SetVisibility(true);
}

void ACeilingLight::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACeilingLight, Light);
	DOREPLIFETIME(ACeilingLight, LightComp);
}