// Raul Hidalgo Nicolas - Copyright (C) 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MenuDisplay.generated.h"

// Forward declaration
class UWidgetComponent;

UCLASS()
class TESTSUBJECT_API AMenuDisplay : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMenuDisplay();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* MenuWidgetComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
