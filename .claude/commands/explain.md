Explain how `$ARGUMENTS` works in the vkeng engine, treating this as a learning exercise.

## Approach

1. **Find the relevant code** — read the header and implementation files for the system or concept named.

2. **Explain the Vulkan concepts first** — before diving into the code, explain the underlying Vulkan or graphics concepts at play. What problem does this system solve? Why does Vulkan require this? How does it differ from how OpenGL would do it?

3. **Walk through the code** — go through the implementation step by step, explaining:
   - What each major block of code does.
   - Why it's done in that order (Vulkan often requires a specific sequence).
   - What each Vulkan struct field means and why it's set to that value.
   - Where the data comes from and where it goes next.

4. **Draw connections** — show how this system connects to other parts of the engine. For example, if explaining the Renderer, show how it connects to the SwapChain, Pipeline, RenderPass, and SceneNode.

5. **Identify learning opportunities** — point out:
   - Things the current implementation does well.
   - Things that could be improved (reference known bugs if relevant).
   - Alternative approaches and their tradeoffs.
   - What a more advanced engine would do differently.

## Tone
Be thorough but conversational. Use analogies where they help. Don't skip over "obvious" things — if someone's learning Vulkan, nothing is obvious yet. Explain jargon on first use.
