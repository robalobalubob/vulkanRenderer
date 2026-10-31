Plan the next development milestone for: `$ARGUMENTS`

If no argument is given, propose the next logical milestone based on the current engine state.

## Process

1. **Assess current state** — read CLAUDE.md's known bugs and missing features sections. Run `/status` mentally to understand where things stand.

2. **Define the milestone** — a milestone should be a cohesive set of changes that together unlock a new capability. Good milestones:
   - "Textured rendering" (texture binding + descriptor sets + shader sampling + fallback texture)
   - "Multi-material support" (material sorting + pipeline variants + blend states)
   - "Basic shadow mapping" (shadow pass + depth-only pipeline + shadow map sampling)

   Bad milestones (too scattered):
   - "Fix 3 bugs and add instancing and also MSAA"

3. **Break into ordered tasks** — list the specific tasks in the order they should be done. Each task should be small enough to complete in one Claude Code session. For each task:
   - What files are created or modified.
   - What Vulkan concepts are involved.
   - What you'll learn from doing it.
   - How to verify it works (what should you see on screen, what validation errors should disappear).

4. **Identify dependencies** — note if any task depends on a bug fix from the known bugs list. If so, include the bug fix as the first task.

5. **Estimate complexity** — for each task, label it:
   - 🟢 **Small** — single file change, well-understood pattern.
   - 🟡 **Medium** — multiple files, new Vulkan concepts to learn.
   - 🔴 **Large** — architectural change, new shaders, significant testing needed.

## Output Format

```
## Milestone: [Name]
[1-2 sentence description of what this unlocks]

### Task 1: [Name] 🟢
- Files: ...
- Concepts: ...
- Verification: ...

### Task 2: [Name] 🟡
- Files: ...
- Depends on: Task 1
- Concepts: ...
- Verification: ...

...

### Done Condition
[What the engine can do after this milestone that it couldn't before]
```
