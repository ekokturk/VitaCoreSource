// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#include "VitaCoreEditor.h"
#include "Modules/ModuleManager.h"
#include "VitaCoreEditorLog.h"

#if WITH_EDITOR
#include "Editor/PropertyEditor/Public/PropertyEditorModule.h"
#include "VCGameModeDetails.h"
#include "VCCharacterDetails.h"
#endif

IMPLEMENT_PRIMARY_GAME_MODULE(VitaCoreEditorModule, VitaCoreEditor, "VitaCoreEditor");

void VitaCoreEditorModule::StartupModule()
{
	VCEDITOR_LOG(Warning, TEXT("VitaCore Editor module started!"));
	
#if WITH_EDITOR
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout("VCGameMode", FOnGetDetailCustomizationInstance::CreateStatic(&FVCGameModeDetails::MakeInstance));
	PropertyModule.RegisterCustomClassLayout("VCCharacter", FOnGetDetailCustomizationInstance::CreateStatic(&FVCCharacterDetails::MakeInstance));
#endif
}


void VitaCoreEditorModule::ShutdownModule()
{
	VCEDITOR_LOG(Warning, TEXT("VitaCore Editor module shuting down!"));
}


