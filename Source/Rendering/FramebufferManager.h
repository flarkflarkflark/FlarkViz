#pragma once

#include <JuceHeader.h>

/**
 * @class FramebufferManager
 * @brief Manages ping-pong framebuffers for MilkDrop feedback effects
 *
 * MilkDrop uses texture feedback where the previous frame's output
 * becomes the next frame's input. This requires alternating between
 * two framebuffers (ping-pong technique).
 */
class FramebufferManager
{
public:
    FramebufferManager();
    ~FramebufferManager();

    /**
     * @brief Initialize framebuffers with specified dimensions
     */
    bool initialize(int width, int height);

    /**
     * @brief Cleanup OpenGL resources
     */
    void cleanup();

    /**
     * @brief Resize framebuffers
     */
    void resize(int width, int height);

    /**
     * @brief Swap ping and pong buffers
     */
    void swap();

    /**
     * @brief Bind the current write framebuffer
     */
    void bindWriteFramebuffer();

    /**
     * @brief Bind the current read texture
     */
    void bindReadTexture(int textureUnit = 0);

    /**
     * @brief Unbind framebuffer (render to screen)
     */
    void unbindFramebuffer();

    /**
     * @brief Get current write texture ID
     */
    unsigned int getWriteTextureId() const;

    /**
     * @brief Get current read texture ID
     */
    unsigned int getReadTextureId() const;

    /**
     * @brief Check if initialized
     */
    bool isInitialized() const { return initialized; }

    /**
     * @brief Get dimensions
     */
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    bool initialized = false;
    int width = 0;
    int height = 0;

    // Ping-pong framebuffers
    unsigned int fbo[2] = {0, 0};
    unsigned int texture[2] = {0, 0};

    // Current buffer index (0 or 1)
    int currentIndex = 0;

    // Helper functions
    bool createFramebuffer(int index);
    void deleteFramebuffer(int index);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FramebufferManager)
};
