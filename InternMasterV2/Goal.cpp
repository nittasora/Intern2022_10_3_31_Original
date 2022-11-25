#include "Goal.h"
#include "Transform.h"
#include "TileData.h"
#include "BitFlag.h"

namespace MySystem
{
	class Goal::Impl
	{
	public:
		Transform transform;
		GimmickData::TypeIndex	type;
		GimmickData::Flag		flag;

		Impl(const GimmickData::Desc& data, Transform* pParent) :
			transform{ data.position, Vec2{ 1.0, 1.0 }, data.angle, pParent },
			type{ data.gimmickType },
			flag{ BitFlag::GetMask(GimmickData::gENABLE, GimmickData::gACTIVE) }
		{

		}

		void Update()
		{

		}

		void Draw() const
		{
			if (BitFlag::CheckAllUnPop(flag, GimmickData::gENABLE))
			{
				return;
			}

			GetBody()(TextureAsset(GimmickData::gLABEL_TEXTURE[type])).draw();
		}

		Transform::Scale GetBodyScale() const
		{
			return transform.GetWorlScale() * TileData::gTILE_SIZE;
		}

		Quad GetBody() const
		{
			auto position = transform.GetWorlPosition();
			auto scale = GetBodyScale();
			auto angle = transform.GetWorlAngle();

			return
				RectF{ Arg::center = position, scale }.
				rotatedAt(position, angle);
		}

		Circle GetCollision() const
		{
			auto scale = GetBodyScale() / 2.0;
			return Circle{ transform.GetWorlPosition(), Max(scale.x, scale.y) };
		}
	};

	Goal::Goal(const GimmickData::Desc& data, Transform* pParent) :
		Gimmick{},
		_pImpl{ new Impl{ data, pParent } }
	{

	}

	Goal::~Goal()
	{

	}

	void Goal::Update()
	{
		_pImpl->Update();
	}

	void Goal::Draw() const
	{
		_pImpl->Draw();
	}

	Circle Goal::GetCollision() const
	{
		return _pImpl->GetCollision();
	}

	void Goal::SetIsActive(const bool isActive)
	{
		BitFlag::PopOrUnPop(_pImpl->flag, GimmickData::gACTIVE, isActive);
	}

	bool Goal::GetIsActive()
	{
		return BitFlag::CheckAnyPop(_pImpl->flag, GimmickData::gACTIVE);
	}

	void Goal::SetParent(Transform* pParent)
	{
		_pImpl->transform.SetParent(pParent);
	}

	GimmickData::Type Goal::GetType()
	{
		return static_cast<GimmickData::Type>(_pImpl->type);
	}

	void Goal::SetDestroy()
	{
		BitFlag::UnPop(_pImpl->flag, GimmickData::gENABLE);
	}

	bool Goal::GetEnable()
	{
		return BitFlag::CheckAnyPop(_pImpl->flag, GimmickData::gENABLE);
	}
}
