// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCGameModeDetails.h"


#if WITH_EDITOR

#include "Editor/PropertyEditor/Public/DetailLayoutBuilder.h"
#include "Editor/PropertyEditor/Public/DetailCategoryBuilder.h"
#include "Editor/PropertyEditor/Public/DetailWidgetRow.h"
#include "Runtime/Slate/Public/Widgets/Text/STextBlock.h"
#include "Runtime/Slate/Public/Widgets/Input/SButton.h"

#include "VitaCore/Public/Gameplay/VCTeam.h"
#include "VitaCore/Public/World/GameModes/VCGameMode.h"

#define LOCTEXT_NAMESPACE "VCGameModeDetails"

TSharedRef<IDetailCustomization> FVCGameModeDetails::MakeInstance()
{
	return MakeShareable(new FVCGameModeDetails);
}



void FVCGameModeDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("VC Settings", FText::GetEmpty(), ECategoryPriority::Important);
	TArray< TWeakObjectPtr<UObject> > ObjectsCustomized;
	DetailBuilder.GetObjectsBeingCustomized(ObjectsCustomized);
	UObject* ObjectCustomized = (ObjectsCustomized.Num() > 0) ? ObjectsCustomized[0].Get() : NULL;
	//AWorldSettings* WorldSettings = Cast<AWorldSettings>(ObjectCustomized);
	//check(WorldSettings)

	Category.AddCustomRow(LOCTEXT("RowSearchName","Team"))
	.NameContent()
	[
		SNew(STextBlock)
		.Text(LOCTEXT("DetailName", "Team Generation"))
		.Font(IDetailLayoutBuilder::GetDetailFont())
	]
	.ValueContent().HAlign(EHorizontalAlignment::HAlign_Fill)
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot().Padding(FMargin(0, 5, 5, 5))[
			SNew(SButton)
				.Text(LOCTEXT("CreateButtonText", "Create Team"))
				.HAlign(HAlign_Center)
				.ToolTipText(LOCTEXT("CreateButtonTooltip", "Spawn team actors in the level defined in the game mode without changing existing ones."))
				.OnClicked_Lambda([ObjectCustomized]()
				{
					if(AVCGameMode * GameMode = Cast<AVCGameMode>(ObjectCustomized))
					{
						GameMode->CreateTeamActors();
						GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileSuccess_Cue.CompileSuccess_Cue"));
						return FReply::Handled();
					}
					GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileFailed_Cue.CompileFailed_Cue"));
					return FReply::Unhandled();
				})
			]
		+ SHorizontalBox::Slot().Padding(FMargin(0, 5, 5, 5))[
			SNew(SButton)
			.Text(LOCTEXT("RefreshButtonText", "Refresh Teams"))
			.HAlign(HAlign_Center)
			.ToolTipText(LOCTEXT("RefreshButtonTooltip", "Remove all team actors from the level and re-create them."))
			.OnClicked_Lambda([ObjectCustomized]()
			{
				if (AVCGameMode* GameMode = Cast<AVCGameMode>(ObjectCustomized))
				{
					GameMode->RefreshTeamActors();
					GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileSuccess_Cue.CompileSuccess_Cue"));
					return FReply::Handled();
				}
				GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileFailed_Cue.CompileFailed_Cue"));
				return FReply::Unhandled();
			})
		]
	];
}

#undef LOCTEXT_NAMESPACE
#endif
