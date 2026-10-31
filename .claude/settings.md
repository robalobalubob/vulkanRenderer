# Claude Code Settings Documentation

## Hooks

### Post-Edit C++ Auto-Compile
**Trigger:** After Claude writes or edits any `.cpp` or `.hpp` file.
**What it does:** Runs `cmake --build build` and shows the last 20 lines of output.
**Why:** Catches compile errors immediately instead of discovering them several edits later. If the build fails, Claude sees the error output and can fix it in the same turn.
**Timeout:** 30 seconds. If your build takes longer, increase `timeout` in settings.json.

### Post-Edit Shader Validation
**Trigger:** After Claude writes or edits any `.vert`, `.frag`, `.comp`, `.geom`, `.tesc`, or `.tese` file.
**What it does:** Runs `glslc --target-env=vulkan1.2` on the modified shader to check for GLSL syntax errors.
**Why:** Shader errors only manifest at runtime (pipeline creation failure) which is much harder to debug than catching them at edit time.
**Prerequisite:** `glslc` must be in your PATH (installed with the Vulkan SDK).

## Permissions
The settings pre-allow common safe commands (cmake, glslc, grep, find, etc.) so Claude doesn't have to ask permission for routine operations. Destructive commands like `rm -rf /` and `sudo` are denied.

## Customizing

### Disable a hook temporarily
Comment out or remove the hook entry in settings.json. Claude Code reloads settings on each session start.

### Add project-specific environment
Create `.claude/settings.local.json` (gitignored) for personal overrides:
```json
{
  "env": {
    "VULKAN_SDK": "/path/to/your/vulkansdk"
  }
}
```

### Adjust build timeout
If your build regularly exceeds 30 seconds, increase the timeout value in the PostToolUse hook (value is in milliseconds).
