/** ****************************************************************************************************
 * @file
 * @brief ステージのデータ操作関連
 **************************************************************************************************** */
#pragma once

#include "StagePieceData.h"

/** ****************************************************************************************************
 * @brief ステージのデータ操作関連
 **************************************************************************************************** */
namespace MySystem::StageData
{
	using WorldIndex = uint8;	//!< ワールド数設定用
	using StageIndex = uint8;	//!< ステージ数設定用

	/** ****************************************************************************************************
	 * @brief ステージのデータ格納用
	 **************************************************************************************************** */
	struct Desc
	{
		String stageName;
		Array<StagePieceData::Desc> aPieceDescs;
	};

	bool Load(const WorldIndex worldIndex, const StageIndex stageIndex) noexcept;

	bool Save(const WorldIndex worldIndex, const StageIndex stageIndex) noexcept;

	std::weak_ptr<Desc> GetCurrentData();
}


