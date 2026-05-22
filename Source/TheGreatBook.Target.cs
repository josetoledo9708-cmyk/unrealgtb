using UnrealBuildTool;
using System.Collections.Generic;

public class TheGreatBookTarget : TargetRules
{
	public TheGreatBookTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("TheGreatBook");
	}
}
