/** ****************************************************************************************************
 * @file
 * @brief ステージ操作関連
 **************************************************************************************************** */
#pragma once

#include <Siv3D.hpp>
#include "StageData.h"
#include "GimmickData.h"

namespace MySystem
{
	/** ****************************************************************************************************
	 * @brief ステージ操作関連
	 **************************************************************************************************** */
	class Stage
	{
	private:
		class Impl;
		std::unique_ptr<Impl> _pImpl;

	public:
		Stage() = delete;
		Stage(const Stage&) = delete;
		Stage& operator=(const Stage&) = delete;

		Stage(const Vec2& position, const Vec2& scale, const double angle, class Transform* pParent) noexcept;

		~Stage();

		void Update() noexcept;

		void Draw() const noexcept;

		void CreateStage() noexcept;

		void CreateStage(const StageData::WorldIndex worldIndex, const StageData::StageIndex stageIndex) noexcept;

		void SetGameClear();

		void SetGameOverPlayerScaleOut();

		void SetGameOverPlayerFallOut();

		bool LoadStageWait();

		bool LoadSelectWait();

		std::weak_ptr<GimmickData::Gimmick> CreatePlayer(const GimmickData::Desc& data, Transform* pParent);

		std::weak_ptr<GimmickData::Gimmick> CreateGoal(const GimmickData::Desc& data, Transform* pParent);
	};
}


