// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class UE4Demo : ModuleRules
{
	public UE4Demo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "MMIFramework", "MMIScene" });

		PrivateDependencyModuleNames.AddRange(new string[] { "MMIScene" });

		PublicIncludePaths.AddRange(new string[] { "../Plugins/UE4_MOSIM/Source/MMMIScene/Public", "../Plugins/UE4_MOSIM/Source/MMMIScene/Private" });
	

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
