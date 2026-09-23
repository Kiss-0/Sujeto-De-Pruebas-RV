// Raul Hidalgo Nicolas - Copyright (C) 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimonGame.generated.h"

class ASimonButton;
class USoundBase;

UENUM(BlueprintType)
enum class ESimonState : uint8
{
	Idle,
	ShowingSequence,
	WaitingForPlayer,
	GameOver,
	Win
};

UCLASS()
class TESTSUBJECT_API ASimonGame : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimonGame();

	UFUNCTION(BlueprintCallable, Category = "Simon")
	void StartGame();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float TimeAccumulator = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Simon")
	float ShowInterval = 5.f;

	bool IsFirstTime = true;

	// Reference to the buttons in the game
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Simon")
	TArray<ASimonButton*> Buttons;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Simon")
	ESimonState CurrentState = ESimonState::Idle;

	// The sequence of button indices that the player must follow
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Simon")
	TArray<int32> Sequence;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Simon")
	int32 PlayerProgress = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Simon")
	int32 CurrentRound = 0;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Simon")
	int32 RoundsToWin = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Simon")
	float TimeBetweenFlashes = 0.7f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Simon")
	USoundBase* ErrorSound;

	// Internal functions 
	void SetState(ESimonState NewState);
	void StartNextRound();
	void ShowSequence();
	void ShowNextInSequence();
	void OnPlayerSuccess();   
	void OnPlayerFail();      

	UFUNCTION()
	void HandleButtonPressed(int32 ButtonIndex);

	int32 ShowIndex = 0;

	FTimerHandle SequenceTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
