// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Enemy.h"

// Sets default values
ACPP_Enemy::ACPP_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ACPP_Enemy::UpdateHealth(float Damage)
{
	PlayAnimMontage(HitAnim);
	ACPP_Enemy::Health -= Damage;
	FVector BackwardVector = ACPP_Enemy::GetActorForwardVector() * - 750.0f;
	//LaunchCharacter(BackwardVector, true, false);
	if (Health <= 0.0f)
	{
		bDead = true;
		PlayAnimMontage(DeathAnim);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACPP_Enemy::Death, 1.9f);
	}
}

FHitResult ACPP_Enemy::LineTraceBySword(FVector Start, FVector End)
{
	TArray<AActor*> Ignore;
	FHitResult OutHit;

	UKismetSystemLibrary::LineTraceSingle(this, Start, End, ETraceTypeQuery::TraceTypeQuery10, false, Ignore, EDrawDebugTrace::None, OutHit, true);

	return OutHit;
}

FHitResult ACPP_Enemy::SphereTraceByChase()
{
	FHitResult OutHit;

	FVector StartActorLoc = this->GetActorLocation();
	FVector EndActorLoc = (UKismetMathLibrary::GetForwardVector(this->GetActorRotation()) * 200.0f) + StartActorLoc;

	TArray<AActor*> Ignore;

	UKismetSystemLibrary::SphereTraceSingle(this, StartActorLoc, EndActorLoc, 45.0f, ETraceTypeQuery::TraceTypeQuery10, false, Ignore, EDrawDebugTrace::None, OutHit, true);

	return OutHit;
}

void ACPP_Enemy::Death()
{
	this->Destroy();
}

// Called when the game starts or when spawned
void ACPP_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPP_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}