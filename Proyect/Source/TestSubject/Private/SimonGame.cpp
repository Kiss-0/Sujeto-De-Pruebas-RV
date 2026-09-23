// Raul Hidalgo Nicolas - Copyright (C) 2026. All Rights Reserved.


#include "SimonGame.h"
#include "SimonButton.h"
#include "MainGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASimonGame::ASimonGame()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setted via Blueprint
    ErrorSound = nullptr;

}

// Called when the game starts or when spawned
void ASimonGame::BeginPlay()
{
	Super::BeginPlay();

    for (ASimonButton* Button : Buttons)
    {
        if (Button)
        {
            Button->OnButtonPressed.AddDynamic(this, &ASimonGame::HandleButtonPressed);
        }
    }

    StartGame();
	
}

void ASimonGame::StartGame()
{
    Sequence.Empty();
    PlayerProgress = 0;
    CurrentRound = 0;
    StartNextRound();
}

void ASimonGame::SetState(ESimonState NewState)
{
    CurrentState = NewState;

    if (CurrentState != ESimonState::WaitingForPlayer)
    {
        // Reset all buttons to their default state
        for (ASimonButton* Button : Buttons)
        {
            if (Button)
            {
                Button->IsAllowedToInteract = false;
            }
        }
	}
    else
    {
        // Reset all buttons to their default state
        for (ASimonButton* Button : Buttons)
        {
            if (Button)
            {
                Button->IsAllowedToInteract = true;
            }
        }
    }
    
}

void ASimonGame::StartNextRound()
{
    CurrentRound++;

	// Add a new random button to the sequence
    const int32 RandomIndex = FMath::RandRange(0, Buttons.Num() - 1);
    Sequence.Add(RandomIndex);

    PlayerProgress = 0;

	// Show the sequence to the player
    ShowSequence();
}

void ASimonGame::ShowSequence()
{
    SetState(ESimonState::ShowingSequence);
    ShowIndex = 0;

    GetWorldTimerManager().SetTimer(
        SequenceTimerHandle, this, &ASimonGame::ShowNextInSequence, TimeBetweenFlashes, false);
}

void ASimonGame::ShowNextInSequence()
{
	// Finished showing the sequence ?
    if (ShowIndex >= Sequence.Num())
    {
		// Player's turn to repeat the sequence
        SetState(ESimonState::WaitingForPlayer);
        return;
    }

    const int32 ButtonIdx = Sequence[ShowIndex];
    if (Buttons.IsValidIndex(ButtonIdx) && Buttons[ButtonIdx])
    {
        Buttons[ButtonIdx]->Highlight();
    }

    ShowIndex++;

	// Next button in the sequence after a delay
    GetWorldTimerManager().SetTimer(
        SequenceTimerHandle, this, &ASimonGame::ShowNextInSequence, TimeBetweenFlashes, false);
}

void ASimonGame::HandleButtonPressed(int32 ButtonIndex)
{
    IsFirstTime = false;
	// If not the player's turn, ignore the input
    if (CurrentState != ESimonState::WaitingForPlayer)
    {
        return;
    }

    if (Sequence.IsValidIndex(PlayerProgress) && ButtonIndex == Sequence[PlayerProgress])
    {
        PlayerProgress++;

        if (PlayerProgress >= Sequence.Num())
        {
			// Round completed successfully
            OnPlayerSuccess();
        }
    }
    else
    {
		// Fail, reset the game
        OnPlayerFail();
    }
    
}

void ASimonGame::OnPlayerSuccess()
{
 
    if (CurrentRound >= RoundsToWin)
    {
        SetState(ESimonState::Win);

		// Unlock the ending
        if (AMainGameMode* GM = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(this)))
        {
            GM->TriggerGameOver();
        }
       
    }
    else
    {
        SetState(ESimonState::Idle);
		// Afther a short delay, start the next round
        FTimerHandle NextRoundHandle;
        GetWorldTimerManager().SetTimer(
            NextRoundHandle, this, &ASimonGame::StartNextRound, 1.0f, false);
    }

}

void ASimonGame::OnPlayerFail()
{
    SetState(ESimonState::Idle);

	// Reset the game state
    Sequence.Empty();
    PlayerProgress = 0;
    CurrentRound = 0;

 
    for (ASimonButton* Button : Buttons)
    {
        if (Button)
        {
            Button->Highlight(false);
        }
	}

    if (ErrorSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ErrorSound, GetActorLocation());
    }

    FTimerHandle RestartHandle;
    GetWorldTimerManager().SetTimer(
        RestartHandle, this, &ASimonGame::StartNextRound, 1.5f, false);
}

// Called every frame
void ASimonGame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    TimeAccumulator += DeltaTime;
    if (TimeAccumulator >= ShowInterval && IsFirstTime)
    {
        TimeAccumulator = 0.f;
        ShowSequence();
    }

}

