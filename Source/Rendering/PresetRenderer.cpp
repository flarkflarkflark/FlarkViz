#include "PresetRenderer.h"

using namespace juce::gl;

PresetRenderer::PresetRenderer()
{
    renderState = std::make_unique<RenderState>();
    framebufferManager = std::make_unique<FramebufferManager>();
}

PresetRenderer::~PresetRenderer()
{
    cleanupGL();
}

void PresetRenderer::initializeGL()
{
    // Create fullscreen quad geometry
    createFullscreenQuad();

    // Initialize framebuffer manager
    if (!framebufferManager->initialize(viewportWidth, viewportHeight))
    {
        DBG("FlarkViz: Failed to initialize framebuffer manager");
        return;
    }

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    DBG("FlarkViz: OpenGL initialized (MilkDrop pipeline ready)");
}

void PresetRenderer::cleanupGL()
{
    if (gl.fullscreenVAO != 0)
        glDeleteVertexArrays(1, &gl.fullscreenVAO);

    if (gl.fullscreenVBO != 0)
        glDeleteBuffers(1, &gl.fullscreenVBO);

    if (framebufferManager)
        framebufferManager->cleanup();

    gl.fullscreenVAO = 0;
    gl.fullscreenVBO = 0;
}

void PresetRenderer::setViewportSize(int width, int height)
{
    viewportWidth = width;
    viewportHeight = height;

    glViewport(0, 0, width, height);

    // Resize framebuffers
    if (framebufferManager && framebufferManager->isInitialized())
        framebufferManager->resize(width, height);
}

void PresetRenderer::beginFrame(float dt)
{
    deltaTime = dt;

    // Clear screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PresetRenderer::renderPreset(float bass, float mid, float treb,
                                  float bassAtt, float midAtt, float trebAtt)
{
    if (!presetLoaded || !renderState)
        return;

    // Update audio data in render state
    renderState->updateAudioData(bass, mid, treb, bassAtt, midAtt, trebAtt);

    // Execute per-frame expressions
    auto& context = renderState->executeFrame(deltaTime);

    // Render warp pass (texture feedback)
    renderWarpPass();

    // Render composite pass (final output to screen)
    renderCompositePass();

    // Swap framebuffers for next frame
    framebufferManager->swap();
}

void PresetRenderer::endFrame()
{
    // Nothing needed for now
}

bool PresetRenderer::loadPreset(const MilkDropPreset& preset)
{
    if (!renderState)
        return false;

    // Load preset into render state (compiles expressions and shaders)
    if (!renderState->loadPreset(preset))
    {
        DBG("FlarkViz: Failed to load preset into RenderState");
        return false;
    }

    presetLoaded = true;
    DBG("FlarkViz: Preset loaded: " << preset.name);
    return true;
}

void PresetRenderer::enableDoublePresetMode(bool enable)
{
    doublePresetMode = enable;
    DBG("FlarkViz: Double-preset mode " << (enable ? "enabled" : "disabled"));
}

