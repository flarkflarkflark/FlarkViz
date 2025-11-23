/**
 * FlarkViz Demo - Standalone OpenGL Audio Visualizer
 * No JUCE dependencies - Pure OpenGL + SDL2
 * 
 * This is a working proof-of-concept showing the core visualization
 * while the full JUCE version is being built.
 */

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <cmath>
#include <vector>
#include <random>
#include <iostream>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int SAMPLE_SIZE = 512;

class SimpleVisualizer {
private:
    SDL_Window* window;
    SDL_GLContext glContext;
    std::vector<float> waveform;
    float time;
    bool running;
    
    // flarkAUDIO colors
    float orangeR = 1.0f;
    float orangeG = 0.4f;
    float orangeB = 0.0f;
    
public:
    SimpleVisualizer() : time(0.0f), running(true) {
        waveform.resize(SAMPLE_SIZE, 0.0f);
        
        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
            return;
        }
        
        // Set OpenGL attributes
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        
        // Create window
        window = SDL_CreateWindow(
            "FlarkViz Demo - flarkAUDIO",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
        );
        
        if (!window) {
            std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
            return;
        }
        
        // Create OpenGL context
        glContext = SDL_GL_CreateContext(window);
        if (!glContext) {
            std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
            return;
        }
        
        // Enable VSync
        SDL_GL_SetSwapInterval(1);
        
        std::cout << "FlarkViz Demo initialized!" << std::endl;
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "  ESC or Q - Quit" << std::endl;
        std::cout << "  SPACE - Change color" << std::endl;
        std::cout << "  F - Toggle fullscreen" << std::endl;
    }
    
    ~SimpleVisualizer() {
        if (glContext) SDL_GL_DeleteContext(glContext);
        if (window) SDL_DestroyWindow(window);
        SDL_Quit();
    }
    
    void generateTestWaveform() {
        // Generate interesting test waveform (simulates audio)
        for (int i = 0; i < SAMPLE_SIZE; ++i) {
            float t = time + i * 0.01f;
            waveform[i] = 0.3f * std::sin(t * 2.0f) +
                         0.2f * std::sin(t * 5.0f) +
                         0.1f * std::sin(t * 11.0f);
        }
        time += 0.05f;
    }
    
    void render() {
        // Clear to flarkAUDIO black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw waveform in flarkAUDIO orange
        glColor3f(orangeR, orangeG, orangeB);
        glLineWidth(2.0f);
        
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < SAMPLE_SIZE; ++i) {
            float x = (float)i / SAMPLE_SIZE * 2.0f - 1.0f;
            float y = waveform[i];
            glVertex2f(x, y);
        }
        glEnd();
        
        // Draw frequency spectrum (fake FFT visualization)
        glColor4f(orangeR, orangeG, orangeB, 0.5f);
        glBegin(GL_QUADS);
        for (int i = 0; i < 64; ++i) {
            float x = (float)i / 64.0f * 2.0f - 1.0f;
            float width = 1.8f / 64.0f;
            
            // Simulate FFT magnitude
            float magnitude = std::abs(waveform[i * 8]) * 0.5f;
            
            glVertex2f(x, -0.8f);
            glVertex2f(x + width, -0.8f);
            glVertex2f(x + width, -0.8f + magnitude);
            glVertex2f(x, -0.8f + magnitude);
        }
        glEnd();
        
        // Draw title text (using basic lines)
        drawText("FlarkViz by flarkAUDIO", -0.9f, 0.9f, 0.05f);
        
        SDL_GL_SwapWindow(window);
    }
    
    void drawText(const char* text, float x, float y, float scale) {
        // Simple line-based text rendering (just shows it's working)
        glColor3f(orangeR, orangeG, orangeB);
        glLineWidth(2.0f);
        
        float cursorX = x;
        while (*text) {
            // Just draw a simple vertical line per character (placeholder)
            glBegin(GL_LINES);
            glVertex2f(cursorX, y);
            glVertex2f(cursorX, y - scale);
            glEnd();
            
            cursorX += scale * 0.6f;
            text++;
        }
    }
    
    void handleEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                    
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                        case SDLK_q:
                            running = false;
                            break;
                            
                        case SDLK_SPACE:
                            // Randomize color
                            {
                                std::random_device rd;
                                std::mt19937 gen(rd());
                                std::uniform_real_distribution<float> dist(0.5f, 1.0f);
                                orangeR = dist(gen);
                                orangeG = dist(gen) * 0.5f;
                                orangeB = dist(gen) * 0.3f;
                                std::cout << "Color changed!" << std::endl;
                            }
                            break;
                            
                        case SDLK_f:
                            // Toggle fullscreen
                            {
                                Uint32 flags = SDL_GetWindowFlags(window);
                                if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP) {
                                    SDL_SetWindowFullscreen(window, 0);
                                    std::cout << "Windowed mode" << std::endl;
                                } else {
                                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                                    std::cout << "Fullscreen mode" << std::endl;
                                }
                            }
                            break;
                    }
                    break;
            }
        }
    }
    
    void run() {
        std::cout << "\nFlarkViz Demo Running!" << std::endl;
        std::cout << "Displaying test waveform..." << std::endl;
        
        while (running) {
            handleEvents();
            generateTestWaveform();
            render();
            
            // ~60 FPS
            SDL_Delay(16);
        }
        
        std::cout << "FlarkViz Demo closed." << std::endl;
    }
};

int main(int argc, char* argv[]) {
    std::cout << "========================================" << std::endl;
    std::cout << "    FlarkViz Demo - by flarkAUDIO      " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    SimpleVisualizer viz;
    viz.run();
    
    return 0;
}
