// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartMatchApproveSignature, ABasePlayerController*, ApprovedController);

/**
 * 
 */
UCLASS(Abstract)
class PINGPONG_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnStartMatchApproveSignature OnStartMatchApproveDelegate;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> TiltAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> StartMatchAction;
	
protected:

	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	
	void Tilt(const FInputActionValue& Value);

	void Approve(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void ServerHandleApprove(ABasePlayerController* ApprovedController);

public:
	UFUNCTION(Client, Reliable)
	void ClientUpdateScore(const FText& NewScore);
};
