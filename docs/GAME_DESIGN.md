# OpenBattle Game Design v0.1

## Core concept

OpenBattle is a vertical social survival shooter set in a 10-floor tower. The game focuses on gun skill, trust, temporary alliances, betrayal, bounty economics, surrender, and upward progression.

## Match structure

- 40 total slots.
- Floors 1-9: four Rebel starting slots per floor.
- Floor 10: four human Security defenders.
- Bots only fill missing slots before a match starts.
- No mid-match bot injection.

## Modes

### Solo
No fixed team. Players may fight, avoid, negotiate, or form alliances during the match.

### Team Work
Players enter as a group and begin with trusted communication while using the same tower rules.

## Floor identity

A player's origin floor remains visible for the whole match. It influences equipment expectations, bounty value, and underdog scoring.

## Alliances

Players can request and accept an alliance at close range. Alliance members share an armband identity and radio channel. Friendly fire and betrayal remain possible.

## ID Card economy

Every dead player drops one unique server-tracked ID Card.

- Any surviving player may pick it up.
- The redeemer does not need to be the killer.
- Each ID can be redeemed once.
- Security Exchange Boxes convert valid IDs into rewards.
- IDs from players with the same origin floor have very high bounty value.
- Security eliminations are also high-value ranked events.

## Equipment curve

Lower floors begin with weaker equipment. Higher floors trend stronger. Low-floor players who beat better-equipped opponents should earn much higher score.

## Tower pressure

Floors transition through:

SAFE -> UNSTABLE -> CRITICAL -> LOST

The system forces upward movement without relying on a giant open-world circle.

## First proof-of-fun

Build only 3 floors / 12 players first:
- movement
- one firearm
- damage/death
- floor identity
- alliance interaction
- ID drop/pickup
- Security Exchange Box
- floor pressure
- dedicated server replication

If the graybox loop is not fun, art does not fix it.
