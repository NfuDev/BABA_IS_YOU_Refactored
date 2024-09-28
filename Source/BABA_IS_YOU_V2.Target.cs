// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.

using UnrealBuildTool;
using System.Collections.Generic;

public class BABA_IS_YOU_V2Target : TargetRules
{
	public BABA_IS_YOU_V2Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "BABA_IS_YOU_V2" } );
	}
}
