// Raul Hidalgo Nicolas - Copyright (C) 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

/**
 * 
 MainGameMode is the GameMode class for tha main gameplay level. 
 It will manage the game states (Pause, game over and playing) and the transitions between them.
 */

 // Forward declaration
class AWorldUIPanel;

UENUM(BlueprintType)
enum class ERVGameState : uint8  // Enum to represent the current state of the main game.
{
	Playing,
	Paused,
	GameOver
};

UCLASS()
class TESTSUBJECT_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainGameMode();

	UFUNCTION(BlueprintCallable, Category = "GameState")
	void TogglePause();

	UFUNCTION(BlueprintCallable, Category = "GameState")
	void TriggerGameOver();

	UFUNCTION(BlueprintCallable, Category = "GameState")
	void RestartGame();

	UFUNCTION(BlueprintPure)
	ERVGameState getGameState() const
	{
		return currentGameState;
	}

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void setGameState(ERVGameState NewState);

	UPROPERTY()
	AWorldUIPanel* PausePanel;

	UPROPERTY()
	AWorldUIPanel* GameOverPanel;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<AWorldUIPanel> PausePanelClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<AWorldUIPanel> GameOverPanelClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ERVGameState currentGameState = ERVGameState::Playing;
};


