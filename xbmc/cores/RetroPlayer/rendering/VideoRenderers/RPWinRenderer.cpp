/*
 *      Copyright (C) 2017 Team Kodi
 *      http://kodi.tv
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this Program; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "RPWinRenderer.h"
#include "cores/RetroPlayer/rendering/RenderContext.h"
#include "cores/RetroPlayer/rendering/RenderTranslator.h"
#include "cores/RetroPlayer/rendering/RenderVideoSettings.h"
#include "cores/RetroPlayer/rendering/VideoShaders/windows/RPWinOutputShader.h"
#include "cores/RetroPlayer/rendering/VideoShaders/windows/VideoShaderPresetDX.h"
#include "cores/RetroPlayer/rendering/VideoShaders/windows/VideoShaderTextureDX.h"
#include "guilib/D3DResource.h"
#include "rendering/dx/RenderSystemDX.h"
#include "utils/log.h"

extern "C"
{
#include "libswscale/swscale.h"
}

#include <cstring>

using namespace KODI;
using namespace RETRO;

// --- CWinRendererFactory -----------------------------------------------------

std::string CWinRendererFactory::RenderSystemName() const
{
  return "DirectX";
}

CRPBaseRenderer *CWinRendererFactory::CreateRenderer(const CRenderSettings &settings, CRenderContext &context, std::shared_ptr<IRenderBufferPool> bufferPool)
{
  return new CRPWinRenderer(settings, context, std::move(bufferPool));
}

RenderBufferPoolVector CWinRendererFactory::CreateBufferPools(CRenderContext &context)
{
  return { std::make_shared<CWinRenderBufferPool>() };
}

// --- CWinRenderBuffer --------------------------------------------------------

CWinRenderBuffer::CWinRenderBuffer(AVPixelFormat pixFormat, DXGI_FORMAT dxFormat) :
  m_pixFormat(pixFormat),
  m_targetDxFormat(dxFormat),
  m_targetPixFormat(GetPixFormat(dxFormat))
{
}

bool CWinRenderBuffer::CreateTexture()
{
  if (!m_intermediateTarget->GetPointer()->Create(m_width, m_height, 1, D3D11_USAGE_DYNAMIC, m_targetDxFormat))
  {
    CLog::Log(LOGERROR, "WinRenderer: Intermediate render target creation failed");
    return false;
  }

  return true;
}

bool CWinRenderBuffer::GetTexture(uint8_t*& data, unsigned int& stride)
{
  // Scale and upload texture
  D3D11_MAPPED_SUBRESOURCE destlr;
  if (!m_intermediateTarget->GetPointer()->LockRect(0, &destlr, D3D11_MAP_WRITE_DISCARD))
  {
    CLog::Log(LOGERROR, "WinRenderer: Failed to lock swtarget texture into memory");
    return false;
  }

  data = static_cast<uint8_t*>(destlr.pData);
  stride = destlr.RowPitch;

  return true;
}

bool CWinRenderBuffer::ReleaseTexture()
{
  if (!m_intermediateTarget->GetPointer()->UnlockRect(0))
  {
    CLog::Log(LOGERROR, "WinRenderer: Failed to unlock swtarget texture");
    return false;
  }

  return true;
}

bool CWinRenderBuffer::UploadTexture()
{
  if (m_targetDxFormat == DXGI_FORMAT_UNKNOWN)
  {
    CLog::Log(LOGERROR, "WinRenderer: Invalid DX texture format");
    return false;
  }

  if (!CreateScalingContext())
    return false;

  // Create intermediate texture
  if (!m_intermediateTarget)
  {
    m_intermediateTarget.reset(new SHADER::CShaderTextureCD3D(new CD3DTexture));
    if (!CreateTexture())
    {
      m_intermediateTarget.reset();
      return false;
    }
  }

  uint8_t *destData = nullptr;
  unsigned int destStride = 0;
  if (!GetTexture(destData, destStride))
    return false;

  const unsigned int sourceStride = static_cast<unsigned int>(m_data.size() / m_height);
  ScalePixels(m_data.data(), sourceStride, destData, destStride);

  if (!ReleaseTexture())
    return false;

  return true;
}

bool CWinRenderBuffer::CreateScalingContext()
{
  if (m_swsContext == nullptr)
  {
    m_swsContext = sws_getContext(m_width, m_height, m_pixFormat, m_width, m_height, m_targetPixFormat,
      SWS_FAST_BILINEAR, NULL, NULL, NULL);

    if (m_swsContext == nullptr)
    {
      CLog::Log(LOGERROR, "WinRenderer: Failed to create swscale context");
      return false;
    }
  }

  return true;
}

void CWinRenderBuffer::ScalePixels(uint8_t *source, unsigned int sourceStride, uint8_t *target, unsigned int targetStride)
{
  uint8_t* src[] =       { source,                          nullptr,   nullptr,   nullptr };
  int      srcStride[] = { static_cast<int>(sourceStride),  0,         0,         0       };
  uint8_t* dst[] =       { target,                          nullptr,   nullptr,   nullptr };
  int      dstStride[] = { static_cast<int>(targetStride),  0,         0,         0       };

  sws_scale(m_swsContext, src, srcStride, 0, m_height, dst, dstStride);
}

AVPixelFormat CWinRenderBuffer::GetPixFormat(DXGI_FORMAT dxFormat)
{
  return AV_PIX_FMT_BGRA; //! @todo
}

// --- CWinRenderBufferPool ----------------------------------------------------

CWinRenderBufferPool::CWinRenderBufferPool()
{
  CompileOutputShaders();
}

bool CWinRenderBufferPool::IsCompatible(const CRenderVideoSettings &renderSettings) const
{
  //! @todo Move this logic to generic class

  // Shader presets are compatible
  if (!renderSettings.GetShaderPreset().empty())
    return true;

  // If no shader preset is specified, scaling methods must match
  return GetShader(renderSettings.GetScalingMethod()) != nullptr;
}

IRenderBuffer *CWinRenderBufferPool::CreateRenderBuffer(void *header /* = nullptr */)
{
  return new CWinRenderBuffer(m_format, m_targetDxFormat);
}

