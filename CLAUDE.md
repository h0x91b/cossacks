# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Cossacks: Back to War 1.52 — MisterCoderman's fork with SDL2, multithreading, 100+ missions, many fixes. Our custom features (auto-defend, auto-fill, resource overlay) migrated from v1.42 fork. 32-bit (x86) Windows application.

## Build

Solution: `src\Cossacks.sln`. Output: `src\Testing\`. Game dir: `Cossacks Back to War v1.52 (2025)\`.

**Full build (Release):**
```
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" "D:\src\cossacks-1.52\src\Cossacks.sln" -p:PlatformToolset=v143 -p:WindowsTargetPlatformVersion=10.0.22621.0 -p:Configuration=Release -p:Platform=x86 -m
```

**Debug build:** Use `-p:Configuration=Testing` (uses `/MTd`).

**Critical build constraints:**
- Do NOT use `-t:Rebuild` on the whole solution — circular .lib dependencies will break.
- If clean rebuild is needed, build individual `.vcxproj` files sequentially using `Platform=Win32` (not `x86`).
- Post-build copies dmcr.exe, IChat.dll, IntExplorer.dll into `Cossacks Back to War v1.52 (2025)\`.

## Architecture

Four projects with circular inter-dependencies:

```
CommCore.lib  (static library)
    └─► linked into dmcr.exe

dmcr.exe  (main executable, outputs as "dmcr")
    ├─► links CommCore.lib, IChat.lib (import lib)
    ├─► links DirectX (DDraw, DirectPlay, DirectSound), Winsock2
    └─► exports symbols consumed by IChat.dll and IntExplorer.dll

IChat.dll  (chat/network library)
    ├─► links dmcr.lib (import lib from main executable)
    └─► also compiles IntExplorer's ParseRQ.cpp (shared source file)

IntExplorer.dll  (game browser/explorer)
    └─► links dmcr.lib and IChat.lib
```

The circular dependency (dmcr.exe ↔ IChat.dll ↔ IntExplorer.dll) is why `-t:Rebuild` on the whole solution fails — the import .lib files from one project are needed to link another.

## Key Source Locations

- `src/Main executable/` — 100+ source files: game logic, rendering, AI, networking, UI, audio
  - `common.h` — shared header included by DLL projects
  - Subdirs: `Arc/`, `Chat/`, `CEngine/`, `HTTP/`, `GameSpy/`, `Dialogs/`, `NewCode/`, `queryreporting/`
- `src/CommCore library/` — network communication core (CommInet, CommPeers, CommPing, CommQueue, etc.)
- `src/IChat library/` — chat DLL, also compiles `ParseRQ.cpp` from IntExplorer
- `src/IntExplorer library/` — game server browser DLL
- `src/Temp/` — intermediate build files (.obj, .pdb, .bsc)
- `src/Testing/` — final build output (dmcr.exe, DLLs)

## Compiler Settings

- Platform Toolset: v143 (VS2022), Target: x86 (32-bit only)
- Release: `/MT` (static CRT), whole program optimization
- Testing: `/MTd` (debug static CRT), no optimization, debug info
- Warning level 4 for main executable, level 3 for libraries
- C4996 (deprecated functions) is disabled across all projects

## Git Commit Policy

**Always commit your changes before responding to the user.** When you've made changes and reached a logical completion point, commit before reporting back. This prevents source code from being lost.

**Rules for committing:**
- Commit only the files YOU changed. Use `git add <specific files>` — never `git add -A` or `git add .`.
- Multiple agents may work in parallel. Never touch, reset, or discard other agents' changes. If you see unstaged/staged changes in files you didn't modify — leave them alone.
- Never run `git reset --hard`, `git checkout .`, `git clean`, or any destructive git commands that could wipe others' work.
- Write concise commit messages describing what you did.

## Changelog Policy

**For every code change, create a changelog entry file.** This avoids merge conflicts when multiple agents work in parallel.

**Path:** `change-logs/YYYY/MM/DD/<type>-<short-slug>.md`

**Type prefixes:** `feature-`, `fix-`, `refactor-`, `docs-`, `chore-`

**Content:** Plain text, 1-3 sentences describing what was done. No frontmatter, no headers.

**Rules:**
- Include the changelog file in the same commit as the code change.
- The slug must be unique and descriptive enough to avoid collisions between parallel agents.
- See `change-logs/README.md` for the full format specification.

## Scripting

Use python instead of PowerShell when you need to make any utility call
