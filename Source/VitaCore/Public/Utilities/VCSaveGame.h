// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "VCSettingsTypes.h"
#include "VCSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class VITACORE_API UVCSaveGame : public USaveGame
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite)
		FVitaCoreSettings VitaCoreSettings;
};
