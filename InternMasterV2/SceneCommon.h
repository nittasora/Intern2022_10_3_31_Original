#pragma once

#include <Siv3D.hpp>
#include "StageData.h"

namespace MySystem
{
	struct CommonSceneData
	{
		static constexpr StageData::WorldIndex gMAX_WORLD_NUM = 1;
		static constexpr StageData::StageIndex gMAX_STAGE_NUM = 2;

		StageData::WorldIndex worldIndex;
		StageData::StageIndex stageIndex;

		inline CommonSceneData() :
			worldIndex{ static_cast<StageData::WorldIndex>(0) },
			stageIndex{ static_cast<StageData::StageIndex>(0) }
		{

		}
	};

	using App = SceneManager<String, CommonSceneData>;
}
