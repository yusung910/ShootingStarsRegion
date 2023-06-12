// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PFOnlineGame : ModuleRules
{
	public PFOnlineGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.AddRange(new string[] {
			"PFOnlineGame",

			"PFOnlineGame/ASplines",

			"PFOnlineGame/Creature",
			"PFOnlineGame/Creature/Monster",
			"PFOnlineGame/Creature/User",
			"PFOnlineGame/Creature/Weapon",

			"PFOnlineGame/Display",
			"PFOnlineGame/Display/Message",
			"PFOnlineGame/Display/UI",
			"PFOnlineGame/Display/UI/MainPlay",
			"PFOnlineGame/Display/UI/Sub",

			"PFOnlineGame/GameMode",
			"PFOnlineGame/GameMode/CharManagement",
			"PFOnlineGame/GameMode/UserPlayMode",
			"PFOnlineGame/GameInsts",

			"PFOnlineGame/Network",
			"PFOnlineGame/Network/VO",
			"PFOnlineGame/Network/Packets"
		});

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "AIModule", "GameplayTasks", "NavigationSystem", "Niagara", "MoviePlayer", "Slate", "SlateCore" });

		//PrivateDependencyModuleNames.AddRange(new string[] {  });


		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
