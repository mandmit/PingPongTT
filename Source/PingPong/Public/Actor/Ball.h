// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USphereComponent;
class UProjectileMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoalSignature, ABasePlayerController*, Loser);

UCLASS(Abstract)
class PINGPONG_API ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABall();

	UFUNCTION()
	void StartMovement();

	UFUNCTION()
	void ResetBall();

public:

	UPROPERTY(BlueprintAssignable)
	FOnGoalSignature OnGoalDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> MoveComp;

	UPROPERTY(VisibleAnywhere)
	FVector InitialLocation;

	UPROPERTY(EditAnywhere)
	float BaseSpeedMultiplayer = 600.f;

	UPROPERTY(EditAnywhere)
	float AccelerationMultiplayer = 1.05f;
};
