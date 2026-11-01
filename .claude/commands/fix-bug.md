Fix the known engine bug described by: `$ARGUMENTS`

## Process

1. **Identify the bug** — look up the relevant bug in CLAUDE.md's "Known Bugs" section. If the argument is a number, use that bug number. If it's a description, match it to the closest known issue.

2. **Find all affected code** — search the codebase for every location where this bug manifests. Don't fix just one instance if the same mistake appears in multiple files (e.g., both TestSceneApp and ModelViewerApp often share the same bugs).

3. **Explain what's wrong** — before changing anything, explain:
   - What the current code does.
   - Why that's incorrect (what can go wrong).
   - What the fix is and why it's correct.
   This is a learning project, so the explanation matters as much as the fix.

4. **Apply the fix** — make the minimal, targeted change. Don't refactor surrounding code unless it's necessary for the fix.

5. **Verify** — check that:
   - The fix compiles (no missing includes, no type errors).
   - The fix doesn't break the public API of any class.
   - If the fix touches Vulkan sync or resource management, explain the synchronization reasoning.

6. **Update CLAUDE.md** — after fixing, update the "Known Bugs" section to mark the bug as resolved or remove it.

## Reference
The current known bug list is in CLAUDE.md. The full analysis with detailed explanations of each bug is in the project's analysis document if available.
