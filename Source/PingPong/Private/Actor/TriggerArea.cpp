// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/TriggerArea.h"
#include "PlayerController/BasePlayerController.h"
#include "Components/BoxComponent.h"
#include "Actor/Ball.h"

// Sets default values
ATriggerArea::ATriggerArea()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(RootComponent);
	BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

}

// Called when the game starts or when spawned
void ATriggerArea::BeginPlay()
{
	Super::BeginPlay();

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ATriggerArea::OnAreaBeginOverlap);

}

void ATriggerArea::OnAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABall* Ball = Cast<ABall>(OtherActor))
	{
		if (HasAuthority())
		{
			if (IsValid(OwnerController))
			{
				Ball->OnGoalDelegate.Broadcast(OwnerController);
			}
			Ball->ResetBall();
		}
	}
}

