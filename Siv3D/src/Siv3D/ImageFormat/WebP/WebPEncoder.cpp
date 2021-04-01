﻿//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2021 Ryo Suzuki
//	Copyright (c) 2016-2021 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# include <Siv3D/ImageFormat/WebPEncoder.hpp>
# include <Siv3D/BinaryWriter.hpp>
# include <Siv3D/BlobWriter.hpp>
# include <Siv3D/Image.hpp>
# include <Siv3D/EngineLog.hpp>

# if SIV3D_PLATFORM(WINDOWS) | SIV3D_PLATFORM(MACOS)
#	include <ThirdParty-prebuilt/libwebp/decode.h>
#	include <ThirdParty-prebuilt/libwebp/encode.h>
# else
#	include <libwebp/decode.h>
#	include <libwebp/encode.h>
# endif

namespace s3d
{
	namespace detail
	{
		static int WebPWriteCallback(const uint8_t* data, size_t data_size, const WebPPicture* picture)
		{
			IWriter* pWriter = static_cast<IWriter*>(picture->custom_ptr);

			return (static_cast<int64>(data_size) == pWriter->write(data, data_size));
		}

		static void SetMethod(const WebPMethod method, WebPConfig& config) noexcept
		{
			switch (method)
			{
			case WebPMethod::Fast:
				config.method = 0;
				config.pass = 1;
				config.alpha_filtering = 1;
				break;
			case WebPMethod::Default:
				config.method = 4;
				config.pass = 1;
				config.alpha_filtering = 1;
				break;
			case WebPMethod::Quality:
				config.method = 5;
				config.pass = 2;
				config.alpha_filtering = 2;
				break;
			case WebPMethod::MaxQuality:
				config.method = 6;
				config.pass = 10;
				config.alpha_filtering = 2;
				break;
			}
		}
	}

	StringView WebPEncoder::name() const
	{
		return U"WebP"_sv;
	}

	const Array<String>& WebPEncoder::possibleExtensions() const
	{
		static const Array<String> extensions = { U"webp" };

		return extensions;
	}

	bool WebPEncoder::save(const Image& image, const FilePathView path) const
	{
		return save(image, path, Lossless::No, 90.0, WebPMethod::Default);
	}

	bool WebPEncoder::save(const Image& image, const FilePathView path, const Lossless lossless, const double quality, const WebPMethod method) const
	{
		BinaryWriter writer{ path };

		if (not writer)
		{
			return false;
		}

		return encode(image, writer);
	}

	bool WebPEncoder::encode(const Image& image, IWriter& writer) const
	{
		return encode(image, writer, Lossless::No, 90.0, WebPMethod::Default);
	}

	bool WebPEncoder::encode(const Image& image, IWriter& writer, const Lossless lossless, const double quality, const WebPMethod method) const
	{
		if (not writer.isOpen())
		{
			return false;
		}

		WebPPicture picture;
		WebPConfig config;
		WebPAuxStats stats;

		if (!::WebPPictureInit(&picture) || !::WebPConfigInit(&config))
		{
			return false;
		}

		picture.width = image.width();
		picture.height = image.height();

		if (lossless)
		{
			Image tmp(image);

			picture.argb = static_cast<uint32*>(static_cast<void*>(tmp.data()));
			picture.argb_stride = tmp.width();
			picture.use_argb = 1;
			config.lossless = 1;
			config.quality = Clamp(static_cast<float>(quality), 0.0f, 100.0f);
			config.thread_level = true;
			detail::SetMethod(method, config);

			::WebPPictureImportRGBA(&picture, tmp.dataAsUint8(), tmp.stride());

			if (!::WebPValidateConfig(&config))
			{
				return false;
			}

			picture.writer = detail::WebPWriteCallback;
			picture.custom_ptr = &writer;
			picture.stats = &stats;

			if (!::WebPEncode(&config, &picture))
			{
				::WebPPictureFree(&picture);
				return false;
			}

			::WebPPictureFree(&picture);
		}
		else
		{
			::WebPPictureImportRGBA(&picture, image.dataAsUint8(), image.stride());

			if (!::WebPConfigPreset(&config, WEBP_PRESET_DEFAULT, Clamp(static_cast<float>(quality), 0.0f, 100.0f)))
			{
				return false;
			}

			config.thread_level = true;
			detail::SetMethod(method, config);

			if (!::WebPValidateConfig(&config))
			{
				return false;
			}

			picture.writer = detail::WebPWriteCallback;
			picture.custom_ptr = &writer;
			picture.stats = &stats;

			if (!::WebPEncode(&config, &picture))
			{
				::WebPPictureFree(&picture);
				return false;
			}

			::WebPPictureFree(&picture);
		}

		return true;
	}

	Blob WebPEncoder::encode(const Image& image) const
	{
		return encode(image, Lossless::No, 90.0, WebPMethod::Default);
	}

	Blob WebPEncoder::encode(const Image& image, const Lossless lossless, const double quality, const WebPMethod method) const
	{
		BlobWriter writer;

		if (not encode(image, writer, lossless, quality, method))
		{
			return{};
		}

		return writer.retrieve();
	}
}
