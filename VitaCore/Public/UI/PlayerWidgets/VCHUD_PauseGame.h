// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VCHUD_PauseGame.generated.h"

class UCanvasPanel;
class UVCOptionsMenu;

UCLASS()
class VITACORE_API UVCHUD_PauseGame : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UCanvasPanel * InGameMenuCanvas;
	
	UFUNCTION() void SetMenuActive(bool IsActive);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "SetMenuActive"))
		void BP_SetMenuActive(bool IsActive);

	UFUNCTION(BlueprintCallable)
		bool IsCurrentlyActive() const;

	UPROPERTY(BlueprintReadWrite)
		bool bIsTransitioning;
	
protected:
	UPROPERTY() bool bIsCurrentlyActive;




};
