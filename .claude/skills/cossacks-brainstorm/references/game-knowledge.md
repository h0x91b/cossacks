# Cossacks: Back to War - Game Knowledge Reference

## Overview

Cossacks: Back to War (2002, GSC Game World) - real-time strategy set in 17th-18th century Europe. Standalone expansion to Cossacks: European Wars (2001). Massive-scale battles (up to 8000 units), deep economy, formation-based combat.

## Nations (20 in Back to War)

### European Wars nations (16):
Algeria, Austria, England, France, Netherlands, Piedmont, Poland, Portugal, Prussia, Russia, Saxony, Spain, Sweden, Turkey, Ukraine, Venice

### Back to War additions (4):
Bavaria, Denmark, Hungary, Switzerland

### Nation differentiation:
- Unique units (1-3 per nation, typically 18th century)
- Unique bonuses (building costs, unit stats, economy modifiers)
- Different available unit rosters (not all nations get all generic units)
- Same tech tree structure, different available branches

## Resources (6 types)

| Resource | Source | Role |
|----------|--------|------|
| Food | Farms (mills) | Unit production, upkeep |
| Wood | Trees (peasants chop) | Buildings, ships |
| Stone | Stone deposits (peasants mine) | Buildings, walls, towers |
| Gold | Gold mines | Units, upgrades, upkeep |
| Iron | Iron mines | Advanced units, upgrades |
| Coal | Coal mines | Advanced units (18c), cannons |

### Economy mechanics:
- Peasants are the sole gatherers (except mines which house peasants inside)
- Mines have capacity limits (typically 5 peasants base, upgradeable)
- Market allows resource trading (exchange rates worsen with volume)
- Upkeep: 18th century units and formations consume food + gold continuously
- Running out of food/gold with upkeep units = mass desertion
- Farms produce food automatically once built (with peasants inside mill)
- Gathering rates upgradeable at mill/academy

## Units

### Peasants
- Build all structures, gather wood/stone/food, enter mines
- No combat value, cheap, essential for economy
- Produced at Town Hall

### Infantry (17th century):
- **Pikemen** - cheap melee, strong vs cavalry, weak vs ranged
- **Musketeers (17c)** - ranged, slow reload, decent damage
- **Roundshiers** - sword+shield melee, fast, good raid units
- Produced at Barracks

### Infantry (18th century):
- **Musketeers (18c)** - better stats, require upkeep (food+gold)
- **Grenadiers** - elite ranged, grenade attack, expensive
- **Unique national units** - Highlanders (England), Pandurs (Austria), etc.
- Produced at 18th century Barracks
- ALL 18c infantry require upkeep

### Cavalry:
- **Light cavalry** (17c) - fast, cheap, raiding
- **Dragoons** - ranged cavalry, versatile
- **Heavy cavalry** (cuirassiers, hussars) - expensive, devastating charge
- **Unique cavalry** - Winged Hussars (Poland), Cossack cavalry (Ukraine), etc.
- Produced at Stables / 18th century Stables

### Artillery:
- **Cannons** (light, medium, heavy) - siege and anti-infantry
- **Mortars** - long range, area damage, siege
- **Multi-barrel cannon** - anti-infantry specialist
- Produced at Artillery Depot
- Require coal + iron

### Naval:
- **Galley** - cheap, oar-powered
- **Frigate** - standard warship
- **Battleship** - heavy warship
- **Yacht** - fast scout
- Produced at Shipyard

## Formations

Core combat mechanic. Units in formation get massive stat bonuses.

### Requirements:
- 15, 36, 72, 120, or 196 identical units
- 1 Officer + 1 Drummer per formation
- Officer/Drummer produced at special buildings (Academy for officers, Diplomatic Center for drummers in some versions)

### Formation bonuses:
- +100% to +300% attack and defense (depending on formation size)
- Morale system: formations can break and rout
- Officer death = formation breaks
- Drummer death = morale penalty

### Morale:
- Affected by: casualties, nearby routs, officer death, drummer death
- Low morale = units flee uncontrollably
- Can cascade: one rout triggers nearby formations to break

## Buildings

### Economy:
- **Town Hall** - produces peasants, main building
- **Mill** - food production (peasants work inside)
- **Storehouse** - enables wood/stone gathering in area
- **Mine** (Gold/Iron/Coal) - built on deposits, peasants enter to gather
- **Market** - resource trading

### Military:
- **Barracks** (17c, 18c) - infantry
- **Stables** (17c, 18c) - cavalry
- **Artillery Depot** - cannons, mortals
- **Shipyard** - naval units
- **Academy** - officers, research
- **Diplomatic Center** - drummers, diplomacy

### Defense:
- **Walls** (wood, stone) - block movement
- **Towers** - ranged defense
- **Gates** - passage through walls
- Walls can be very long, create chokepoints

### Building capture:
- Enemy units can capture buildings by standing near them with no defenders
- Captured buildings switch ownership
- Key strategic mechanic - must defend outlying buildings

## Tech Tree / Upgrades

### Academy upgrades:
- Unit stat upgrades (attack, defense, HP) by era and type
- Economy upgrades (gathering speed, building speed)
- Multiple levels per upgrade

### Blacksmith:
- Weapon and armor upgrades
- Affect specific unit categories

### Other:
- Mill upgrades (food production rate)
- Mine upgrades (capacity, speed)
- Market upgrades (better exchange rates)

## Keyboard Shortcuts (from source code analysis)

### Selection

