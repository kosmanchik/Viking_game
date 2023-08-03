// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PickUp_Health.h"
#include "CPP_Character.h"

// Sets default values
ACPP_PickUp_Health::ACPP_PickUp_Health()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("My box collision"));
	MyCollision->InitBoxExtent(FVector(32.0f, 32.0f, 32.0f));

	RootComponent = MyCollision;

	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My mesh"));
	MyMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACPP_PickUp_Health::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_PickUp_Health::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}