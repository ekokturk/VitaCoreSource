// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "WidgetSwitcher.h"
#include "ProgressBar.h"
#include "TextBlock.h"
#include "CanvasPanel.h"
#include "VCHUD_Game.h"

#include "VCHUD_CoreGame.generated.h"

class AVCCoreGameState;
class AVCCharacter;
class AVCPlayerController;
class AVCTeam;
class UVCWidget_Indicator;
class UVCWidget_CoreTitan;
class UImage;
class UVCWidget_GameStats;
class UVerticalBox;

UCLASS()
class VITACORE_API UVCHUD_CoreGame : public UVCHUD_Game
{
	GENERATED_BODY()

public:
	virtual void AddWidgetToCenterFeed(UUserWidget* Widget) override;
	virtual void AddWidgetToSideFeed(UUserWidget* Widget) override;
	virtual void TogglePlayerStats(bool IsActive) override;
	
protected:

	UFUNCTION() void HandleTeamDataChanged() const;

	UFUNCTION() void HandleFireTitan();

	UFUNCTION() void HandleIceTitan();

	UFUNCTION() void HandleVitaCoreTimer();

	UFUNCTION() void HandleStartTimer();

	UFUNCTION() void HandleCoreGameStageChanged();

protected:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UVerticalBox * CenterFeed_VerticalBox;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UVerticalBox * RightFeed_VerticalBox;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UCanvasPanel * GeneralState_Canvas;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UVCWidget_GameStats * PlayerStats_Widget;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UWidgetSwitcher * GameState_Switcher;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UProgressBar * FirstTeamCoreScore_Bar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UProgressBar * SecondTeamCoreScore_Bar;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UCanvasPanel * Gameplay_Canvas;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UImage * VitaCore_Image;

// SCORES

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UTextBlock * FirstTeamScore_TextBlock;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UTextBlock * SecondTeamScore_TextBlock;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UTextBlock * VitaCoreTimer_TextBlock;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UTextBlock * StartTimer_TextBlock;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UTextBlock * Objective_TextBlock;

// BOSS
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UVCWidget_Indicator * IceTitan_IndicatorWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UVCWidget_CoreTitan * IceTitan_Widget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UVCWidget_Indicator * FireTitan_IndicatorWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UVCWidget_CoreTitan * FireTitan_Widget;

	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnWin"))
		void BP_OnWin();
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnLose"))
		void BP_OnLose();

	UPROPERTY() bool bShouldTick;

	FTimerHandle TimerHandle_ObjectiveText;
	
	UPROPERTY(Transient) AVCCoreGameState * VCCoreGameState;
	UPROPERTY(Transient) AVCCharacter * MyCharacter;

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
