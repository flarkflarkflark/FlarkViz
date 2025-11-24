#include "RenderState.h"
#include <cmath>

RenderState::RenderState()
{
    perFrameInitEval = std::make_unique<MilkdropEval>();
    perFrameEval = std::make_unique<MilkdropEval>();
    perPixelEval = std::make_unique<MilkdropEval>();
}

RenderState::~RenderState()
{
}

void RenderState::reset()
{
    context = MilkDrop::ExecutionContext();
    frameCount = 0;
    totalTime = 0.0f;
    perFrameInitExecuted = false;
    currentPreset = nullptr;

    perFrameInitEval->clear();
    perFrameEval->clear();
    perPixelEval->clear();

    warpShader.reset();
    compositeShader.reset();
}

bool RenderState::loadPreset(const MilkDropPreset& preset)
{
    reset();
    currentPreset = &preset;

    // Compile per-frame init code
    if (!preset.perFrameInitCode.empty())
    {
        if (!perFrameInitEval->compileBlock(preset.perFrameInitCode))
        {
            return false;
        }
    }

    // Compile per-frame code
    if (!preset.perFrameCode.empty())
    {
        if (!perFrameEval->compileBlock(preset.perFrameCode))
        {
            return false;
        }
    }

    // Compile per-pixel code (for future use)
    if (!preset.perPixelCode.empty())
    {
        if (!perPixelEval->compileBlock(preset.perPixelCode))
        {
            return false;
        }
    }

    // Compile shaders
    if (!preset.warpShaderCode.empty())
    {
        warpShader = shaderCompiler.compileMilkDropShader(
            preset.warpShaderCode,
            MilkDrop::ShaderType::Warp);
    }
    else
    {
        warpShader = shaderCompiler.createDefaultShader(MilkDrop::ShaderType::Warp);
    }

    if (!preset.compShaderCode.empty())
    {
        compositeShader = shaderCompiler.compileMilkDropShader(
            preset.compShaderCode,
            MilkDrop::ShaderType::Composite);
    }
    else
    {
        compositeShader = shaderCompiler.createDefaultShader(MilkDrop::ShaderType::Composite);
    }

    // Initialize preset parameters into context
    context.zoom = preset.fDecay;
    context.rot = preset.fRot;
    context.cx = preset.fRotCX;
    context.cy = preset.fRotCY;
    context.dx = preset.fXPush;
    context.dy = preset.fYPush;
    context.warp = preset.fWarpAmount;
    context.sx = preset.fStretchX;
    context.sy = preset.fStretchY;

    context.wave_r = preset.wave_r;
    context.wave_g = preset.wave_g;
    context.wave_b = preset.wave_b;
    context.wave_a = 1.0f;

    return true;
}

MilkDrop::ExecutionContext& RenderState::executeFrame(float deltaTime)
{
    if (!currentPreset)
        return context;

    // Update time
    totalTime += deltaTime;
    context.time = totalTime;
    context.frame = static_cast<double>(frameCount);

    // Execute per-frame init code (only once)
    if (!perFrameInitExecuted && !currentPreset->perFrameInitCode.empty())
    {
        perFrameInitEval->execute(context);
        perFrameInitExecuted = true;
    }

    // Execute per-frame code
    if (!currentPreset->perFrameCode.empty())
    {
        perFrameEval->execute(context);
    }

    // Increment frame counter
    frameCount++;

    return context;
}

void RenderState::updateAudioData(float bass, float mid, float treb,
                                  float bassAtt, float midAtt, float trebAtt)
{
    context.bass = bass;
    context.mid = mid;
    context.treb = treb;
    context.bass_att = bassAtt;
    context.mid_att = midAtt;
    context.treb_att = trebAtt;
}
