#include "FramebufferManager.h"

using namespace juce::gl;

FramebufferManager::FramebufferManager()
{
}

FramebufferManager::~FramebufferManager()
{
    cleanup();
}

bool FramebufferManager::initialize(int w, int h)
{
    if (initialized)
        cleanup();

    width = w;
    height = h;

    // Create both framebuffers
    if (!createFramebuffer(0) || !createFramebuffer(1))
    {
        cleanup();
        return false;
    }

    initialized = true;
    currentIndex = 0;

    return true;
}

void FramebufferManager::cleanup()
{
    if (!initialized)
        return;

    deleteFramebuffer(0);
    deleteFramebuffer(1);

    initialized = false;
}

void FramebufferManager::resize(int w, int h)
{
    if (w == width && h == height)
        return;

    initialize(w, h);
}

void FramebufferManager::swap()
{
    currentIndex = 1 - currentIndex;
}

void FramebufferManager::bindWriteFramebuffer()
{
    if (!initialized)
        return;

    glBindFramebuffer(GL_FRAMEBUFFER, fbo[currentIndex]);
}

void FramebufferManager::bindReadTexture(int textureUnit)
{
    if (!initialized)
        return;

    int readIndex = 1 - currentIndex;
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, texture[readIndex]);
}

void FramebufferManager::unbindFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int FramebufferManager::getWriteTextureId() const
{
    return texture[currentIndex];
}

unsigned int FramebufferManager::getReadTextureId() const
{
    return texture[1 - currentIndex];
}

bool FramebufferManager::createFramebuffer(int index)
{
    // Create texture
    glGenTextures(1, &texture[index]);
    glBindTexture(GL_TEXTURE_2D, texture[index]);

    // Allocate texture storage
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Create framebuffer
    glGenFramebuffers(1, &fbo[index]);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo[index]);

    // Attach texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                          GL_TEXTURE_2D, texture[index], 0);

    // Check framebuffer completeness
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        DBG("FlarkViz: Framebuffer " << index << " not complete: " << status);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void FramebufferManager::deleteFramebuffer(int index)
{
    if (fbo[index] != 0)
    {
        glDeleteFramebuffers(1, &fbo[index]);
        fbo[index] = 0;
    }

    if (texture[index] != 0)
    {
        glDeleteTextures(1, &texture[index]);
        texture[index] = 0;
    }
}
