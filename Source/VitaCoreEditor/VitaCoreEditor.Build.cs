// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

using UnrealBuildTool;

public class VitaCoreEditor : ModuleRules
{
	public VitaCoreEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
        PublicIncludePaths.Add("VitaCoreEditor/Public");
        PrivateIncludePaths.Add("VitaCoreEditor/Private");

        PublicDependencyModuleNames.AddRange(new string[]{ // VitaCore
            "VitaCore"
        });
        PublicDependencyModuleNames.AddRange(new string[]{ // Core
            "Core", "CoreUObject", "Engine", "InputCore"
        });
        PublicDependencyModuleNames.AddRange(new string[]{ // UI
            "Slate", "SlateCore"
        });
        PublicDependencyModuleNames.AddRange( new string[] { // Editor
            "UnrealEd", "DetailCustomizations", "PropertyEditor", "EditorStyle" 
        });

    }
}
