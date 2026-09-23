// Raul Hidalgo Nicolas - Copyright (C) 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RVGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TESTSUBJECT_API URVGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	bool bIsInMenuMode = false;
	
};
