"""dev3 dev-server entry point for Cossacks 1.52.

Kills any running dmcr.exe, builds the solution from this worktree, copies the
fresh binaries into the game directory and launches the game. Runs in the
foreground so that "Dev Server: stop" terminates the game.

cwd is the task worktree; the game data lives only in the main checkout, which
dev3 exposes as $DEV3_PROJECT_PATH.
"""

import os
import shutil
import subprocess
import sys
import time
from pathlib import Path

GAME_DIR_NAME = "Cossacks Back to War v1.52 (2025)"
BINARIES = ["dmcr.exe", "IChat.dll", "IntExplorer.dll"]


def log(msg):
    print(f"[dev-run] {msg}", flush=True)


def fail(msg):
    log(f"ERROR: {msg}")
    sys.exit(1)


def find_worktree():
    # The script lives at <worktree>/.dev3/dev-run.py
    return Path(__file__).resolve().parent.parent


def find_game_dir():
    project_path = os.environ.get("DEV3_PROJECT_PATH")
    candidates = []
    if project_path:
        candidates.append(Path(project_path) / GAME_DIR_NAME)
    candidates.append(find_worktree() / GAME_DIR_NAME)
    for c in candidates:
        if c.is_dir():
            return c
    fail(
        "game directory not found, looked in:\n  "
        + "\n  ".join(str(c) for c in candidates)
    )


def kill_game():
    # /F because the game ignores WM_CLOSE while in fullscreen.
    r = subprocess.run(
        ["taskkill", "/IM", "dmcr.exe", "/F"],
        capture_output=True,
        text=True,
    )
    if r.returncode == 0:
        log("killed running dmcr.exe")
        time.sleep(1)  # let Windows release the file locks
    else:
        log("no running dmcr.exe")


def build(worktree):
    log("building...")
    # build.bat ends with `pause`; NUL on stdin makes it return immediately.
    r = subprocess.run(
        f'"{worktree / "build.bat"}" < NUL',
        shell=True,
        cwd=worktree,
    )
    if r.returncode != 0:
        fail(f"build failed (exit {r.returncode})")
    log("build OK")


def deploy(worktree, game_dir):
    src_dir = worktree / "src" / "Testing"
    for name in BINARIES:
        src = src_dir / name
        if not src.is_file():
            fail(f"missing build artifact: {src}")
        for attempt in range(5):
            try:
                shutil.copy2(src, game_dir / name)
                break
            except PermissionError:
                # A dying process can hold the lock for a moment.
                time.sleep(1)
        else:
            fail(f"could not copy {name} — file is locked")
    log(f"deployed {len(BINARIES)} binaries to {game_dir}")


def run(game_dir):
    exe = game_dir / "dmcr.exe"
    log(f"launching {exe}")
    proc = subprocess.Popen([str(exe)], cwd=str(game_dir))
    try:
        code = proc.wait()
    except KeyboardInterrupt:
        proc.terminate()
        raise
    log(f"game exited with code {code}")
    return code


def main():
    worktree = find_worktree()
    game_dir = find_game_dir()
    log(f"worktree:  {worktree}")
    log(f"game dir:  {game_dir}")

    kill_game()
    build(worktree)
    deploy(worktree, game_dir)
    return run(game_dir)


if __name__ == "__main__":
    sys.exit(main() or 0)
