// Raul Hidalgo Nicolas - Copyright (C) 2026. All Rights Reserved.


#include "MainGameMode.h"
#include "RVPawn.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "WorldUIPanel.h"

AMainGameMode::AMainGameMode()
{

}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (PausePanelClass)
	{
		PausePanel = Cast<AWorldUIPanel>(
			UGameplayStatics::GetActorOfClass(this, PausePanelClass));
	}
	if (GameOverPanelClass)
	{
		GameOverPanel = Cast<AWorldUIPanel>(
			UGameplayStatics::GetActorOfClass(this, GameOverPanelClass));
	}
	setGameState(ERVGameState::Playing);
}

void AMainGameMode::TogglePause()
{
	if (currentGameState == ERVGameState::Playing)
	{
		setGameState(ERVGameState::Paused);
	}
	else if (currentGameState == ERVGameState::Paused)
	{
		setGameState(ERVGameState::Playing);
	}
}

void AMainGameMode::TriggerGameOver()
{
	if (currentGameState != ERVGameState::GameOver)
	{
		setGameState(ERVGameState::GameOver);
	}
}

void AMainGameMode::RestartGame()
{
	// Restart the game
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
}


void AMainGameMode::setGameState(ERVGameState NewState)
{
	currentGameState = NewState;

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);

	switch (NewState)
	{
		case ERVGameState::Playing:
		{
			UGameplayStatics::SetGamePaused(this, false);

			if (PausePanel)
			{
				PausePanel->HidePanel();
			}
			if (GameOverPanel)
			{
				GameOverPanel->HidePanel();
			}

			break;
		}
		case ERVGameState::Paused:
		{
			if (PausePanel)
			{
				PausePanel->ShowPanel();
			}
			UGameplayStatics::SetGamePaused(this, true);
			break;
		}
		case ERVGameState::GameOver:
		{
			if (GameOverPanel)
			{
				GameOverPanel->ShowPanel();
			}
			UGameplayStatics::SetGamePaused(this, true);
			break;
		}
	}
}
