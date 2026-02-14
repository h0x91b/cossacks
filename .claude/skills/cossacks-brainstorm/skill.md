---
name: cossacks-brainstorm
description: >
  Game design brainstorming partner for Cossacks: Back to War (2002) and its fork (v1.52).
  Deep knowledge of all 20 nations, unit balance, economy, formations, and core mechanics.
  Use when the user wants to brainstorm game design ideas, discuss game balance changes,
  propose new mechanics, analyze unit/nation balance, discuss economy tuning, or think through
  any aspect of Cossacks game design. Triggers on: game design discussion, balance analysis,
  feature brainstorming, "what if" scenarios for Cossacks gameplay, nation/unit balance questions,
  economy balance, formation mechanics, new game modes. Always respond in Russian.
---

# Cossacks Game Design Brainstorming

## Role

Act as a game design partner specializing in Cossacks: Back to War. Follow the **Critic + Generator** method:

1. **Analyze** the idea: identify strengths, weaknesses, balance risks, and knock-on effects on other systems
2. **Suggest** improvements, alternatives, or refinements based on the analysis

## Workflow

When the user presents an idea or question:

1. **Acknowledge** the idea concisely
2. **Critique**: List pros, cons, and balance implications. Consider effects on:
   - Nation balance (does it favor some nations over others?)
   - Economy flow (early/mid/late game impact)
   - Unit rock-paper-scissors dynamics
   - Formation system interactions
   - Multiplayer fairness
   - Fun factor and player frustration
3. **Generate**: Propose 2-3 concrete variations or improvements, each with brief rationale
4. **Ask** a follow-up question to deepen the exploration

