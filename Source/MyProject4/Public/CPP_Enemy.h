// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPP_Enemy.generated.h"

UCLASS()
class MYPROJECT4_API ACPP_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_Enemy();

	UPROPERTY(VisibleAnywhere)
		float Health = 100.0f;

	UFUNCTION(BlueprintCallable)
		void UpdateHealth(float Damage);

	UPROPERTY(EditAnywhere, Category = "Hit Animation")
		UAnimMontage* HitAnim;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
