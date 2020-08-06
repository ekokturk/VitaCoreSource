// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCCharacterDetails.h"

#if WITH_EDITOR

#include "Editor/PropertyEditor/Public/DetailLayoutBuilder.h"
#include "Editor/PropertyEditor/Public/DetailCategoryBuilder.h"
#include "Editor/PropertyEditor/Public/DetailWidgetRow.h"
#include "Runtime/Slate/Public/Widgets/Text/STextBlock.h"
#include "Runtime/Slate/Public/Widgets/Input/SButton.h"

#include "VCCharacter.h"

#define LOCTEXT_NAMESPACE "VCCharacterDetails"

TSharedRef<IDetailCustomization> FVCCharacterDetails::MakeInstance()
{
	return MakeShareable(new FVCCharacterDetails);
}


void FVCCharacterDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	 IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Config", FText::GetEmpty(), ECategoryPriority::Important);
	 TArray< TWeakObjectPtr<UObject> > ObjectsCustomized;
	 DetailBuilder.GetObjectsBeingCustomized(ObjectsCustomized);
	 UObject* ObjectCustomized = (ObjectsCustomized.Num() > 0) ? ObjectsCustomized[0].Get() : nullptr;
	 AVCCharacter* Character = Cast<AVCCharacter>(ObjectCustomized);

	 //Category.AddCustomRow(LOCTEXT("RowSearchName","Camera"))
	 //.NameContent()
	 //[
	 //	SNew(STextBlock)
	 //	.Text(LOCTEXT("DetailName", "Camera View"))
	 //	.Font(IDetailLayoutBuilder::GetDetailFont())
	 //]
	 //.ValueContent().HAlign(EHorizontalAlignment::HAlign_Fill)
		// [
		//	 SNew(SButton)
		//	 .Text(LOCTEXT("CameraViewButtonText", "Set View"))
		//	 .HAlign(HAlign_Center)
		//	 .ToolTipText(LOCTEXT("CameraViewButtonTooltip", "Setup camera based on the currently selected view"))
		//	 .OnClicked_Lambda([Character]()
		//	 {
		//		 Character->SetCameraView(Character->GetCameraViewType());
		//		 return FReply::Handled();
		//	 })
		//];
}

#undef LOCTEXT_NAMESPACE
#endif
