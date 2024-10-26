// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPlatform.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UFloatingPawnMovement;
class UBoxComponent;

UCLASS(Abstract)
class PINGPONG_API APlayerPlatform : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPlatform();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(float Value);

	UFUNCTION(Server, Reliable)
	void ServerMove(float Value);

	void TiltPlatform(float Value);

	UFUNCTION(Server, Reliable)
	void ServerUpdateTransform(const FTransform& NewTransform);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Input)
	float SpeedMult = 0.5f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Input)
	float TiltStep = 10.f;

	UPROPERTY(VisibleAnywhere)
	FVector StartRightVector;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxCollider;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMovementComponent;
};
