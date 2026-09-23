#include "OpenBattleGameState.h"
#include "Net/UnrealNetwork.h"

AOpenBattleGameState::AOpenBattleGameState()
{
    bReplicates = true;
    for (int32 Floor = 1; Floor <= 10; ++Floor)
    {
        FOBFloorRuntimeState Entry;
        Entry.Floor = Floor;
        Entry.State = EOBFloorState::Safe;
        FloorStates.Add(Entry);
    }
}

void AOpenBattleGameState::SetFloorState(int32 Floor, EOBFloorState NewState)
{
    if (!HasAuthority() || Floor < 1 || Floor > 10) return;
    for (FOBFloorRuntimeState& Entry : FloorStates)
    {
        if (Entry.Floor == Floor)
        {
            Entry.State = NewState;
            return;
        }
    }
}

EOBFloorState AOpenBattleGameState::GetFloorState(int32 Floor) const
{
    for (const FOBFloorRuntimeState& Entry : FloorStates)
    {
        if (Entry.Floor == Floor) return Entry.State;
    }
    return EOBFloorState::Lost;
}

void AOpenBattleGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AOpenBattleGameState, FloorStates);
}
