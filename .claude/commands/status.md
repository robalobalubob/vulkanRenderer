Give me a status report on the vkeng engine.

## Gather Information

1. Read `CLAUDE.md` for the current known bugs and missing features list.
2. Check `CMakeLists.txt` for the current source file list and dependencies.
3. Quickly scan for any TODO, FIXME, or HACK comments in the codebase:
   ```
   grep -rn "TODO\|FIXME\|HACK\|XXX\|WORKAROUND" src/ include/ shaders/ --include="*.cpp" --include="*.hpp" --include="*.h" --include="*.vert" --include="*.frag"
   ```
4. Check if the build is currently passing:
   ```
   cmake --build build --parallel 2>&1 | tail -5
   ```

## Report Format

### Build Status
- Does it compile cleanly? Any warnings?

### Source File Count
- How many `.cpp` and `.hpp` files total?
- Rough line count if easy to determine.

### Known Bugs Remaining
- List from CLAUDE.md, noting which are high/medium/low severity.

### TODOs in Code
- Summarize any TODO/FIXME comments found.

### Missing Features (from CLAUDE.md)
- List what's still needed, organized by priority.

### Suggested Next Step
- Based on the current state, what's the single most impactful thing to work on next? Explain why.

Keep the report concise — this is a quick pulse check, not a deep analysis.
