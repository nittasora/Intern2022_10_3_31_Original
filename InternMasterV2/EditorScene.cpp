#include "EditorScene.h"

namespace MySystem
{
	class EditorScene::Impl
	{

	};

	EditorScene::EditorScene(const InitData& init) :
		IScene{ init },
		_pImpl{ new Impl{} }
	{
	}

	EditorScene::~EditorScene()
	{
	}

	void EditorScene::update()
	{
	}

	void EditorScene::draw() const
	{
	}
}
