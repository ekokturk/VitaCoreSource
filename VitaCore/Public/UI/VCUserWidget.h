// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VCUserWidget.generated.h"

class AVCPlayerController;
class AVCPlayerState;
class AVCGameState;
class UVCUserWidget;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnWidgetActiveStateChanged, const UVCUserWidget*)
DECLARE_MULTICAST_DELEGATE(FOnRemovedFromViewport)

/**
 * Base class for VitaCore widgets
 */
UCLASS()
class VITACORE_API UVCUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// Set active state of the widget
	UFUNCTION() void SetWidgetActive(bool IsActive);
	
	// Return active state of the widget
	UFUNCTION(BlueprintCallable) bool IsWidgetActive() const { return bIsWidgetActive; };

	// Delegate to notify widget is activated
	FOnWidgetActiveStateChanged OnWidgetActivated;

	// Delegate to notify widget is deactivated
	FOnWidgetActiveStateChanged OnWidgetDeactivated;

	// Delegate to notify widget is about to be removed from the viewport
	FOnRemovedFromViewport OnRemovedFromViewport;
	
protected:

	// Activation callback
	UFUNCTION() virtual void HandleWidgetActivated();

	// Activation callback for blueprints
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnWidgetActivated"))
		void BP_OnWidgetActivated();

	// Deactivation callback
	UFUNCTION() virtual void HandleWidgetDeactivated();

	// Deactivation callback for blueprints
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnWidgetDeactivated"))
		void BP_OnWidgetDeactivated();

protected:

	// If true, widget will be set to active when it is constructed
	UPROPERTY(EditAnywhere, Category = "Config") bool bShouldActivateWidgetOnConstruct;

	// If true, widget will be set to active when it is constructed
	UPROPERTY(EditAnywhere, Category = "Config") bool bShouldRemoveFromViewportOnDeactivation;
	
	// If true, active state will be changed after animation is played
	UPROPERTY(EditAnywhere, Category = "Config|Animation") bool bShouldChangeActiveStateWithAnimation;

	// Animation to be played on activation
	UPROPERTY(EditAnywhere, Category = "Config|Animation") UWidgetAnimation* Animation_WidgetActivation;
	
	// Animation to be played on deactivation
	UPROPERTY(EditAnywhere, Category = "Config|Animation") UWidgetAnimation* Animation_WidgetDeactivation;

	

	
	// Active state of the widget
	UPROPERTY() bool bIsWidgetActive;

	// Cached references to VitaCore objects
	UPROPERTY(Transient) AVCPlayerController* VCPlayerController;
	UPROPERTY(Transient) AVCGameState* VCGameState;
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void RemoveFromParent() override;
};
