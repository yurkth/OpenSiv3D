//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2021 Ryo Suzuki
//	Copyright (c) 2016-2021 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# pragma once
# include <Siv3D/Common.hpp>
# include <Siv3D/Common/OpenGL.hpp>
# include <Siv3D/Renderer/IRenderer.hpp>
# include <Siv3D/PointVector.hpp>
# include <Siv3D/Renderer/GLES3/BackBuffer/GLES3BackBuffer.hpp>
# include <Siv3D/Renderer/GLES3/BlendState/GLES3BlendState.hpp>
# include <Siv3D/Renderer/GLES3/RasterizerState/GLES3RasterizerState.hpp>
# include <Siv3D/Renderer/GLES3/SamplerState/GLES3SamplerState.hpp>

namespace s3d
{
	class CRenderer_GLES3 final : public ISiv3DRenderer
	{
	private:
		
		CRenderer2D_GLES3* pRenderer2D = nullptr;
		GLFWwindow* m_window = nullptr;

		std::unique_ptr<GLES3BackBuffer> m_backBuffer;

		std::unique_ptr<GLES3BlendState> m_blendState;

		std::unique_ptr<GLES3RasterizerState> m_rasterizerState;
		
		std::unique_ptr<GLES3SamplerState> m_samplerState;

	public:

		CRenderer_GLES3();

		~CRenderer_GLES3() override;

		EngineOption::Renderer getRendererType() const noexcept override;

		void init() override;

		StringView getName() const override;

		void clear() override;

		void flush() override;

		bool present() override;

		void setSceneResizeMode(ResizeMode resizeMode) override;

		ResizeMode getSceneResizeMode() const noexcept override;

		void setSceneBufferSize(Size size) override;

		Size getSceneBufferSize() const noexcept override;

		void setSceneTextureFilter(TextureFilter textureFilter) override;

		TextureFilter getSceneTextureFilter() const noexcept override;

		void setBackgroundColor(const ColorF& color) override;

		const ColorF& getBackgroundColor() const noexcept override;

		void setLetterboxColor(const ColorF& color) override;

		const ColorF& getLetterboxColor() const noexcept override;

		std::pair<float, RectF> getLetterboxComposition() const noexcept override;

		//
		// GLES3
		//

		GLES3BlendState& getBlendState() noexcept;

		GLES3RasterizerState& getRasterizerState() noexcept;

		GLES3SamplerState& getSamplerState() noexcept;
	};
}