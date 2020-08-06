// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

using UnrealBuildTool;

public class VitaCore : ModuleRules
{
	public VitaCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
        // Core
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
        // User Widgets
        PublicDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
        // Gameplay Abilities API
        PublicDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks" });
        // AI
        PublicDependencyModuleNames.AddRange(new string[] { "AIModule"});
        // Wwise
        PublicDependencyModuleNames.AddRange(new string[] { "AkAudio"});
        // HTTP and JSON
        PublicDependencyModuleNames.AddRange(new string[] { "Http", "Json", "JsonUtilities" });

        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange( new string[] { "DetailCustomizations", "PropertyEditor", "EditorStyle" });
            PublicDependencyModuleNames.AddRange( new string[] { "UnrealEd" });
        }

    }
}
