#include "SelectScene.h"
#include "ButtonGUI.h"
#include "Stage.h"
#include "CommonKey.h"

namespace MySystem
{
	class SelectScene::Impl
	{
	public:
		ImageButtonGUI startButton;
		Stage stage;
		StageData::WorldIndex worldIndex;
		StageData::StageIndex stageIndex;

		Impl() :
			startButton{ U"Assets/Image/StartButton.png", Vec2{ 0.0, Scene::Size().y - 158.0}, Vec2{308.0, 158.0}},
			stage{ Scene::CenterF(), Vec2{ 0.75, 0.75 }, 0.0, nullptr },
			worldIndex{ static_cast<StageData::WorldIndex>(100)},
			stageIndex{ static_cast<StageData::StageIndex>(100) }
		{

		}
	};

	SelectScene::SelectScene(const InitData& init) :
		IScene{ init },
		_pImpl{ new Impl{} }
	{
		auto& data = getData();

		if (data.stageIndex != _pImpl->stageIndex || data.worldIndex != _pImpl->worldIndex)
		{
			_pImpl->worldIndex = data.worldIndex;
			_pImpl->stageIndex = data.stageIndex;
			_pImpl->stage.CreateStage(data.worldIndex, data.stageIndex);
		}
	}

	SelectScene::~SelectScene()
	{
	}

	void SelectScene::update()
	{
		auto& data = getData();

		if (CommonKeyState::gINPUT_LEFT.down())
		{
			if (data.stageIndex > 0)
			{
				data.stageIndex--;
			}
		}
		if (CommonKeyState::gINPUT_RIGHT.down())
		{
			data.stageIndex = (data.stageIndex + 1) % CommonSceneData::gMAX_STAGE_NUM;
		}

		if (data.stageIndex != _pImpl->stageIndex || data.worldIndex != _pImpl->worldIndex)
		{
			_pImpl->worldIndex = data.worldIndex;
			_pImpl->stageIndex = data.stageIndex;
			_pImpl->stage.CreateStage(data.worldIndex, data.stageIndex);
		}

		_pImpl->startButton.Update();

		if (_pImpl->startButton.GetIsOn())
		{
			changeScene(U"Game");
		}
	}

	void SelectScene::draw() const
	{
		_pImpl->stage.Draw();
		_pImpl->startButton.Draw();
	}
}
