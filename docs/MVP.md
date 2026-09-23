# OpenBattle MVP — M1 Vertical Slice

The first MVP is intentionally small: three floors and twelve players.

## Miniature match

- Floor 1: 4 Rebels
- Floor 2: 4 Rebels
- Floor 3: 4 Security
- Server authoritative
- First-person movement
- Hitscan firearm
- Health, damage, death
- Death drops a unique ID Card
- Walking over an ID Card collects it
- Press E while looking at a Security Exchange Box to redeem carried IDs
- Same-origin-floor ID: +500 bonus score
- Security ID: +500 bonus score
- Redeeming IDs restores a small amount of health
- Press G while aiming at another player to request an alliance
- Target presses H to accept
- Friendly fire remains possible
- Floor pressure advances Safe -> Unstable -> Critical -> Lost

## Controls

- W/A/S/D: move
- Mouse: look
- Space: jump
- Left Mouse: fire
- E: interact / redeem
- G: request alliance
- H: accept alliance

## Floor pressure timing

Floor 1:
- 60s Unstable
- 90s Critical
- 120s Lost

Floor 2:
- 120s Unstable
- 150s Critical
- 180s Lost

Floor 3 remains the Security hold floor for this slice.

## Graybox

The prototype tower is generated in C++ from Unreal Engine cube primitives. No custom art assets or hand-authored level are required to test the first gameplay loop.
