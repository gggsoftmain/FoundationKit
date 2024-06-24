// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class FoundationEditor : ModuleRules
{
	public FoundationEditor(ReadOnlyTargetRules Target) : base(Target)
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
                "FoundationEditor/Private",
            }
		);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "Foundation"
           }
		);

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Projects",
                "Slate",
                "SlateCore",
                "EditorStyle",
                "Foundation"
            }
        );
    }
}
