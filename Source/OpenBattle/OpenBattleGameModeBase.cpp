#include "OpenBattleGameModeBase.h"
#include "OpenBattleGameState.h"
#include "OpenBattlePlayerState.h"

AOpenBattleGameModeBase::AOpenBattleGameModeBase()
{
    GameStateClass = AOpenBattleGameState::StaticClass();
    PlayerStateClass = AOpenBattlePlayerState::StaticClass();
}
