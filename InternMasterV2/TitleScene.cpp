#include "TitleScene.h"
#include "ButtonGUI.h"

namespace MySystem
{
	class TitleScene::Impl
	{
	public:
		ImageButtonGUI startButton;
		ImageButtonGUI endButton;

		Impl() :
			startButton{ U"Assets/Image/StartButton.png", Vec2{ Scene::CenterF().x - 400.0, Scene::CenterF().y + 200.0 }, Vec2{ 308.0, 158.0 } },
			endButton{ U"Assets/Image/EndButton.png", Vec2{ Scene::CenterF().x + 92.0, Scene::CenterF().y + 200.0 }, Vec2{ 308.0, 158.0 } }
		{

		}
	};

	TitleScene::TitleScene(const InitData& init) :
		IScene{ init },
		_pImpl{ new Impl{} }
	{
		TextureAsset::Register(U"TitleImage", U"Assets/Image/Title.png");
	}

	TitleScene::~TitleScene()
	{
	}

	void TitleScene::update()
	{
		_pImpl->startButton.Update();
		_pImpl->endButton.Update();

		if (_pImpl->startButton.GetIsOn())
		{
			changeScene(U"Select");
		}
		else if (_pImpl->endButton.GetIsOn()) {
			System::Exit();
		}
	}

	void TitleScene::draw() const
	{
		TextureAsset(U"TitleImage").drawAt(Scene::CenterF());

		_pImpl->startButton.Draw();
		_pImpl->endButton.Draw();
	}
}
