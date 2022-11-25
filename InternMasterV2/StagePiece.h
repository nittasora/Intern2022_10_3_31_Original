/** ****************************************************************************************************
 * @file
 * @brief ステージのカケラ操作関連
 **************************************************************************************************** */
#pragma once

#include <Siv3D.hpp>
#include "StagePieceData.h"

namespace MySystem
{
	/** ****************************************************************************************************
	 * @brief ステージのカケラ操作関連
	 **************************************************************************************************** */
	class StagePiece
	{
	private:
		class Impl;
		std::unique_ptr<Impl> _pImpl;

	public:
		StagePiece() = delete;
		StagePiece(const StagePiece&) = delete;
		StagePiece& operator=(const StagePiece&) = delete;

		StagePiece(const StagePieceData::Desc& data, const uint8 pieceNumber, class Stage* pStage, class Transform* pParent) noexcept;

		~StagePiece();

		void Update() noexcept;

		void Draw() const noexcept;

		bool UpdatePiece() noexcept;

		void SetActive(const bool isActive) noexcept;

		Array<Polygon> GetLocalHitBoxes() const;

		Array<Polygon> GetWorldHitBoxes() const;

		Quad GetLocalBody() const;

		Quad GetWorldBody() const;

		StagePieceData::Type GetType() const;

		Array<std::weak_ptr<GimmickData::Gimmick>> GetGimmicks() const;

		void AddGimmick(std::weak_ptr<GimmickData::Gimmick> pGimmick);

		void DeleteActiveGimmicks();

		double GetAngle();
	};
}
