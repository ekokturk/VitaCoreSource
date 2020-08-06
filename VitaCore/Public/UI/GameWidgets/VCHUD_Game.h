// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCUserWidget.h"
#include "VCHUD_Game.generated.h"

/**
 * 
 */
UCLASS()
class VITACORE_API UVCHUD_Game : public UVCUserWidget
{
	GENERATED_BODY()


protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	// OBJECTIVE
public:

	UFUNCTION(BlueprintCallable)
		virtual void ShowCurrentObjective();

	UFUNCTION(BlueprintCallable)
		virtual void AddWidgetToSideFeed(UUserWidget * Widget);

	UFUNCTION(BlueprintCallable)
		virtual void AddWidgetToCenterFeed(UUserWidget * Widget);
	
	UFUNCTION(BlueprintCallable)
		virtual void TogglePlayerStats(bool IsActive);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnShowCurrentObjective"))
		void BP_ShowCurrentObjective();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		float ObjectiveDisplayDuration;
	
};
