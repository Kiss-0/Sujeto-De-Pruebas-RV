// Raul Hidalgo Nicolas - Copyright (C) 2026. All Rights Reserved.


#include "MenuDisplay.h"
#include "Components/WidgetComponent.h"

// Sets default values
AMenuDisplay::AMenuDisplay()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    MenuWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("MenuWidgetComp"));
    SetRootComponent(MenuWidgetComp);

    // World-space
    MenuWidgetComp->SetWidgetSpace(EWidgetSpace::World);

    // Set size
    MenuWidgetComp->SetDrawSize(FVector2D(1500.f, 1500.f));

	// Enable collision for the widget component
    MenuWidgetComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

// Called when the game starts or when spawned
void AMenuDisplay::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMenuDisplay::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

