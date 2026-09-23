using UnrealBuildTool;
using System.Collections.Generic;

public class OpenBattleEditorTarget : TargetRules
{
    public OpenBattleEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.AddRange(new string[] { "OpenBattle" });
    }
}
