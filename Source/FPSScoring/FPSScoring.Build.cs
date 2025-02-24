// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FPSScoring : ModuleRules
{
	public FPSScoring(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "FPSScoring" });
        
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "NavigationSystem", "UMG" });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "GameplayAbilities",
            "GameplayTasks",
            "GameplayTags"
        });
    }
}
