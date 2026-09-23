# OpenBattle

OpenBattle is an open-source **vertical social survival shooter** set inside a 10-floor tower.

> 40 players. 10 floors. Trust is gameplay.

## Status

Pre-alpha. The repository now contains the first **MVP / M1 vertical-slice gameplay foundation**.

## Desktop-first targets

- Windows x64
- macOS Apple Silicon
- Linux x86_64
- Linux Dedicated Server

## M1 prototype

The current prototype intentionally compresses the full game into **3 floors / 12 players**:

- Floor 1: 4 Rebels
- Floor 2: 4 Rebels
- Floor 3: 4 Security
- first-person movement
- server-authoritative hitscan shooting
- replicated health / death
- unique ID Card drops and pickup
- Security Exchange Box redemption
- alliance request / accept
- friendly-fire betrayal remains possible
- floor-pressure states and damage
- procedural graybox tower generated from engine primitives

### Controls

- W/A/S/D — move
- Mouse — look
- Space — jump
- Left Mouse — fire
- E — interact / redeem IDs
- G — request alliance
- H — accept alliance

## Full-game target

The production target remains 40 players across 10 floors with four Security defenders on Floor 10.

## Docs

- `docs/GAME_DESIGN.md`
- `docs/ARCHITECTURE.md`
- `docs/ROADMAP.md`
- `docs/MVP.md`
