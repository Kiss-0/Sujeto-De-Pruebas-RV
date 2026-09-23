// Raul Hidalgo Nicolas - Copyright (C) 2026. All Rights Reserved.


#include "PauseMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MainGameMode.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind the button click events to their respective callback functions
	if (Btn_Resume)
	{
		Btn_Resume->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeClicked);
	}
	if (Btn_QuitToMenu)
	{
		Btn_QuitToMenu->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnQuitToMenuClicked);
	}
}

void UPauseMenuWidget::OnResumeClicked()
{
	if (AMainGameMode* GM = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->TogglePause(); 
	}
}

void UPauseMenuWidget::OnQuitToMenuClicked()
{
	UGameplayStatics::OpenLevel(this, GameLevelName);
}

