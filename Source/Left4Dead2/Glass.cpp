// Fill out your copyright notice in the Description page of Project Settings.


#include "Glass.h"
//#include "Juno_CPPCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AGlass::AGlass()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Glass = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Glass"));
	Glass->SetupAttachment(RootComponent);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(Glass);
}

// Called when the game starts or when spawned
void AGlass::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AGlass::OnBeginOverlap);
}

// Called every frame
void AGlass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGlass::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//AJuno_CPPCharacter* Character = Cast<AJuno_CPPCharacter>(OtherActor);
	//if (Character)
	//{
		//Destroy();
	//}
}

