// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Ball.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	NetUpdateFrequency = 100.f;
	SetReplicateMovement(true);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(CollisionComp);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMesh->SetupAttachment(CollisionComp);

	CollisionComp->InitSphereRadius(50.0f);

	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	MoveComp->InitialSpeed = 500.f;
	MoveComp->MaxSpeed = 1000.f;
	MoveComp->bShouldBounce = false;

}

void ABall::StartMovement()
{
	const FVector Direction = { FMath::Cos(FMath::RandRange(0.0f, 2 * PI)), FMath::Sin(FMath::RandRange(0.0f, 2 * PI)), 0.f };

	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + Direction * BaseSpeedMultiplayer, FColor::Emerald);

	MoveComp->SetVelocityInLocalSpace(Direction.GetSafeNormal() * BaseSpeedMultiplayer);
}

void ABall::ResetBall()
{
	SetActorLocation(InitialLocation);
	MoveComp->SetVelocityInLocalSpace(FVector::ZeroVector);
	FTimerHandle TH;
	GetWorldTimerManager().SetTimer(TH, FTimerDelegate::CreateWeakLambda(this, [&]() {
		StartMovement();
		}), 3.f, false);
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();

	//CollisionComp->OnComponentHit.AddDynamic(this, &ABall::OnHit);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABall::OnBeginOverlap);

	InitialLocation = GetActorLocation();

	if (!HasAuthority())
	{
		MoveComp->Deactivate();
	}
}

void ABall::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && HasAuthority())
	{
		const FVector NormalVelocity = MoveComp->Velocity.GetSafeNormal();
		const float SpeedMult = MoveComp->Velocity.Length() * AccelerationMultiplayer;
		const FVector NewDirection = NormalVelocity - 2 * FVector::DotProduct(SweepResult.Normal, NormalVelocity) * SweepResult.Normal;

		MoveComp->SetVelocityInLocalSpace(NewDirection * SpeedMult);
	}
}