bool CWinRenderBufferPool::ConfigureDX(DXGI_FORMAT dxFormat)
{
  if (m_targetDxFormat != DXGI_FORMAT_UNKNOWN)
    return false; // Already configured

  m_targetDxFormat = dxFormat;

  return true;
}

CRPWinOutputShader *CWinRenderBufferPool::GetShader(SCALINGMETHOD scalingMethod) const
{
  auto it = m_outputShaders.find(scalingMethod);

  if (it != m_outputShaders.end())
    return it->second.get();

  return nullptr;
}

const std::vector<SCALINGMETHOD> &CWinRenderBufferPool::GetScalingMethods()
{
  static std::vector<SCALINGMETHOD> scalingMethods = {
    SCALINGMETHOD::NEAREST,
    SCALINGMETHOD::LINEAR,
  };

  return scalingMethods;
}

void CWinRenderBufferPool::CompileOutputShaders()
{
  for (auto scalingMethod : GetScalingMethods())
  {
    std::unique_ptr<CRPWinOutputShader> outputShader(new CRPWinOutputShader);
    if (outputShader->Create(scalingMethod))
      m_outputShaders[scalingMethod] = std::move(outputShader);
    else
      CLog::Log(LOGERROR, "RPWinRenderer: Unable to create output shader (%s)",
        CRenderTranslator::TranslateScalingMethod(scalingMethod));
  }
}

// --- CRPWinRenderer ----------------------------------------------------------

CRPWinRenderer::CRPWinRenderer(const CRenderSettings &renderSettings, CRenderContext &context, std::shared_ptr<IRenderBufferPool> bufferPool) :
  CRPBaseRenderer(renderSettings, context, std::move(bufferPool))
{
  // Initialize CRPBaseRenderer fields
  m_shaderPreset.reset(new SHADER::CVideoShaderPresetDX(m_context));

  // Compile the output shaders
  // TODO: Should we do that if shader presets are enabled?
  SCALINGMETHOD defaultScalingMethod = renderSettings.VideoSettings().GetScalingMethod();
  CompileOutputShaders(defaultScalingMethod);
}

CRPWinRenderer::~CRPWinRenderer()
{
  Deinitialize();
}

