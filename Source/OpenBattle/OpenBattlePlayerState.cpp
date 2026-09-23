#include "OpenBattlePlayerState.h"
#include "Net/UnrealNetwork.h"

AOpenBattlePlayerState::AOpenBattlePlayerState()
{
    bReplicates = true;
}

void AOpenBattlePlayerState::SetOriginFloor(int32 NewFloor)
{
    if (HasAuthority()) OriginFloor = FMath::Clamp(NewFloor, 1, 10);
}

void AOpenBattlePlayerState::SetRole(EOBRole NewRole)
{
    if (HasAuthority()) Role = NewRole;
}

void AOpenBattlePlayerState::SetAlliance(const FOBAllianceIdentity& NewAlliance)
{
    if (HasAuthority()) Alliance = NewAlliance;
}

void AOpenBattlePlayerState::ClearAlliance()
{
    if (HasAuthority()) Alliance = FOBAllianceIdentity{};
}

void AOpenBattlePlayerState::AddRankScore(int32 Delta)
{
    if (HasAuthority()) RankScore = FMath::Max(0, RankScore + Delta);
}

void AOpenBattlePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AOpenBattlePlayerState, OriginFloor);
    DOREPLIFETIME(AOpenBattlePlayerState, Role);
    DOREPLIFETIME(AOpenBattlePlayerState, Alliance);
    DOREPLIFETIME(AOpenBattlePlayerState, RankScore);
}
