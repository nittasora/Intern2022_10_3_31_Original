﻿/** ****************************************************************************************************
 * @file
 * @brief ゴール操作関連
 **************************************************************************************************** */
#pragma once

#include <Siv3D.hpp>
#include "GimmickData.h"

namespace MySystem
{
	/** ****************************************************************************************************
	 * @brief ゴール操作関連
	 **************************************************************************************************** */
	class Goal : public GimmickData::Gimmick
	{
	private:
		class Impl;
		std::unique_ptr<Impl> _pImpl;

	public:
		Goal() = delete;
		Goal(const Goal&) = delete;
		Goal& operator=(const Goal&) = delete;

		Goal(const GimmickData::Desc& data, class Transform* pParent);
		~Goal();

		void Update() override;
		void Draw() const override;

		Circle GetCollision() const override;

		void SetIsActive(const bool isActive) override;

		bool GetIsActive() override;

		void SetParent(Transform* pParent) override;

		GimmickData::Type GetType() override;

		void SetDestroy() override;

		bool GetEnable() override;
	};
}