| Key | Action |
|-----|--------|
| **Ctrl+A** | Select all military units |
| **Ctrl+B** | Select all buildings |
| **Ctrl+S** | Select all ships |
| **Ctrl+P** | Select all idle peasants |
| **Ctrl+M** | Select all unfilled mines |
| **Ctrl+Z** | Select all units of the currently selected type on screen |
| **Z** | Select all units of the selected type on screen (same as Ctrl+Z) |
| **Double-click** | Select all units of clicked type on screen (within 600ms, <16px movement) |
| **0-9** | Recall saved unit group |
| **Ctrl + 0-9** | Save current selection to group (50ms sticky Ctrl window) |
| **Shift + group recall** | Add group to current selection |
| **Ctrl+click (on unit type icon)** | Deselect that unit type from selection |
| **Shift+click (on unit type icon)** | Add that unit type to selection |

### Unit Commands

| Key | Action |
|-----|--------|
| **A** | Attack-move mode (go and attack) |
| **B** | Selects formation style / unit action (SpecCmd 10) |
| **Ctrl+F** | Formation-related command (SpecCmd 13) |
| **F** | Formation-related command (SpecCmd 14) |
| **Delete** | Delete selected units/buildings |
| **Backspace** | Jump camera to last action/event location |

### Production Queue Multipliers

When clicking a production button, hold these keys to produce multiple units at once:

| Key | Multiplier |
|-----|-----------|
| **F5** | x250 |
| **Tab** | x50 |
| **F2** | x36 (one full formation) |
| **Alt** | x20 |
| **F1** | x15 (one small formation) |
| **Shift** | x5 |
| *(none)* | x1 |

### Display & Information

| Key | Action |
|-----|--------|
| **I** | Toggle statistics/information overlay (Inform mode 1) |
| **Ctrl+I** | Toggle InfoMode (detailed info) |
| **U** | Toggle unit statistics overlay (Inform mode 2) |
| **~ (tilde)** | Toggle health bars display (HealthMode) |
| **O** | Toggle transparency mode (TransMode) |
| **Ctrl+O** | Toggle options panel (OptHidden) |
| **M** | Toggle full minimap |
| **Q** | Cycle grid lock modes (off / 2 / 3) |
| **Caps Lock** | Toggle ego flag (camera follows selection) |
| **E** | Toggle fast rendering mode (editor) |

### Game Control

| Key | Action |
|-----|--------|
| **F12** | Open main menu |
| **F1** | Help menu (or send message to player 1 in multiplayer) |
| **F2-F8** | Send message to player 2-8 |
| **F9** | Exit edit mode / send message to player 9 |
| **F11** | Save screenshot |
| **Escape** | Cancel current mode/action (build, patrol, guard, attack-move) |
| **Pause** | Pause game (if not locked) |
| **Enter** | Enter chat mode |
| **P** | Pop-up diplomacy panel (in multiplayer) |
| **J** | Show full statistics screen (in replay/observer mode) |
| **K / Ctrl+K** | Adjust game speed (RealPause +2 / -2) |

### Camera & Map

| Key | Action |
|-----|--------|
| **Arrow keys** | Scroll map |
| **Shift (on minimap)** | Render full minimap |
| **NumPad 1-8** | Switch to player 1-8 view (single player / editor only) |

### Configurable Key Bindings

The game supports configurable key bindings through config files, mapped via `KeyCodes[][]` array. Supports 68 bindable keys (0-9, A-Z, NumPad 0-9, F1-F10, PgUp, PgDn, Home, End, Insert, and math operators). Modifier states: 1=Ctrl, 2=Alt, 4=Shift.

## Game Phases / Tempo

### Early game (0-10 min):
- Peasant boom, secure resources
- Build basic military for defense
- Scouts to find enemy
- Rush strategies possible (17c barracks units)

### Mid game (10-25 min):
- Transition to 18c units
- Formation-based armies
- Artillery appears
- Economy must sustain upkeep
- Territory control, mine control

### Late game (25+ min):
- Massive armies (thousands of units)
- Artillery-heavy compositions
- Naval battles if water map
- Economy warfare (deny mines, raid peasants)
- Wall/fortification sieges

## Key Balance Dynamics

### Rock-Paper-Scissors:
- Pikemen counter cavalry (charge defense)
- Cavalry counter artillery/ranged (speed to close distance)
- Musketeers counter pikemen (ranged kills before contact)
- Artillery counters formations (area damage)
- Light cavalry counters artillery/economy (raids)

### Economy vs Military tradeoff:
- More peasants = stronger economy but weaker army
- Early aggression vs economic boom
- 18c units are powerful but require continuous resource drain

### Formation vs no-formation:
- Formations are 2-4x stronger but require officers/drummers
- Losing officer = entire formation breaks
- Sniping officers is a valid strategy
- Non-formation units are more flexible but weaker

### Map control:
- Mines are limited and critical
- Controlling central mines = economic advantage
- Walls + towers can lock down areas
- Naval control on water maps is decisive

## Known Balance Issues (original game)

- Some nations have strictly better unique units than others
- 18c musketeer rush (with upkeep economy) can be overwhelming
- Artillery spam in late game can be hard to counter
- Some nations lack key unit types (no cavalry, no good 18c infantry)
- Market abuse: certain resource conversion chains are too efficient
- Peasant rush: overwhelming early game with mass cheap units
- Tower/wall spam: defensive play can stall games indefinitely
- Formation size 196 is disproportionately strong relative to cost of officer/drummer

## Multiplayer Considerations

- Lockstep simulation: all players execute same game logic deterministically
- Game speed affects all players equally
- Common game modes: 1v1, 2v2, FFA, team games
- Popular maps: land, island, coastal
- Common settings: peace time (10-30 min), no peace time (aggressive)
- Diplomatic system allows alliances mid-game
