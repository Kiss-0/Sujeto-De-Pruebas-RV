// Raul Hidalgo Nicolas - Copyright (C) 2026. All Rights Reserved.


#include "GameOverWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MainGameMode.h"

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind the button click events to their respective callback functions
	if (Btn_Retry)
	{
		Btn_Retry->OnClicked.AddDynamic(this, &UGameOverWidget::OnRetryClicked);
	}
	if (Btn_QuitToMenu)
	{
		Btn_QuitToMenu->OnClicked.AddDynamic(this, &UGameOverWidget::OnBackToMenuClicked);
	}
}

void UGameOverWidget::OnRetryClicked()
{
	if (AMainGameMode* GM = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->RestartGame();
	}
}

void UGameOverWidget::OnBackToMenuClicked()
{
	UGameplayStatics::SetGamePaused(this, false);
	UGameplayStatics::OpenLevel(this, GameLevelName);
}

