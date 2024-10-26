// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PingPongGameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "Actor/Ball.h"
#include "PlayerController/BasePlayerController.h"
#include "Actor/TriggerArea.h"
#include "EngineUtils.h"

APingPongGameModeBase::APingPongGameModeBase()
{
}

void APingPongGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	for (TActorIterator<APlayerStart> It(GetWorld(), APlayerStart::StaticClass()); It; ++It)
	{
		UnoccupiedStartPoints.Add(*It);
	}
}

void APingPongGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	if (UnoccupiedStartPoints.Num() == 0)
	{
		ErrorMessage = TEXT("Server is full.");
	}

	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

FString APingPongGameModeBase::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	if (UnoccupiedStartPoints.Num() == 0)
	{
		return FString(TEXT("There is no available spot to spawn player."));
	}

	NewPlayerController->StartSpot = UnoccupiedStartPoints.Pop();
	FTransform SpawnTransform;
	SpawnTransform.SetTranslation(NewPlayerController->StartSpot->GetActorLocation());
	const FVector BackwardVector = (-NewPlayerController->StartSpot->GetActorForwardVector() * GateOffset);
	
	SpawnTransform.AddToTranslation(BackwardVector);
	ATriggerArea* Area = GetWorld()->SpawnActor<ATriggerArea>(GateClass, SpawnTransform);
	Area->OwnerController = Cast<ABasePlayerController>(NewPlayerController);

	return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
}

void APingPongGameModeBase::UpdateAllPlayersScore()
{
	FText Score = FormScore();
	for (auto It : ApprovedPlayers)
	{
		It->ClientUpdateScore(Score);
	}
}

void APingPongGameModeBase::BeginMatch()
{
	if (!IsValid(Ball))
	{
		Ball = GetWorld()->SpawnActor<ABall>(BallClass, SpawnBallTransform);
		Ball->OnGoalDelegate.AddDynamic(this, &ThisClass::UpdateScore);
	}
	Ball->StartMovement();
	bApprovePhase = false;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Match has started."));

}

void APingPongGameModeBase::ResetBall()
{
	Ball->SetActorLocation(SpawnBallTransform.GetLocation());
	Ball->StartMovement();
}

void APingPongGameModeBase::StartApprovePhase()
{
	bApprovePhase = true;
}

void APingPongGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (UnoccupiedStartPoints.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Now each player should approve (hit [SPACE]) to begin match."));
		StartApprovePhase();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Welcome on server. Waiting for player to enter."));
	}

	if (ABasePlayerController* Controller = Cast<ABasePlayerController>(NewPlayer))
	{
		Controller->OnStartMatchApproveDelegate.AddDynamic(this, &ThisClass::Approve);
	}
}

FText APingPongGameModeBase::FormScore() const
{
	return FText::FromString(FString::Printf(TEXT("%d | %d"), Scores[0], Scores[1]));
}

void APingPongGameModeBase::UpdateScore(ABasePlayerController* Loser)
{

	if (ApprovedPlayers[0] == Loser)
	{
		++Scores[1];
	}
	else
	{
		++Scores[0];
	}
	UpdateAllPlayersScore();
	//ApprovedPlayers.FindByPredicate([&](ABasePlayerController* Other) {return Loser != Other; });
}

void APingPongGameModeBase::Approve(ABasePlayerController* ApprovedController)
{
	if (bApprovePhase)
	{
		if (ApprovedPlayers.Find(ApprovedController) == INDEX_NONE)
		{
			ApprovedPlayers.Add(ApprovedController);
			Scores.Add(0);

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Pawn: %s, approved match starting."), *ApprovedController->GetName()));
			if (ApprovedPlayers.Num() == MaxPlayers)
			{
				BeginMatch();
			}
		}
	}
}