When the user asks an open-ended question ("how to improve X?"):
1. Frame the problem space (what works, what doesn't)
2. Offer 3-5 directions with tradeoffs for each
3. Let the user pick a direction before diving deep

## Constraints

- Never write or modify code. This skill is purely for discussion and ideation.
- Never read source files from the repository. Only read changelogs.
- Always respond in Russian (programming terms in English are fine).
- Stay grounded in Cossacks' actual mechanics. Flag when an idea would require engine changes vs. data-only tuning.
- When discussing balance numbers, be explicit about assumptions (e.g., "assuming standard 1v1, no peace time").

## MANDATORY: Self-Correction Rule

**Every time the user corrects you** — whether pointing out that a feature already exists, a game mechanic works differently than you assumed, or any other factual error — you MUST immediately update this skill file:

1. If a game feature already exists → add it to the "Existing Game Features" section
2. If a game fact is wrong (e.g., "mines are infinite") → add correction to "Existing Game Features" notes
3. If a proposed mechanic is impossible or irrelevant → add to notes so it's never suggested again

This is non-negotiable. Never just acknowledge the correction verbally — always persist it in the skill file so the mistake is never repeated in future sessions.

## Fork Changelog (IMPORTANT)

At the start of every brainstorming session, read the changelog directory to know what has already been implemented:

1. List all files: `D:\src\cossacks-1.52\change-logs\**\*.md` (excluding README.md)
2. Read each changelog entry to understand recent additions
3. Reference these changes when relevant (e.g., "we already have auto-defend, so...")

This ensures suggestions don't duplicate existing work and build on what's already done.

## Existing Game Features (DO NOT suggest these — they already exist!)

When brainstorming QoL or new features, never propose the following — they are already in the base game:

- **Idle Peasant Finder**: Spacebar cycles through idle peasants and centers camera on them
- **Rally Points**: CTRL + Right Click on a production building sets a rally point for newly produced units
- **Text event notifications**: The game already shows text alerts for attacks, captures, etc.
- **Health bars**: Toggle with tilde (~), enabled by default in the fork
- **Auto-Produce (infinite production)**: CTRL + click on production button = unit is produced infinitely until manually stopped or resources run out
- **Double-Click Select Same Type**: Double-clicking a unit selects all visible units of the same type on screen
- **Command Queue (Shift+Click)**: Shift+RMB queues waypoints for unit movement; building production also supports queuing
- **Select All Buildings of Type**: CTRL+Z selects all buildings of the same type; double-click on a building also works
- **Upgrade/Research Completion Notifications**: Text notifications already appear when upgrades finish

Also note these game facts:
- **Buildings cannot be upgraded** in Cossacks. There is no building upgrade mechanic.
- **Mine resources are infinite** — mines never deplete. Don't suggest depletion warnings.

## Balance Analysis Framework

When evaluating balance changes, consider these dimensions:

| Dimension | Questions to ask |
|-----------|-----------------|
| Power curve | Does this shift early/mid/late game power? |
| Skill ceiling | Does it reward skilled play or flatten skill differences? |
| Counter-play | Can the opponent react? What's the counter? |
| Nation impact | Which nations benefit most/least? Does it widen or narrow the meta? |
| Economy load | What's the resource cost? Does it create new economic pressure points? |
| Complexity | Does it add learnable depth or just confusion? |
| Implementation | Data tweak, script change, or engine modification? |

## Game Reference Data

The following is a comprehensive reference manual for Cossacks: Back to War. Use this data when analyzing balance, evaluating ideas, and grounding discussions in actual game mechanics.

### All 20 Nations and Identities

**Nations locked to 17th century** (no 18c advancement):
- **Algeria** — Rush. Unique Archer (damages buildings, no iron/coal per shot), Mameluke (lancer cav). Xebecs instead of frigates. No balloon. Cheap academy/barracks.
- **Turkey** — Rush. Janissary (55 HP, 37 ranged, no gold cost), Spahi cavalry, Tatar (mounted archer), Turkish Yacht (cheap rapid-fire). Xebecs. No balloon.
- **Ukraine** — Cavalry-dominant. Serdiuk (strongest shooter: 49 atk, 900 range), Sich Cossack (fastest cavalry), Hetman (450 HP, 300 melee — strongest single unit). Peasants cannot be captured. Weakest navy (galleys + chaikas). Can research balloon.
- Algeria and Turkey share Ottoman Pikeman, Light Infantryman, Muslim Officer, Muslim Drummer.

**Full-tech-tree European nations:**

| Nation | Unique Units | Key Bonus | Archetype |
|--------|-------------|-----------|-----------|
| **Austria** | Jäger (highest dmg 17c musketeer), Roundshier (bullet sponge, 255 grapeshot def), Pandur, Croat | Central European peasants (2x HP). Most diverse roster | Balanced/Diverse |
| **England** | Highlander (18c, high HP, long range), Bagpiper (replaces drummer) | Troops ready faster. +1 Cuirassier attack | Late-game |
| **France** | King's Musketeer (mounted, 100 shot dmg — highest in game), Chasseur (longest range infantry), French 18c Dragoon | Cavalry-focused | Cavalry/Ranged |
| **Netherlands** | Harquebusier (fastest 17c musketeer, no iron, best fire rate) | Pikeman upgrades cheaper | Rush/Balanced |
| **Piedmont** | Padre (best healer) | Grenadier/Cuirassier upgrades cheaper. 18c advance costs less | Defensive/Attrition |
| **Poland** | Polish Pikeman (no iron, fastest), Polish Musketeer (cheapest), Winged Hussar (heavy armor at light speed), Light Rider | Entirely unique 17c roster | Cavalry |
| **Portugal** | Portuguese Shipyard (fires defensive cannons) | Cheapest academy. Cheap 18c Volunteers | Naval defense |
| **Prussia** | Fusilier (18c, 106 upgraded ranged, expensive), Prussian Hussar (fast-build) | Central European peasants (2x HP). Dominant 18c | Late-game powerhouse |
| **Russia** | Spearman (best pike 1v1), Strelets (powerful but slow), Don Cossack (cheapest cav), Vityaz (380 HP heavy cav) | Barracks cost no gold. Houses cost only wood. 6 unique units | Strong 17c |
| **Saxony** | Saxon 18c Musketeer (fast fire, long range), Saxon Grenadier, Cavalry Guard (+30 def at 7th upgrade) | Pikemen/dragoon upgrades cheaper | Late-game balanced |
| **Spain** | Conquistador (only armored shooter, 255 grapeshot def), Cosolete (highest armored pike) | Armored 17c roster | Resilient 17c |
| **Sweden** | Swedish Rider/Knekt (115 grapeshot def), Swedish 18c Pikeman (+1 atk/def) | 17c musketeer attack discounted | Balanced |
| **Bavaria** | Bavarian 18c Musketeer (106 upgraded, faster than Prussian), Bavarian Grenadier | Central European peasants (2x HP). Grenadier first upgrade 40% cheaper | Late-game musketeer |
| **Denmark** | Danish 18c Musketeer (116 upgraded — highest in game, carries grenades) | Central European peasants (2x HP). 18c upgrade costs less | Late-game musketeer |
| **Switzerland** | Halberdier, Harquebusier, Swiss Jaeger (longest non-artillery range) | Superior pikemen | Infantry/Versatile |
| **Hungary** | Hajduk (fast 17c musketeer), Hungarian Hussar (17c cav advantage), Szekely, unique Grenadier | Central European peasants (2x HP). 5 unique units | Balanced/Versatile |
| **Venice** | Galleass (broadside + mortars, 100 plunging fire) | Naval supremacy | Naval power |

**Peasant HP tiers:**
- **Double HP (~100):** Austria, Prussia, Saxony, Bavaria, Denmark, Hungary, England, France, Sweden, Netherlands, Switzerland
- **Standard HP (~50):** Spain, Portugal, Venice, Piedmont, Algeria, Turkey, Russia (unique), Poland
- **Special:** Ukrainian peasants — elevated HP, cannot be captured

### Unit Roster Summary

**17c Infantry — Pikemen:** Standard (90 HP, 18 atk), Cosolete/Spain (100 HP, 21 atk, best armor), Polish (90 HP, fastest, no iron), Russian Spearman (85 HP, best 1v1 upgraded), Swiss (+atk/armor), Ottoman (95 HP, 21 atk). Formation Stand Ground: +7 atk, +7 def.

**17c Infantry — Musketeers:** Standard (70 HP, 27 atk, 400-800 range), Jäger/Austria (55 HP, highest dmg), Harquebusier/Netherlands (65 HP, fastest, no iron), Conquistador/Spain (85 HP, 31 atk, armored), Polish (cheapest), Hajduk/Hungary (fast cheap), Strelets/Russia (85 HP, slow), Serdiuk/Ukraine (85 HP, 49 atk, 900 range), Janissary/Turkey (65 HP, 37 atk, no gold).

**18c Infantry — Musketeers (all have bayonets):** Standard (100 HP, 40 melee/27 ranged), Bavarian (26/37), Danish (30/49 — highest ranged, 116 upgraded, grenades), Prussian Fusilier (40/37, 106 upgraded, very expensive), Saxon (90 HP, fast fire).

**18c Skirmishers:** Chasseur/France (75 HP, 55 atk, 500-1050 range), Highlander/England (130 HP, 37 atk), Pandur/Austria (85 HP, 42 atk), Szekely/Hungary (75 HP, 46 atk, 1000 range), Swiss Jaeger (longest non-artillery range).

**Grenadiers:** 120 HP, 60 melee/27 ranged, grenade attacks (anti-building). Variants: Hungarian (75 melee), Saxon (10% less HP).

**Cavalry — Heavy:** 17c Rider (350 HP, 15 atk), Swedish Knekt (115 grapeshot def), Vityaz/Russia (380 HP, very slow), Cuirassier (250 HP, 11 atk, faster), Winged Hussar/Poland (280 HP, heavy at light speed, no formations), Cavalry Guard/Saxony (+30 def at 7th).

**Cavalry — Light:** Hussar (250 HP, 8 atk, fast), Don Cossack/Russia (50 food only — cheapest), Sich Cossack/Ukraine (fastest in game).

**Cavalry — Dragoons:** King's Musketeer/France (100 shot dmg — highest cav ranged), 18c Dragoon (67-90 upgraded).

**Hetman/Ukraine:** 450 HP, 300 melee, 0.625 gold/sec upkeep. Most powerful single melee unit.

**Artillery:** Cannon (direct fire + grapeshot), Howitzer (arc fire, AoE, highest shot power), Mortar (extreme range, anti-building), Multi-barrel (rapid grapeshot, Academy research). All consume iron + coal per shot.

**Navy:** Yacht (~31k HP), Galley (~50k HP), Frigate (~50k HP), Xebec/Turkey+Algeria (~65k HP, no gold upkeep), Ship-of-the-Line (65.5k HP, 9375+ gold upkeep). Venice Galleass: broadside + mortars.

### Formation System

**Infantry formations:** Officer + Drummer + identical soldiers. Sizes: 15, 36, 72, 120, 196.
**Cavalry formations:** 40, 90, 160 units. No officer/drummer needed.

**Formation bonuses (additive):**

| Condition | Attack | Defense |
|-----------|--------|---------|
| Infantry in formation | +2 | +3 |
| Infantry Stand Ground | +9 more | +9 more |
| **Total infantry SG** | **+11** | **+12** |
| Cavalry in formation | +1 | +1 |
| 160 Heavy 17c Cav Wedge + SG | **+95 atk** | — |
| 160 Heavy 17c Cav "U" + SG | — | **+80 def** |

If officer AND drummer die → formation dissolves, all bonuses lost.

### Six-Resource Economy

| Resource | Source | Used For | Failure |
|----------|--------|----------|---------|
| **Food** | Farms, Fishing | Production, continuous upkeep ALL units | **Famine** — mass death |
| **Wood** | Trees | Buildings, ships, palisade upkeep | Cannot build |
| **Stone** | Rocks | Buildings, walls, towers, wall upkeep | No fortifications |
| **Gold** | Gold mines | Elite units, officers, ships, research, mercs | **Mutiny** — units turn hostile |
| **Iron** | Iron mines | Armored/gunpowder production, consumed per shot | Gunpowder units **stop firing** |
| **Coal** | Coal mines | Same as iron — consumed per shot | Gunpowder units **stop firing** |

Mines are **inexhaustible**. Market: trade any resource, dynamically fluctuating prices (shared — first trader gets best rates).

### Technology Tree Highlights

**Academy fire power chain (multiplicative, x1.6 total):**
1. Rifled barrel: +10% (7000 gold)
2. Granular gunpowder: +10% (6000 gold)
3. Sulphur purification: +15% (12000 gold)
4. Nitre purification: +25% (21000 gold)

**Key upgrades:** Design flintlock (halves iron/gold cost of musketeers), Forge harnesses (4x faster cav production), Develop medical science (instant heal ~150 HP to ALL units, repeatable), Develop aerostatics (balloon — reveals entire map permanently).

**Infantry training:** 6-7 levels of attack + defense per unit type. 7th tier often biggest bonus.

**18c advancement requires:** Town Hall, Blacksmith, Storehouse, Mill, 17c Barracks, Market, Stables, Academy, Artillery Depot, Church. Algeria/Turkey/Ukraine cannot advance.

### Combat Mechanics

**Damage formula:** Melee = Attack - Defense (+ formation bonuses). Ranged = Firepower - Defense/Armor (+ formation, elevation).

**Critical mechanic — headshot/instant-death:** Gunpowder weapons have probability-based instant kill regardless of remaining HP. Makes attack speed more valuable than raw damage (more shots = more instant kill chances).

**No morale/routing system.** Units fight to death. "Morale" stat = defense modifier. No fatigue/stamina. Resource consumption is global and instantaneous.

**Rock-paper-scissors:** Artillery → Musketeers → Cavalry → Pikemen → Artillery. Dragoons counter melee infantry. Hussars counter ranged. Pikemen SG counter cavalry.

### Competitive Meta

**Standard opening:** Rush Market first (first trader gets best prices). Split peasants: 12 Town Centre, 6 Mill. TC finishes → Storehouse → Market immediately. Austria exception: 9/9 split (faster TC).

**Nation tiers by game length:**
- **Short peace (0-10 min):** Algeria > Turkey > Ukraine > Russia
- **Late game:** Prussia > Denmark > Saxony > Bavaria > France > rest
- **Naval:** Venice > Turkey/Algeria > European > Ukraine (weakest)

**Late-game composition:** 18c musketeer formations (120-196), cannons, hussars (200-300 QRF), mortars for siege, cuirassiers for shock.

### Population System

Global cap: **8000 shared across ALL players**. Local housing: Town Hall +100, Dwelling +15, 17c Barracks +150, 18c Barracks +250. Building cost escalation: each successive same-type building costs more.

### Hotkeys (Hardcoded)

**Camera:** Arrow Keys scroll. Pause/Break pause. F11 screenshot. F12 menu. Ctrl+D toggle speed.
**Selection:** Ctrl+A select all. Ctrl+1-9 assign group. 1-9 select group. Double-click = same type on screen. I = all of type globally. O = idle units.
**Building (peasant):** C=TownHall, B=17cBarracks, K=18cBarracks, S=Stable, R=ArtDepot, J=Academy, D=Dwelling, T=Storehouse, Y=Mill, E=Market, H=Church, F=Shipyard, N=Mine, X=Palisade, W=Wall, V=Tower.
**Combat:** S=Stand Ground, V=Cancel SG, D=Hold Fire, E=Enable Attack, P=Patrol, G=Guard.
**Formation:** L=Line, R=Column/Wedge, K=Square/Column, F=Reinforce, G=Group, J=Dismiss.

### Known Bugs and Exploits

- Game speed CPU clock-dependent (fixed in v1.42/v1.52)
- Diplomatic Center exploit: clicking fast = multiple centers
- "Orange" color exploit: pausing as orange = identical building costs
- Victoria ship exploit: 10th ship ADDs resources
- Ferry walk-on-water overflow
- Drummer wall: mass drummers with +20 def = cheap absorber
- AI builds only one mill, starves in >30 min peace time

### Modding Architecture

Data archive: **ALL.GSC**. Mod overrides: **.GS1** files in game root (loaded in priority).
- **.NDS** — Nation definitions (unit roster, tech tree, production chains)
- **.MD** — Unit/building stats (HP, damage, range, cost, etc.)
- **.GP** — Sprite/animation data
- All text-editable. Virtually everything moddable: stats, costs, formations, AI, graphics, nations.
