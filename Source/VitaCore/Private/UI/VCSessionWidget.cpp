// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCSessionWidget.h"
#include "UMG/Public/Components/VerticalBox.h"

bool UVCSessionWidget::Initialize()
{
	bool bResult = Super::Initialize();

	// Check for base class
	if (!bResult)
	{
		return false;
	}

	
	return true;
}


void UVCSessionWidget::NativeConstruct()
{
	Super::NativeConstruct();


}



//
//void UVCSessionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
//{
//	Super::NativeTick(MyGeometry, InDeltaTime);
//
//}