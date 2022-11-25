#include <Siv3D.hpp>
#include "Transform.h"
#include "Stage.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "GameScene.h"
#include "EditorScene.h"

void Main()
{
	Window::Resize(1200, 720);

	MySystem::App manager;
	manager.add<MySystem::TitleScene>(U"Title");
	manager.add<MySystem::SelectScene>(U"Select");
	manager.add<MySystem::GameScene>(U"Game");
	manager.add<MySystem::EditorScene>(U"Editor");

	//manager.init(U"Game");

	while (System::Update())
	{
		if (manager.update())
		{

		}
	}
}
