using UnrealBuildTool;
using System.Collections.Generic;

public class OpenBattleTarget : TargetRules
{
    public OpenBattleTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.AddRange(new string[] { "OpenBattle" });
    }
}
