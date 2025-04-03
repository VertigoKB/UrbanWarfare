// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class UrbanWarfare : ModuleRules
{
	public UrbanWarfare(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[]
		{
			Path.Combine(ModuleDirectory, "AssetConfig"),
			Path.Combine(ModuleDirectory, "Common"),
			Path.Combine(ModuleDirectory, "Frameworks"),
			Path.Combine(ModuleDirectory, "Frameworks/Components"),
			Path.Combine(ModuleDirectory, "Interfaces"),
			Path.Combine(ModuleDirectory, "Player"),
			Path.Combine(ModuleDirectory, "Player/Components"),
			Path.Combine(ModuleDirectory, "Weapon"),
			Path.Combine(ModuleDirectory, "UI")
		});
			

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "NetCore","InputCore", "EnhancedInput", "AnimGraphRuntime", "LevelSequence", "MovieScene", "CinematicCamera" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
