// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h" 
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"


#include "CPP_Character.generated.h"

UCLASS()
class MYPROJECT4_API ACPP_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_Character();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* FollowCamera;

	void MoveForward(float Axis);
	void MoveRight(float Axis);
	void Dodge();
	void StartSprint();
	void StopSprint();
	void WhileSprint();
	void Heal();
	void ActionRef();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bDead = false;

	UPROPERTY(BlueprintReadWrite)
		bool bCanMove = true;

	bool bIsRunning = false;
	float StartSpeed = 0.0f;
	

	UPROPERTY(EditAnywhere, Category = "UI HUD")
		TSubclassOf<UUserWidget> Player_Widget;

	UUserWidget* Player_Health_Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Endurance = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Health = 100.0f;

	UFUNCTION(BlueprintCallable)
		void RestoreEndurance();

	UFUNCTION(BlueprintCallable)
		void UpdateHealth(float Damage);

	UPROPERTY(EditAnywhere, Category = "Hit Animation")
		UAnimMontage* HitAnim;

	UPROPERTY(EditAnywhere, Category = "Death Animation")
		UAnimMontage* DeathAnim;

	UPROPERTY(EditAnywhere, Category = "Sitting Animation")
		UAnimMontage* SitAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* RefillActor;

	void Death();

	UFUNCTION(BlueprintCallable)
		void SwordTrace(FVector Start, FVector End);

	UPROPERTY(EditAnywhere, Category = "HitFX")
		UParticleSystem* BloddFX;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int CountOfHeal = 3;

	UFUNCTION(BlueprintCallable)
		bool ComboSystem();

	UFUNCTION(BlueprintCallable)
		void ComboSystemNotify();

	UPROPERTY(EditAnywhere)
		UAnimMontage* ComboAnim;

	UPROPERTY(EditAnywhere, Category = "Sound Attack")
		USoundBase* AttackSound;

	bool bIsSoundPlayed = false;

	int AttackIndex = 0;

	TSubclassOf<UDamageType> DamageType;

	FTimerHandle TimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};