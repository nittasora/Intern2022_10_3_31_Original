/** ****************************************************************************************************
 * @file
 * @brief タイルのデータ操作関連
 **************************************************************************************************** */
#pragma once

#include <Siv3D.hpp>

 /** ****************************************************************************************************
  * @brief タイルのデータ操作関連
  **************************************************************************************************** */
namespace MySystem::TileData
{
	using TypeIndex = uint8;	//!< タイルの種類用

	/** ****************************************************************************************************
	 * @brief タイルの種類
	 **************************************************************************************************** */
	enum class Type : TypeIndex
	{
		Rect,		//!< 矩形タイル
		Triangle,	//!< 三角形タイル
		Max
	};

	static constexpr double	gTILE_SIZE = 30.0;		//!< タイルの大きさ
	static constexpr Point	gUV_CELL_NUM{ 16, 32 }; //!< タイルテクスチャの分割数

	/** ****************************************************************************************************
	 * @brief タイルのデータ格納用
	 **************************************************************************************************** */
	struct Desc
	{
		Vec2 position;	//!< 表示座標
		double angle;	//!< 表示角度
		Point uv;		//!< UV開始位置

		TypeIndex tileType;	//!< タイルの種類

		inline Desc() noexcept :
			position{ Vec2{ 0.0, 0.0 } },
			angle{ 0.0 },
			uv{ Point{ 0, 0 } },
			tileType{ static_cast<TypeIndex>(0) }
		{

		}
	};

	void GetSimplePolygon(Array<Polygon>& targets, double nearPointDistance, double nearLineDistance);

	Array<Vec2> GetSimpleVertex(Array<Vec2> targets, double nearPointDistance, double nearLineDistance);
}

