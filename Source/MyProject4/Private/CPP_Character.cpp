// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Character.h"
#include "CPP_Enemy.h"
#include "CPP_PickUp_Health.h"

// Sets default values
ACPP_Character::ACPP_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Camera and CameraBoom initialize
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 400.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateAbstractDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateAbstractDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void ACPP_Character::RestoreEndurance()
{
	if (Endurance < 100.0f && !bIsRunning)
	{
		ACPP_Character::Endurance++;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACPP_Character::RestoreEndurance, 0.1f);
	}
	else
	{
		return;
	}
}

void ACPP_Character::UpdateHealth(float Damage)
{
	ACPP_Character::Health -= Damage;
	PlayAnimMontage(HitAnim);
	if (ACPP_Character::Health <= 0)
	{
		PlayAnimMontage(DeathAnim);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACPP_Character::Death, 2.5f);
	}
}

void ACPP_Character::Death()
{
	UKismetSystemLibrary::PrintString(this, "Game Over");
	bDead = true;
	this->Destroy();
}

void ACPP_Character::SwordTrace(FVector Start, FVector End)
{
	TArray<AActor*> Ignore;
	FHitResult OutHit;

	UKismetSystemLibrary::LineTraceSingle(this, Start, End, ETraceTypeQuery::TraceTypeQuery10, false, Ignore, EDrawDebugTrace::None, OutHit, true);
	
	if (OutHit.GetActor())
	{
		if (Cast<ACPP_Enemy>(OutHit.GetActor()))
		{
			UGameplayStatics::ApplyDamage(OutHit.GetActor(), 5.0f, this->GetController(), this, DamageType);
			OutHit.GetActor()->GetActorForwardVector() = OutHit.GetActor()->GetActorForwardVector() * 1050.0f;
			UGameplayStatics::SpawnEmitterAtLocation(this, BloddFX, OutHit.Location, UKismetMathLibrary::MakeRotFromXY(OutHit.Normal, OutHit.Normal), true);
		}
	}
}

// Called when the game starts or when spawned
void ACPP_Character::BeginPlay()
{
	Super::BeginPlay();

	if (Player_Widget != nullptr)
	{
		Player_Health_Widget = CreateWidget(GetWorld(), Player_Widget);
		Player_Health_Widget->AddToViewport();
	}
}


bool ACPP_Character::ComboSystem()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance->Montage_IsPlaying(ComboAnim))
	{
		ACPP_Character::AttackIndex = 1;
		return true;
	}
	else
	{
		return false;
	}
}

void ACPP_Character::ComboSystemNotify()
{
	ACPP_Character::AttackIndex--;
	ACPP_Character::Endurance -= 5;
	if (ACPP_Character::AttackIndex < 0)
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.35f, ComboAnim);
		ACPP_Character::AttackIndex = 0;
		ACPP_Character::RestoreEndurance();
	}
}

// Called every frame
void ACPP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACPP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Input for looking
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look", this, &APawn::AddControllerPitchInput);

	//Input for jumping, sprinting and dodge
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Dodge/Sprint", IE_DoubleClick, this, &ACPP_Character::Dodge);
	PlayerInputComponent->BindAction("Dodge/Sprint", IE_Pressed, this, &ACPP_Character::StartSprint);
	PlayerInputComponent->BindAction("Dodge/Sprint", IE_Repeat, this, &ACPP_Character::WhileSprint);
	PlayerInputComponent->BindAction("Dodge/Sprint", IE_Released, this, &ACPP_Character::StopSprint);
	PlayerInputComponent->BindAction("Heal", IE_Pressed, this, &ACPP_Character::Heal);
	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &ACPP_Character::ActionRef);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPP_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPP_Character::MoveRight);

}

void ACPP_Character::MoveForward(float Axis)
{
	if (!bDead && !bIsRunning)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);
	}
	else if (!bDead && bIsRunning)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);

		ACPP_Character::WhileSprint();
	}
}

void ACPP_Character::MoveRight(float Axis)
{
	if (!bDead && !bIsRunning)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);
	}
	else if (!bDead && bIsRunning)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);

		ACPP_Character::WhileSprint();
	}
}

void ACPP_Character::Dodge()
{
	if (!bDead && ACPP_Character::Endurance >= 10.0f)
	{
		FVector ForwardVector = ACPP_Character::GetActorForwardVector();
		FVector DodgeRange = ForwardVector * 5000.0f;

		ACPP_Character::Endurance -= 10.0f;

		LaunchCharacter(DodgeRange, true, false);

		RestoreEndurance();
	}
}

void ACPP_Character::StartSprint()
{
	if (!bDead && ACPP_Character::Endurance > 0.0f)
	{
		ACPP_Character::bIsRunning = true;
		StartSpeed = GetCharacterMovement()->MaxWalkSpeed;
		GetCharacterMovement()->MaxWalkSpeed *= 1.5f;
	}
}

void ACPP_Character::StopSprint()
{
	if (!bDead && bIsRunning)
	{
		ACPP_Character::bIsRunning = false;
		GetCharacterMovement()->MaxWalkSpeed = StartSpeed;
		ACPP_Character::RestoreEndurance();
	}
}

void ACPP_Character::WhileSprint()
{
	if (bIsRunning && ACPP_Character::Endurance > 0.0f)
	{
		ACPP_Character::Endurance -= 0.025f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = StartSpeed;
	}
}

void ACPP_Character::Heal()
{
	if (ACPP_Character::Health < 80.0f)
	{
		ACPP_Character::Health += 20.0f;
		ACPP_Character::CountOfHeal--;
	}
	else if (ACPP_Character::Health < 100.0f)
	{
		ACPP_Character::Health = 100.0f;
		ACPP_Character::CountOfHeal--;
	}
}

void ACPP_Character::ActionRef()
{
	if (Cast<ACPP_PickUp_Health>(RefillActor))
	{
		if (CountOfHeal < 5)
		{
			CountOfHeal = 5;
		}
		PlayAnimMontage(SitAnim);
	}
}
