// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class FoundationSample : ModuleRules
{
	public FoundationSample(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDefinitions.AddRange(
			new string[] {
				"ED25519_CUSTOMRNG",
				"ED25519_CUSTOMHASH",
				"ED25519_NO_SEED"
			}
		);
		
		bEnableExceptions = true;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicDefinitions.Add("_CRT_SECURE_NO_WARNINGS");
		}

		bEnableUndefinedIdentifierWarnings = false;

		PublicIncludePaths.AddRange(
			new string[]
			{
				Path.Combine(ModuleDirectory, "Public")
			}
		);


		PrivateIncludePaths.AddRange(
			new string[]
			{
				// ... add other private include paths required here ...
			}
		);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"HTTP",
				"Json", 
				"JsonUtilities",
				"UMG",
				"OpenSSL",
				"WebSockets",
                "Foundation"
            }
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"ApplicationCore",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"DeveloperSettings",
			}
		);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
		);
	}
}
