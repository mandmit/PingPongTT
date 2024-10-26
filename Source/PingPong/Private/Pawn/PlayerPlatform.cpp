// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/PlayerPlatform.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "InputActionValue.h"

// Sets default values
APlayerPlatform::APlayerPlatform()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	NetUpdateFrequency = 100.f;
	SetReplicatingMovement(true);


	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxCollider);
	BoxCollider->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	BoxCollider->SetShouldUpdatePhysicsVolume(true);
	BoxCollider->SetCanEverAffectNavigation(false);
	BoxCollider->bDynamicObstacle = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMesh->SetupAttachment(BoxCollider);
	StaticMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(StaticMesh);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArm);

	FloatingPawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovementComponent"));
	FloatingPawnMovementComponent->UpdatedComponent = BoxCollider;
	FloatingPawnMovementComponent->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void APlayerPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	StartRightVector = GetActorRightVector();
}

void APlayerPlatform::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

UPawnMovementComponent* APlayerPlatform::GetMovementComponent() const
{
	return FloatingPawnMovementComponent;
}

// Called to bind functionality to input
void APlayerPlatform::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerPlatform::Move(float Value)
{
	if (GetController())
	{
		AddMovementInput(StartRightVector, Value * SpeedMult);
		//ServerMove(Value * SpeedMult);

		if (Controller->GetLocalRole() == ROLE_AutonomousProxy)
		{
			ServerUpdateTransform(GetActorTransform());
		}
	}
}

void APlayerPlatform::ServerMove_Implementation(float Value)
{
	if (GetController())
	{
		AddMovementInput(StartRightVector, Value);
	}
}

void APlayerPlatform::TiltPlatform(float Value)
{
	if (GetController())
	{
		SetActorRotation(GetActorRotation() + FRotator(0.f, Value * TiltStep, 0.f));
		if (Controller->GetLocalRole() == ROLE_AutonomousProxy)
		{
			ServerUpdateTransform(GetActorTransform());
		}
	}
}

void APlayerPlatform::ServerUpdateTransform_Implementation(const FTransform& NewTransform)
{
	SetActorTransform(NewTransform);
}
