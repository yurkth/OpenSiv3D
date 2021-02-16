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

# pragma once
# include "Common.hpp"
# include "Math.hpp"

namespace s3d
{
	struct NavMeshConfig
	{
		double cellSize	= 1.0;

		double cellHeight = 0.2;

		double agentMaxSlope = Math::ToRadians(45);

		double agentHeight = 2.0;

		double agentMaxClimb = 0.9;

		double agentRadius = 0.25;

		[[nodiscard]]
		static constexpr NavMeshConfig Default() noexcept
		{
			return NavMeshConfig{};
		}
	};
}