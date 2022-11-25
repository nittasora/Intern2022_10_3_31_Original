/** ****************************************************************************************************
 * @file
 * @brief ステージのカケラのデータ操作関連
 **************************************************************************************************** */
#include "StagePieceData.h"

namespace MySystem::StagePieceData
{
	Desc::Desc() noexcept :
		position{ 0.0, 0.0 },
		scale{ 1.0 },
		angle{ 0.0 },
		backScale{ 0.0, 0.0 },
		canMoving{ false },
		canScaling{ false },
		canRolling{ false },
		backType{ static_cast<TypeIndex>(0) }
	{

	}
}
