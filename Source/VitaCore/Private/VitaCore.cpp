// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#include "VitaCore.h"
#include "Modules/ModuleManager.h"
#include "VitaCoreLog.h"

IMPLEMENT_PRIMARY_GAME_MODULE(VitaCoreModule, VitaCore, "VitaCore");

void VitaCoreModule::StartupModule()
{
	VC_LOG(Warning, "VitaCore module Started!");
}

void VitaCoreModule::ShutdownModule()
{
	VC_LOG(Warning, "VitaCore module shutting down!");
}
