#include "Player.h"
#include "Transform.h"
#include "TileData.h"
#include "BitFlag.h"
#include "CommonKey.h"
#include "HitCheck.h"

namespace MySystem
{
	class Player::Impl
	{
	private:
		static constexpr Point _OFFSET_UV_LEFT_TOP{ 4, 8 };		//!< UVのオフセット値
		static constexpr Point _SIZE_UV_WIDTH_HEIGHT{ 13, 19 };	//!< 1コマのサイズ
		static constexpr Point _UV_CELL_NUM{ 6, 4 };			//!< コマの数

		static constexpr GimmickData::Flag _IS_GROUND = BitFlag::Get<GimmickData::Flag>(2);	//!< 着地フラグ

		static constexpr double _MAX_SPEED = 3.0;					//!< 横方向最大速度 
		static constexpr double _INCREMENT_SPEED = _MAX_SPEED;		//!< 横方向速度増加量
		static constexpr double _DECREMENT_SPEED = _MAX_SPEED * 2;	//!< 横方向速度減少量
		static constexpr double _GRAVITY = 9.8;						//!< 重力
		static constexpr double _JUMP_POWER = -3.0;					//!< ジャンプ時の速度

		//! 表示サイズ
		static constexpr Vec2 _BODEY_SIZE{ TileData::gTILE_SIZE, TileData::gTILE_SIZE * (static_cast<double>(_SIZE_UV_WIDTH_HEIGHT.y) / static_cast<double>(_SIZE_UV_WIDTH_HEIGHT.x)) };	
		static constexpr double _CIRCLE_RADIUS = Max(_BODEY_SIZE.x, _BODEY_SIZE.y);	//!< 当たり判定のサイズ

	public:
		Transform transform;
		GimmickData::TypeIndex	type;
		GimmickData::Flag		flag;

		Vec2	down;
		Vec2	right;
		Vec2	speed;

		int	animationFrame;

		Impl(const GimmickData::Desc& data, Transform* pParent) :
			transform{ data.position, Vec2{ 1.0, 1.0 }, data.angle, pParent },
			type{ data.gimmickType },
			flag{ BitFlag::GetMask(GimmickData::gENABLE, GimmickData::gACTIVE) }
		{
			auto bodyScale = GetBodyScale() - Vec2{ TileData::gTILE_SIZE, TileData::gTILE_SIZE };

			transform.SetLocalPosition(transform.GetLocalPosition() - bodyScale);
		}

