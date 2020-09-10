// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VCOptionsMenu.generated.h"

class UButton;
class USlider;

UCLASS()
class VITACORE_API UVCOptionsMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	
};
