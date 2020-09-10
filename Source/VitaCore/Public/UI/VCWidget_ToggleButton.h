// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VCWidget_ToggleButton.generated.h"

class UOverlay;
class UCheckBox;
class UImage;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnToggleActiveStateChanged, bool)

UCLASS()
class VITACORE_API UVCWidget_ToggleButton : public UUserWidget
{
	GENERATED_BODY()

public:
	
	// Switch toggle active state
	void SetActive(bool IsActive);

	// Check if toggle is active
	bool IsActive() const { return bIsActive; }

	// Set Image for toggle button
	void SetImage(UTexture2D* NewImage)  const;

	// Set Background Image color
	void SetBackgroundColor(FLinearColor BackgroundColor)  const;
	
	FOnToggleActiveStateChanged OnToggleActiveStateChanged;
	
protected:

	// Callback to notify checkbox state
	UFUNCTION() void OnCheckStateChanged(bool IsChecked);
	
protected:
	
	UPROPERTY() bool bIsActive;

	UPROPERTY(meta = (BindWidget)) UOverlay* ToggleButton_Overlay;
	UPROPERTY(meta = (BindWidget)) UCheckBox* ToggleButton_CheckBox;
	UPROPERTY(meta = (BindWidget)) UImage* ToggleButton_BackgroundImage;
	
protected:
	//virtual void onmouse
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;


};
