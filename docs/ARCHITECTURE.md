# OpenBattle Architecture v0.1

## Principles

1. Server authoritative for damage, inventory, score, ID redemption, alliance state, and floor state.
2. Desktop-first portable gameplay core.
3. Windows, macOS, and Linux clients share one simulation protocol.
4. Linux Dedicated Server is the authoritative server target.
5. Tower content should stream by floor/sector.
6. Gameplay is proven in graybox before heavy art production.

## Runtime

Desktop Client
-> input / rendering / local prediction
-> Linux Dedicated Server
-> movement / combat / inventory / IDs / alliance / score / floor state
-> backend later for accounts, matchmaking, rank, and cosmetics

## Initial Unreal classes

- AOpenBattleGameModeBase
- AOpenBattleGameState
- AOpenBattlePlayerState
- Shared enums/structs in OpenBattleTypes

## Networking roadmap

### Phase A
3-floor, 12-player LAN graybox with authoritative combat and replicated player/floor state.

### Phase B
Internet sessions, reconnect handling, bandwidth profiling, and relevance by floor.

### Phase C
40-player soak tests, hotspots around stairs/elevators, packet-loss tests, and anti-cheat evaluation.

## Voice

Voice should be separated from gameplay replication and expose:
- proximity voice
- alliance radio
- Security radio

Any provider must support Windows, macOS, and Linux.