void PresetRenderer::createFullscreenQuad()
{
    // Fullscreen quad vertices (position + texcoord)
    float vertices[] = {
        // Position (x, y)  // TexCoord (u, v)
        -1.0f,  1.0f,       0.0f, 1.0f,  // Top-left
        -1.0f, -1.0f,       0.0f, 0.0f,  // Bottom-left
         1.0f, -1.0f,       1.0f, 0.0f,  // Bottom-right

        -1.0f,  1.0f,       0.0f, 1.0f,  // Top-left
         1.0f, -1.0f,       1.0f, 0.0f,  // Bottom-right
         1.0f,  1.0f,       1.0f, 1.0f   // Top-right
    };

    // Create VAO
    glGenVertexArrays(1, &gl.fullscreenVAO);
    glBindVertexArray(gl.fullscreenVAO);

    // Create VBO
    glGenBuffers(1, &gl.fullscreenVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gl.fullscreenVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute (location 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    // TexCoord attribute (location 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                         (void*)(2 * sizeof(float)));

    glBindVertexArray(0);
}

void PresetRenderer::renderWarpPass()
{
    if (!framebufferManager || !renderState)
        return;

    auto* warpShader = renderState->getWarpShader();
    if (!warpShader || warpShader->programId == 0)
        return;

    // Bind write framebuffer
    framebufferManager->bindWriteFramebuffer();

    // Clear framebuffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Use warp shader
    glUseProgram(warpShader->programId);

    // Bind previous frame texture
    framebufferManager->bindReadTexture(0);

    // Bind uniforms
    auto& context = renderState->getContext();
    bindShaderUniforms(*warpShader, context);

    // Draw fullscreen quad
    drawFullscreenQuad();

    // Unbind framebuffer
    framebufferManager->unbindFramebuffer();
}

void PresetRenderer::renderCompositePass()
{
    if (!framebufferManager || !renderState)
        return;

    auto* compositeShader = renderState->getCompositeShader();
    if (!compositeShader || compositeShader->programId == 0)
        return;

    // Render to screen (framebuffer 0)
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Use composite shader
    glUseProgram(compositeShader->programId);

    // Bind warp pass output texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, framebufferManager->getWriteTextureId());

    // Bind uniforms
    auto& context = renderState->getContext();
    bindShaderUniforms(*compositeShader, context);

    // Draw fullscreen quad
    drawFullscreenQuad();
}

void PresetRenderer::bindShaderUniforms(const MilkDrop::CompiledShader& shader,
                                       const MilkDrop::ExecutionContext& context)
{
    // Time variables
    if (shader.loc_time >= 0)
        glUniform1f(shader.loc_time, static_cast<float>(context.time));
    if (shader.loc_frame >= 0)
        glUniform1f(shader.loc_frame, static_cast<float>(context.frame));
    if (shader.loc_fps >= 0)
        glUniform1f(shader.loc_fps, static_cast<float>(context.fps));

    // Audio variables
    if (shader.loc_bass >= 0)
        glUniform1f(shader.loc_bass, static_cast<float>(context.bass));
    if (shader.loc_mid >= 0)
        glUniform1f(shader.loc_mid, static_cast<float>(context.mid));
    if (shader.loc_treb >= 0)
        glUniform1f(shader.loc_treb, static_cast<float>(context.treb));
    if (shader.loc_bass_att >= 0)
        glUniform1f(shader.loc_bass_att, static_cast<float>(context.bass_att));
    if (shader.loc_mid_att >= 0)
        glUniform1f(shader.loc_mid_att, static_cast<float>(context.mid_att));
    if (shader.loc_treb_att >= 0)
        glUniform1f(shader.loc_treb_att, static_cast<float>(context.treb_att));

    // Preset state
    if (shader.loc_zoom >= 0)
        glUniform1f(shader.loc_zoom, static_cast<float>(context.zoom));
    if (shader.loc_rot >= 0)
        glUniform1f(shader.loc_rot, static_cast<float>(context.rot));
    if (shader.loc_cx >= 0)
        glUniform1f(shader.loc_cx, static_cast<float>(context.cx));
    if (shader.loc_cy >= 0)
        glUniform1f(shader.loc_cy, static_cast<float>(context.cy));
    if (shader.loc_dx >= 0)
        glUniform1f(shader.loc_dx, static_cast<float>(context.dx));
    if (shader.loc_dy >= 0)
        glUniform1f(shader.loc_dy, static_cast<float>(context.dy));
    if (shader.loc_warp >= 0)
        glUniform1f(shader.loc_warp, static_cast<float>(context.warp));
    if (shader.loc_sx >= 0)
        glUniform1f(shader.loc_sx, static_cast<float>(context.sx));
    if (shader.loc_sy >= 0)
        glUniform1f(shader.loc_sy, static_cast<float>(context.sy));

    // Wave colors
    if (shader.loc_wave_r >= 0)
        glUniform1f(shader.loc_wave_r, static_cast<float>(context.wave_r));
    if (shader.loc_wave_g >= 0)
        glUniform1f(shader.loc_wave_g, static_cast<float>(context.wave_g));
    if (shader.loc_wave_b >= 0)
        glUniform1f(shader.loc_wave_b, static_cast<float>(context.wave_b));
    if (shader.loc_wave_a >= 0)
        glUniform1f(shader.loc_wave_a, static_cast<float>(context.wave_a));

    // Resolution
    if (shader.loc_resolution >= 0)
        glUniform2f(shader.loc_resolution,
                   static_cast<float>(viewportWidth),
                   static_cast<float>(viewportHeight));

    // Texture sampler
    if (shader.loc_mainTexture >= 0)
        glUniform1i(shader.loc_mainTexture, 0);  // Texture unit 0

    // Custom variables (q1-q32)
    for (int i = 0; i < 32; ++i)
    {
        if (shader.loc_q[i] >= 0)
            glUniform1f(shader.loc_q[i], static_cast<float>(context.q[i]));
    }
}

void PresetRenderer::drawFullscreenQuad()
{
    glBindVertexArray(gl.fullscreenVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
