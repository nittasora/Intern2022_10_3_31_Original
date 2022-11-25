/** ****************************************************************************************************
 * @file
 * @brief ギミックのデータ操作関連
 **************************************************************************************************** */
#pragma once

#include <Siv3D.hpp>
#include "Transform.h"

/** ****************************************************************************************************
 * @brief ギミックのデータ操作関連
 **************************************************************************************************** */
namespace MySystem::GimmickData
{
	using TypeIndex = uint8;	//!< タイルの種類用
	using Flag = uint8;

	static constexpr Flag gENABLE = static_cast<Flag>(1 << 0);	//!< 存在しているか
	static constexpr Flag gACTIVE = static_cast<Flag>(1 << 1);	//!< 動けるかどうか

	/** ****************************************************************************************************
	 * @brief ギミックの種類
	 **************************************************************************************************** */
	enum class Type : TypeIndex
	{
		Player,	//!< 矩形タイル
		Goal,	//!< 三角形タイル
		Max
	};

	static const std::array<String, static_cast<TypeIndex>(Type::Max) + 1> gLABEL_SIMPLE_TEXTURE = {
		U"PlayerSimpleTexture",
		U"GoalSimpleTexture",
		U"NullSimpleTexture"
	};

	static const std::array<String, static_cast<TypeIndex>(Type::Max) + 1> gLABEL_TEXTURE = {
		U"PlayerTexture",
		U"GoalTexture",
		U"NullTexture"
	};

	/** ****************************************************************************************************
	 * @brief ギミックのデータ格納用
	 **************************************************************************************************** */
	struct Desc
	{
		Vec2 position;	//!< 表示座標
		double angle;	//!< 表示角度

		TypeIndex gimmickType;	//!< ギミックの種類

		inline Desc() noexcept :
			position{ Vec2{ 0.0, 0.0 } },
			angle{ 0.0 },
			gimmickType{ static_cast<TypeIndex>(0) }
		{

		}
	};

	class Gimmick
	{
	public:
		Gimmick();
		virtual ~Gimmick();

		virtual void Update();
		virtual void Draw() const;

		virtual Circle GetCollision() const = 0;

		virtual void SetIsActive(const bool isActive) = 0;

		virtual bool GetIsActive() = 0;

		virtual void SetParent(Transform* pParent) = 0;

		virtual Type GetType() = 0;

		virtual void SetDestroy() = 0;

		virtual bool GetEnable() = 0;
	};
}
