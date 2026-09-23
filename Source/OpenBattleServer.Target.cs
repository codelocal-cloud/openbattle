using UnrealBuildTool;
using System.Collections.Generic;

public class OpenBattleServerTarget : TargetRules
{
    public OpenBattleServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.AddRange(new string[] { "OpenBattle" });
    }
}
