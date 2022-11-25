/** ****************************************************************************************************
 * @file
 * @brief タイル操作関連
 **************************************************************************************************** */
#pragma once

#include "TileData.h"

/** ****************************************************************************************************
 * @brief タイル操作関連
 **************************************************************************************************** */
namespace MySystem
{
	class Tile
	{
	private:
		class Impl;
		std::unique_ptr<Impl> _pImpl;

	public:
		Tile() = delete;
		Tile(const Tile&) = delete;
		Tile& operator=(const Tile&) = delete;

		Tile(const TileData::Desc& data, class Transform* pParent) noexcept;

		~Tile();

		void Update() noexcept;

		void Draw() const noexcept;

		Polygon GetWorldHitBox() const;

		Polygon GetLocalHitBox() const;
	};
}

