# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Cossacks: Back to War 1.42 — a refactored and bugfixed version of the original 2002 game (v1.35). 32-bit (x86) Windows application using DirectDraw, DirectPlay, and DirectSound.

## Build

Solution: `src\Cossacks.sln`. Output: `src\Testing\`. Game dir: `Cossacks142\`.

**Full build (Release):**
```
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" "D:\src\cossacks-revamp-2017\src\Cossacks.sln" -p:PlatformToolset=v143 -p:WindowsTargetPlatformVersion=10.0.22621.0 -p:Configuration=Release -p:Platform=x86 -m
```

**Debug build:** Use `-p:Configuration=Testing` (uses `/MTd`).

**Critical build constraints:**
- Do NOT use `-t:Rebuild` on the whole solution — circular .lib dependencies will break.
- If clean rebuild is needed, build individual `.vcxproj` files sequentially using `Platform=Win32` (not `x86`).
- Post-build copies dmcr.exe, IChat.dll, IntExplorer.dll into `Cossacks142\`.

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
