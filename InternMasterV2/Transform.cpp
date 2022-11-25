/** ****************************************************************************************************
 * @file
 * @brief トランスフォーム情報格納用
 **************************************************************************************************** */
#include "Transform.h"

namespace MySystem
{
	Transform::Transform() noexcept :
		_localPosition{ 0.0, 0.0 },
		_localScale{ 1.0, 1.0 },
		_localAngle{ 0.0 },
		_worldPosition{ 0.0, 0.0 },
		_worldScale{ 1.0, 1.0 },
		_worldAngle{ 0.0 },
		_pParent{ nullptr }
	{

	}

	Transform::Transform(Transform* pParent) noexcept :
		_localPosition{ 0.0, 0.0 },
		_localScale{ 1.0, 1.0 },
		_localAngle{ 0.0 },
		_worldPosition{ 0.0, 0.0 },
		_worldScale{ 1.0, 1.0 },
		_worldAngle{ 0.0 },
		_pParent{ nullptr }
	{
		SetParent(pParent);
		SetLocalPosition(Transform::Position{ 0.0, 0.0 });
		SetLocalScale(Transform::Scale{ 1.0, 1.0 });
		SetLocalAngle(Transform::Angle{ 0.0 });
	}

	Transform::Transform(const Position& position, const Scale& scale, const Angle& angle) noexcept :
		_localPosition{ position },
		_localScale{ scale },
		_localAngle{ angle },
		_worldPosition{ _localPosition },
		_worldScale{ _localScale },
		_worldAngle{ _localAngle },
		_pParent{ nullptr }
	{

	}

	Transform::Transform(const Position& position, const Scale& scale, const Angle& angle, Transform* pParent) noexcept :
		_localPosition{ 0.0, 0.0 },
		_localScale{ 1.0, 1.0 },
		_localAngle{ 0.0 },
		_worldPosition{ 0.0, 0.0 },
		_worldScale{ 1.0, 1.0 },
		_worldAngle{ 0.0 },
		_pParent{ nullptr }
	{
		SetParent(pParent);
		SetLocalPosition(position);
		SetLocalScale(scale);
		SetLocalAngle(angle);
	}

	Transform::~Transform() noexcept
	{
		// 子供たちを親にゆだねる
		for (auto child : _paChildren)
		{
			child->SetParent(_pParent);
		}

		SetParent(nullptr);
	}

	void Transform::UpdateTransform() noexcept
	{
		// 親のワールド座標から自身のワールド座標を得る
		if (_pParent)
		{
			_worldPosition = _localPosition.rotated(_pParent->_worldAngle) * _pParent->_worldScale + _pParent->_worldPosition;
			_worldScale = _localScale * _pParent->_worldScale;
			_worldAngle = _localAngle + _pParent->_worldAngle;
		}
		else
		{
			_worldPosition = _localPosition;
			_worldScale = _localScale;
			_worldAngle = _localAngle;
		}

		// 大抵の場合自身が更新された場合子も更新されるはず
		for (auto child : _paChildren)
		{
			child->UpdateTransform();
		}
	}

	void Transform::SetParent(Transform* pParent) noexcept
	{
		// すでに親がいるなら外す
		if (_pParent) {
			auto& rChildren = _pParent->_paChildren;

			for (auto itr = rChildren.begin(); itr != rChildren.end(); ++itr)
			{
				if (*itr != this)
				{
					continue;
				}

				itr = rChildren.erase(itr);

				// もしも消去後にイテレーターが末尾を指した場合、次のループでエラーが起こる
				if (itr == rChildren.end())
				{
					break;
				}
			}
		}

		_pParent = pParent;

		if (_pParent) {
			// 新しく親が付く場合、自身の座標は親に合わせなければならない。
			_localPosition	= ((_worldPosition - _pParent->_worldPosition) / _pParent->_worldScale).rotated(-_pParent->_worldAngle);
			_localScale		= _worldScale / _pParent->_worldScale;
			_localAngle		= _worldAngle - _pParent->_worldAngle;

			// 親の子供に自身を格納
			_pParent->_paChildren.emplace_back(this);
		}
		else {
			// 親がいなくなったのでワールド座標が自身の座標として正しくなる
			_localPosition	= _worldPosition;
			_localScale		= _worldScale;
			_localAngle		= _worldAngle;
		}

		// 子の座標も更新
		for (auto child : _paChildren)
		{
			child->UpdateTransform();
		}
	}

	void Transform::SetChild(Transform* pChild) noexcept
	{
		// 自身に子を与える = 子に自身を親として与える
		pChild->SetParent(this);
	}

	Transform* Transform::GetParent() noexcept
	{
		return _pParent;
	}

	Transform* Transform::GetChild(const size_t index) noexcept
	{
		if (index >= _paChildren.size())
		{
			return nullptr;
		}

		return _paChildren[index];
	}

	Array<Transform*> Transform::GetChildren() noexcept
	{
		return _paChildren;
	}

	size_t Transform::GetChildCount() noexcept
	{
		return _paChildren.size();
	}

	bool Transform::HasParent() noexcept
	{
		return _pParent != nullptr;
	}

	void Transform::SetLocalPosition(const Position& position) noexcept
	{
		_localPosition = position;

		UpdateTransform();
	}

	void Transform::SetLocalScale(const Scale& scale) noexcept
	{
		_localScale = scale;

		UpdateTransform();
	}

	void Transform::SetLocalAngle(const Angle& angle) noexcept
	{
		_localAngle = angle;

		UpdateTransform();
	}

	void Transform::SetWorlPosition(const Position& position) noexcept
	{
		_worldPosition = position;

		if (not _pParent)
		{
			_localPosition = _worldPosition;
			return;
		}

		_localPosition = ((_worldPosition - _pParent->_worldPosition) / _pParent->_worldScale).rotated(_pParent->_worldAngle);

		UpdateTransform();
	}

	void Transform::SetWorlScale(const Scale& scale) noexcept
	{
		_worldScale = scale;

		if (not _pParent)
		{
			_localScale = _worldScale;
			return;
		}

		_localPosition = ((_worldPosition - _pParent->_worldPosition) / _pParent->_worldScale).rotated(_pParent->_worldAngle);
		_localScale = _worldScale / _pParent->_worldScale;

		UpdateTransform();
	}

	void Transform::SetWorlAngle(const Angle& angle) noexcept
	{
		_worldAngle = angle;

		if (not _pParent)
		{
			_localAngle = _worldAngle;
			return;
		}

		_localPosition = ((_worldPosition - _pParent->_worldPosition) / _pParent->_worldScale).rotated(_pParent->_worldAngle);
		_localAngle = _worldAngle - _pParent->_worldAngle;

		UpdateTransform();
	}

	Transform::Position Transform::GetLocalPosition() const noexcept
	{
		return _localPosition;
	}

	Transform::Scale Transform::GetLocalScale() const noexcept
	{
		return _localScale;
	}

	Transform::Angle Transform::GetLocalAngle() const noexcept
	{
		return _localAngle;
	}

	Transform::Position Transform::GetWorlPosition() const noexcept
	{
		return _worldPosition;
	}

	Transform::Scale Transform::GetWorlScale() const noexcept
	{
		return _worldScale;
	}

	Transform::Angle Transform::GetWorlAngle() const noexcept
	{
		return _worldAngle;
	}
}