void CRPWinRenderer::CompileOutputShaders(SCALINGMETHOD defaultScalingMethod)
{
  auto compileAndRegisterShader = [this] (SCALINGMETHOD scalingMethod, const char* scalingMethodString)
  {
    CRPWinOutputShader* outputShader = new CRPWinOutputShader;
    if (!outputShader->Create(scalingMethod))
      CLog::Log(LOGERROR, "RPWinRenderer: Unable to create output shader (%s)", scalingMethodString);
    m_outputShaders[scalingMethod].reset(outputShader);
  };

  // compile with linear scaling
  compileAndRegisterShader(SCALINGMETHOD::LINEAR, "linear");

  // compile with nearest neighbor scaling
  compileAndRegisterShader(SCALINGMETHOD::NEAREST, "nearest");

  // Fill all remaining keys

  // Use `defaultScalingMethod` if our output shader implements it
  // if not, default to nearest neighbor scaling
  std::shared_ptr<CRPWinOutputShader> defaultScalingShader;
  if (m_outputShaders.count(defaultScalingMethod) != 0)
    defaultScalingShader = m_outputShaders[defaultScalingMethod];
  else
    defaultScalingShader = m_outputShaders[SCALINGMETHOD::NEAREST];

  for (int s = static_cast<int>(SCALINGMETHOD::NEAREST); s != static_cast<int>(SCALINGMETHOD::MAX); ++s)
  {
    auto scalingMethod = static_cast<SCALINGMETHOD>(s);

    if (m_outputShaders.count(scalingMethod) == 0)
      m_outputShaders[scalingMethod] = defaultScalingShader;
  }
}

bool CRPWinRenderer::ConfigureInternal()
{
  CRenderSystemDX *renderingDx = static_cast<CRenderSystemDX*>(m_context.Rendering());

  DXGI_FORMAT targetDxFormat = renderingDx->GetBackBuffer()->GetFormat();

  static_cast<CWinRenderBufferPool*>(m_bufferPool.get())->ConfigureDX(targetDxFormat);

  return true;
}

void CRPWinRenderer::RenderInternal(bool clear, uint8_t alpha)
{
  CRenderSystemDX *renderingDx = static_cast<CRenderSystemDX*>(m_context.Rendering());

  // Set alpha blend state
  renderingDx->SetAlphaBlendEnable(alpha < 0xFF);

  Render(renderingDx->GetBackBuffer());
}

bool CRPWinRenderer::Supports(RENDERFEATURE feature) const
{
  if (feature == RENDERFEATURE::STRETCH ||
      feature == RENDERFEATURE::ZOOM ||
      feature == RENDERFEATURE::PIXEL_RATIO ||
      feature == RENDERFEATURE::ROTATION)
    return true;

  return false;
}

bool CRPWinRenderer::Supports(SCALINGMETHOD method) const
{
  return SupportsScalingMethod(method);
}

bool CRPWinRenderer::SupportsScalingMethod(SCALINGMETHOD method)
{
  if (method == SCALINGMETHOD::LINEAR ||
      method == SCALINGMETHOD::NEAREST)
    return true;

  return false;
}

void CRPWinRenderer::Render(CD3DTexture *target)
{
  CWinRenderBuffer *renderBuffer = static_cast<CWinRenderBuffer*>(m_renderBuffer);
  if (renderBuffer == nullptr)
    return;

  SHADER::CShaderTextureCD3D *renderBufferTarget = renderBuffer->GetTarget();
  if (renderBufferTarget == nullptr)
    return;

  const CPoint destPoints[4] = {
    m_rotatedDestCoords[0],
    m_rotatedDestCoords[1],
    m_rotatedDestCoords[2],
    m_rotatedDestCoords[3]
  };

  // Are we using video shaders?
  if (m_bUseShaderPreset)
  {
    CD3DTexture *intermediateTarget = renderBufferTarget->GetPointer();

    // Render shaders and ouput to display
    m_targetTexture.SetTexture(target);
    if (!m_shaderPreset->RenderUpdate(destPoints, renderBufferTarget, &m_targetTexture))
    {
      m_shadersNeedUpdate = false;
      m_bUseShaderPreset = false;
    }
  }
  else // Not using video shaders, output using output shader
  {
    CD3DTexture *intermediateTarget = renderBufferTarget->GetPointer();

    if (intermediateTarget != nullptr)
    {
      CRect viewPort;
      m_context.GetViewPort(viewPort);

      // Pick appropriate output shader depending on the scaling method of the renderer
      SCALINGMETHOD scalingMethod = m_renderSettings.VideoSettings().GetScalingMethod();
      CRPWinOutputShader* outputShader = m_outputShaders[scalingMethod].get();

      // Use the picked output shader to render to the target
      if (outputShader != nullptr)
      {
        outputShader->Render(*intermediateTarget,
          m_sourceRect, destPoints, viewPort, target,
          m_context.UseLimitedColor() ? 1 : 0);
      }
    }
  }
}
