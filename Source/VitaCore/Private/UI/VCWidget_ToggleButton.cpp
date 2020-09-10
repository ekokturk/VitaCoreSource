// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#include "VCWidget_ToggleButton.h"


#include "CheckBox.h"
#include "Image.h"
#include "Overlay.h"


bool UVCWidget_ToggleButton::Initialize()
{
	bool bResult = Super::Initialize();

	if (!bResult)
	{
		return false;
	}

	
	return true;
}


void UVCWidget_ToggleButton::NativeConstruct()
{
	Super::NativeConstruct();
	ToggleButton_CheckBox->OnCheckStateChanged.AddDynamic(this, &UVCWidget_ToggleButton::OnCheckStateChanged);
}


void UVCWidget_ToggleButton::SetActive(bool IsActive)
{
	bIsActive = IsActive;
	ToggleButton_CheckBox->SetIsChecked(IsActive);
}

void UVCWidget_ToggleButton::SetImage(UTexture2D* NewImage) const
{
	if(NewImage == nullptr){
		return;
	}
	
	ToggleButton_CheckBox->WidgetStyle.CheckedImage.SetResourceObject(NewImage);
	ToggleButton_CheckBox->WidgetStyle.CheckedHoveredImage.SetResourceObject(NewImage);
	ToggleButton_CheckBox->WidgetStyle.CheckedPressedImage.SetResourceObject(NewImage);
	ToggleButton_CheckBox->WidgetStyle.UncheckedImage.SetResourceObject(NewImage);
	ToggleButton_CheckBox->WidgetStyle.UncheckedHoveredImage.SetResourceObject(NewImage);
	ToggleButton_CheckBox->WidgetStyle.UncheckedPressedImage.SetResourceObject(NewImage);
}

void UVCWidget_ToggleButton::SetBackgroundColor(FLinearColor BackgroundColor) const
{
	ToggleButton_BackgroundImage->ColorAndOpacity = BackgroundColor;
}

void UVCWidget_ToggleButton::OnCheckStateChanged(bool IsChecked)
{
	OnToggleActiveStateChanged.Broadcast(IsChecked);
}
