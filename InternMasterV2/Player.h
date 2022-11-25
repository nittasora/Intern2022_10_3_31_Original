/** ****************************************************************************************************
 * @file
 * @brief プレイヤー操作関連
 **************************************************************************************************** */
#pragma once

#include <Siv3D.hpp>
#include "GimmickData.h"

namespace MySystem
{
	/** ****************************************************************************************************
	 * @brief プレイヤー操作関連
	 **************************************************************************************************** */
	class Player : public GimmickData::Gimmick
	{
	private:
		class Impl;
		std::unique_ptr<Impl> _pImpl;

	public:
		Player() = delete;
		Player(const Player&) = delete;
		Player& operator=(const Player&) = delete;

		Player(const GimmickData::Desc& data, class Transform* pParent);
		~Player();

		void Update() override;
		void Draw() const override;

		Circle GetCollision() const override;

		void SetIsActive(const bool isActive) override;

		bool GetIsActive() override;

		void UpdateDirection();

		void UpdateDirection(const double angle);

		void HitCheck(const Array<LineString>& aGroundLines, const Array<std::weak_ptr<GimmickData::Gimmick>>& apGimmicks);

		void SetParent(Transform* pParent) override;

		GimmickData::Type GetType() override;

		void SetDestroy() override;

		bool GetEnable() override;
	};
}

