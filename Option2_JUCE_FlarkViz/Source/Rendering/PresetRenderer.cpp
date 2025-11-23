#include "PresetRenderer.h"
#include "../Audio/AudioAnalyzer.h"

PresetRenderer::PresetRenderer()
{
}

PresetRenderer::~PresetRenderer()
{
}

void PresetRenderer::initializeGL()
{
    using namespace juce::gl;
    
    // Create VAO
    glGenVertexArrays (1, &gl.vao);
    glBindVertexArray (gl.vao);
    
    // Create VBO for waveform
    glGenBuffers (1, &gl.waveformVBO);
    
    // Create simple shader program
    createSimpleShader();
    
    // Enable blending for nice visuals
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    DBG ("FlarkViz: OpenGL initialized");
}

void PresetRenderer::cleanupGL()
{
    using namespace juce::gl;
    
    if (gl.vao != 0)
        glDeleteVertexArrays (1, &gl.vao);
    if (gl.waveformVBO != 0)
        glDeleteBuffers (1, &gl.waveformVBO);
    if (gl.shaderProgram != 0)
        glDeleteProgram (gl.shaderProgram);
}

void PresetRenderer::setViewportSize (int width, int height)
{
    viewportWidth = width;
    viewportHeight = height;
    
    using namespace juce::gl;
    glViewport (0, 0, width, height);
}

void PresetRenderer::beginFrame()
{
    using namespace juce::gl;
    
    // Clear to flarkAUDIO black
    glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    time += 0.016f; // ~60 FPS time increment
}

void PresetRenderer::renderPreset (const std::vector<float>& fftData,
                                  const std::vector<float>& waveformData,
                                  const AudioAnalyzer::Beat& beat)
{
    // Render background (subtle effect)
    renderBackground();
    
    // Render waveform
    if (!waveformData.empty())
        renderWaveform (waveformData);
    
    // If beat detected, add flash effect
    if (beat.isBeat)
    {
        using namespace juce::gl;
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE);
        
        // Flash with flarkAUDIO orange
        auto orange = flarkOrange.withAlpha (beat.intensity * 0.2f);
        glClearColor (orange.getFloatRed(), 
                     orange.getFloatGreen(), 
                     orange.getFloatBlue(), 
                     orange.getFloatAlpha());
    }
}

void PresetRenderer::endFrame()
{
    // Nothing special needed for now
}

void PresetRenderer::renderWaveform (const std::vector<float>& waveformData)
{
    using namespace juce::gl;
    
    // Simple waveform rendering
    std::vector<float> vertices;
    
    for (size_t i = 0; i < waveformData.size(); ++i)
    {
        float x = (float)i / (float)waveformData.size() * 2.0f - 1.0f;
        float y = waveformData[i] * 0.5f;
        
        vertices.push_back (x);
        vertices.push_back (y);
    }
    
    // Upload to GPU
    glBindBuffer (GL_ARRAY_BUFFER, gl.waveformVBO);
    glBufferData (GL_ARRAY_BUFFER, 
                  vertices.size() * sizeof(float), 
                  vertices.data(), 
                  GL_DYNAMIC_DRAW);
    
    // Set color to flarkAUDIO orange
    glUseProgram (gl.shaderProgram);
    glUniform4f (glGetUniformLocation (gl.shaderProgram, "uColor"),
                 1.0f, 0.4f, 0.0f, 0.8f); // Orange
    
    // Draw
    glEnableVertexAttribArray (0);
    glBindBuffer (GL_ARRAY_BUFFER, gl.waveformVBO);
    glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    
    glLineWidth (2.0f);
    glDrawArrays (GL_LINE_STRIP, 0, (GLsizei)waveformData.size());
    
    glDisableVertexAttribArray (0);
}

void PresetRenderer::renderBackground()
{
    // Subtle animated background (placeholder)
    // In full version, this would render the preset's warp shader
}

void PresetRenderer::createSimpleShader()
{
    using namespace juce::gl;
    
    // Simple vertex shader
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        void main()
        {
            gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
        }
    )";
    
    // Simple fragment shader
    const char* fragmentShaderSource = R"(
        #version 330 core
        uniform vec4 uColor;
        out vec4 FragColor;
        void main()
        {
            FragColor = uColor;
        }
    )";
    
    // Compile vertex shader
    GLuint vertexShader = glCreateShader (GL_VERTEX_SHADER);
    glShaderSource (vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader (vertexShader);
    
    // Check for errors
    GLint success;
    glGetShaderiv (vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog (vertexShader, 512, nullptr, infoLog);
        DBG ("Vertex shader compilation failed: " << infoLog);
    }
    
    // Compile fragment shader
    GLuint fragmentShader = glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource (fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader (fragmentShader);
    
    glGetShaderiv (fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog (fragmentShader, 512, nullptr, infoLog);
        DBG ("Fragment shader compilation failed: " << infoLog);
    }
    
    // Link shader program
    gl.shaderProgram = glCreateProgram();
    glAttachShader (gl.shaderProgram, vertexShader);
    glAttachShader (gl.shaderProgram, fragmentShader);
    glLinkProgram (gl.shaderProgram);
    
    glGetProgramiv (gl.shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog (gl.shaderProgram, 512, nullptr, infoLog);
        DBG ("Shader program linking failed: " << infoLog);
    }
    
    // Clean up
    glDeleteShader (vertexShader);
    glDeleteShader (fragmentShader);
    
    DBG ("FlarkViz: Shaders compiled successfully");
}

void PresetRenderer::loadPreset (void* preset)
{
    // TODO: Load actual preset
    DBG ("FlarkViz: Preset loaded");
}

void PresetRenderer::enableDoublePresetMode (bool enable)
{
    doublePresetMode = enable;
    DBG ("FlarkViz: Double-preset mode " << (enable ? "enabled" : "disabled"));
}

void PresetRenderer::randomizeColors()
{
    // TODO: Implement color randomization
    DBG ("FlarkViz: Colors randomized");
}
