using UnrealBuildTool;
using System.Collections.Generic;

public class TheGreatBookEditorTarget : TargetRules
{
	public TheGreatBookEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("TheGreatBook");
	}
}
