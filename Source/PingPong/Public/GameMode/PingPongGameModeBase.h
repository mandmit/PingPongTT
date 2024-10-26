// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PingPongGameModeBase.generated.h"

class APlayerStart;
class ABasePlayerController;
class ABall;
class ATriggerArea;

/**
 * 
 */
UCLASS(Abstract)
class PINGPONG_API APingPongGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	APingPongGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal) override;
	
	void UpdateAllPlayersScore();

	virtual void BeginMatch();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	void ResetBall();

	FText FormScore() const;

	void StartApprovePhase();

protected:

	UFUNCTION()
	void UpdateScore(ABasePlayerController* Loser);

	UFUNCTION()
	void Approve(ABasePlayerController* ApprovedController);

private:

	int8 MaxPlayers = 2;

	int8 bApprovePhase = 0;

	TArray<TObjectPtr<APlayerStart>> UnoccupiedStartPoints;

	TArray<TObjectPtr<ABasePlayerController>> ApprovedPlayers;

	TArray<int32> Scores;

	UPROPERTY(EditDefaultsOnly, Category = "Ball|Settings")
	TSubclassOf<ABall> BallClass;

	UPROPERTY(VisibleAnywhere, Category = "Ball|Settings")
	TObjectPtr<ABall> Ball;

	UPROPERTY(EditDefaultsOnly, Category = "Ball|Settings")
	FTransform SpawnBallTransform;

	UPROPERTY(EditDefaultsOnly, Category = "Gate|Settings")
	TSubclassOf<ATriggerArea> GateClass;

	UPROPERTY(EditDefaultsOnly, Category = "Gate|Settings")
	float GateOffset = 100.f;
};