		void HitCheck(const Array<LineString>& aGroundLines, const Array<std::weak_ptr<GimmickData::Gimmick>>& apGimmicks)
		{
			if (speed == Vec2::Zero())
			{
				BitFlag::Pop(flag, _IS_GROUND);
				return;
			}

			bool isReverse = false;
			Line* pPrevHitLine = nullptr;
			Vec2 prevNormal;
			auto collision = GetCollision();

			auto currentSpeed = speed.y * down + speed.x * right;

			BitFlag::UnPop(flag, _IS_GROUND);

			for (auto i : step(10))
			{
				auto speedRight = Vec2{ -currentSpeed.y, currentSpeed.x };
				double maxDistance = currentSpeed.length();
				Vec2 HitPosition;
				Vec2 hitNormal;
				Vec2 nextSpeed = currentSpeed;
				bool hitFlag = false;
				Line* pCurrentHitLine = nullptr;

				for (auto lineString : aGroundLines)
				{
					for (size_t j = 0, max = lineString.num_lines(CloseRing::Yes); j < max; j++)
					{
						auto line = lineString.line(j, CloseRing::Yes);

						auto normal = lineString.normalAtLine(j, CloseRing::Yes);
						auto speedVec = Line{ (-normal * collision.r) + collision.center, (-normal * collision.r) + collision.center + currentSpeed };

						if (speedRight.cross(normal) < 0.0)
						{
							continue;
						}

						if (not HitCheck(line.stretched(collision.r, collision.r), speedVec, HitPosition, maxDistance))
						{
							continue;
						}

						pCurrentHitLine = &line;

						nextSpeed = HitPosition;
						hitNormal = normal;
						hitFlag = true;
					}
				}

				for (auto pGimmick : apGimmicks)
				{
					auto pRawGimmick = pGimmick.lock();
					if (not HitCheck(collision, pRawGimmick.get(), currentSpeed, maxDistance))
					{
						continue;
					}

					switch (pRawGimmick->GetType())
					{
					case GimmickData::Type::Goal:
						pRawGimmick->SetDestroy();
						break;
					}
				}

				transform.SetWorlPosition(transform.GetWorlPosition() + nextSpeed);

				if (not hitFlag)
				{
					break;
				}

				currentSpeed = HitCheck::calcWallScratchVector(currentSpeed - nextSpeed, -hitNormal);

				{
					auto theta = acos(down.dot(hitNormal));

					if (theta > 110.0_deg)
					{
						BitFlag::Pop(flag, _IS_GROUND);
						speed.y = 0.0;
					}
					else if (theta > 70.0_deg)
					{
						speed.x = 0.0;
					}
					else
					{
						speed.y = 0.0;
					}
				}

				if (currentSpeed == Vec2::Zero())
				{
					break;
				}

				if (pCurrentHitLine)
				{
					if (pPrevHitLine)
					{
						auto theta = acos(prevNormal.dot(hitNormal));

						if (theta > 90.0_deg)
						{
							if (!isReverse)
							{
								isReverse = true;
								currentSpeed *= -1.0;
							}
						}
						else
						{
							isReverse = false;
						}
					}

					pPrevHitLine = pCurrentHitLine;
					prevNormal = hitNormal;
				}
			}
		}

		bool HitCheck(const Circle& collision, GimmickData::Gimmick* pGimmick, const Vec2& currentSpeed, double& maxDistance)
		{
			auto gimmickCollision = pGimmick->GetCollision();
			Vec2 a{ collision.center };
			Vec2 b{ a + currentSpeed };
			Vec2 p{ gimmickCollision.center };
			Vec2 ap = p - a;
			Vec2 ab = b - a;
			Vec2 ba = a - b;
			Vec2 bp = p - b;
			Vec2 neighbor_point;
			double distance;

			if (ap.dot(ab) < 0)
			{
				distance = ap.length();
				neighbor_point = a;
			}
			else if (bp.dot(ba) < 0)
			{
				distance = bp.length();
				neighbor_point = b;
			}
			else
			{
				auto ai_norm = ap.dot(ab) / ab.length();
				neighbor_point = a + ab / ab.length() * ai_norm;
				distance = (p - neighbor_point).length();
			}

			if (distance < collision.r + gimmickCollision.r)
			{
				auto nextDistance = (neighbor_point - a).length();

				if (nextDistance <= maxDistance)
				{
					return true;
				}
			}

			return false;
		}

		bool HitCheck(const Line& groundLine, const Line& speedVec, Vec2& hitPosition, double& maxDistance)
		{
			double tempDistance = maxDistance;
			double hitTime;
			Vec2 point;

			if (HitCheck::HitCheckSegmentToSegment(speedVec.begin, speedVec.end, groundLine.begin, groundLine.end, hitTime, point))
			{
				float nextDistance = (point - speedVec.begin).length();

				if (nextDistance >= maxDistance)
					return false;

				hitPosition = point - speedVec.begin;
				maxDistance = nextDistance;

				return true;
			}

			return false;
		}

		void IncrementSpeed(double incrementSpeed)
		{
			speed.x += incrementSpeed * Scene::DeltaTime();

			speed.x = Clamp(speed.x , -_MAX_SPEED, _MAX_SPEED);
		}

		void DecrementSpeed()
		{
			if (speed.x > 0.0)
			{
				speed.x -= _DECREMENT_SPEED * Scene::DeltaTime();

				speed.x = Max(speed.x, 0.0);
			}
			else if (speed.x < 0.0)
			{
				speed.x += _DECREMENT_SPEED * Scene::DeltaTime();

				speed.x = Min(speed.x, 0.0);
			}
		}

		void Update()
		{
			if (BitFlag::CheckAnyUnPop(flag, BitFlag::GetMask(GimmickData::gENABLE, GimmickData::gACTIVE)))
			{
				return;
			}

			if (CommonKeyState::gINPUT_LEFT.pressed())
			{
				IncrementSpeed(-_INCREMENT_SPEED);
			}
			else if (CommonKeyState::gINPUT_RIGHT.pressed())
			{
				IncrementSpeed(_INCREMENT_SPEED);
			}
			else
			{
				DecrementSpeed();
			}

			if (BitFlag::CheckAnyPop(flag, _IS_GROUND))
			{
				if (CommonKeyState::gINPUT_JUMP.pressed())
				{
					speed.y = _JUMP_POWER;
				}
			}
			else
			{
				speed.y += _GRAVITY * Scene::DeltaTime();
			}

			if (speed.x != 0.0f)
			{
				animationFrame = (speed.x < 0 ? 6 : 12) + (int)(Periodic::Triangle0_1(0.5s) * 2.0 + 0.5);
			}
		}

		void Draw() const
		{
			if (BitFlag::CheckAllUnPop(flag, GimmickData::gENABLE))
			{
				return;
			}

			auto texture = TextureAsset{ GimmickData::gLABEL_TEXTURE[type] };

			auto orizinSize = texture.size() / _UV_CELL_NUM;

			const int offsetU = (animationFrame % 6) * orizinSize.x + _OFFSET_UV_LEFT_TOP.x;
			const int offsetV = (animationFrame / 6) * orizinSize.y + _OFFSET_UV_LEFT_TOP.y;

			GetBody()(texture(offsetU, offsetV, _SIZE_UV_WIDTH_HEIGHT)).draw();
		}

		Transform::Scale GetBodyScale() const
		{
			return transform.GetWorlScale() * _BODEY_SIZE;
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

	Player::Player(const GimmickData::Desc& data, Transform* pParent) :
		Gimmick{},
		_pImpl{ new Impl{ data, pParent } }
	{
	}

	Player::~Player()
	{
	}

	void Player::Update()
	{
		_pImpl->Update();
	}

	void Player::Draw() const
	{
		_pImpl->Draw();
	}

	Circle Player::GetCollision() const
	{
		return _pImpl->GetCollision();
	}

	void Player::SetIsActive(const bool isActive)
	{
		BitFlag::PopOrUnPop(_pImpl->flag, GimmickData::gACTIVE, isActive);
	}

	bool Player::GetIsActive()
	{
		return BitFlag::CheckAnyPop(_pImpl->flag, GimmickData::gACTIVE);
	}

	void Player::UpdateDirection()
	{
		auto angle = _pImpl->transform.GetWorlAngle();

		_pImpl->down = Vec2::Down().rotate(angle);

		_pImpl->right = Vec2{ _pImpl->down.y, -_pImpl->down.x };
	}

	void Player::UpdateDirection(const double angle)
	{
		if (angle == _pImpl->transform.GetWorlAngle())
		{
			return;
		}

		_pImpl->transform.SetWorlAngle(angle);

		UpdateDirection();
	}

	void Player::HitCheck(const Array<LineString>& aGroundLines, const Array<std::weak_ptr<GimmickData::Gimmick>>& apGimmicks)
	{
		_pImpl->HitCheck(aGroundLines, apGimmicks);
	}

	void Player::SetParent(Transform* pParent)
	{
		_pImpl->transform.SetParent(pParent);
	}

	GimmickData::Type Player::GetType()
	{
		return static_cast<GimmickData::Type>(_pImpl->type);
	}

	void Player::SetDestroy()
	{
		BitFlag::UnPop(_pImpl->flag, GimmickData::gENABLE);
	}

	bool Player::GetEnable()
	{
		return BitFlag::CheckAnyPop(_pImpl->flag, GimmickData::gENABLE);
	}
}
