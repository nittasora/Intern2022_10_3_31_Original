#pragma once

#include "SceneCommon.h"

namespace MySystem
{
	class EditorScene : public App::Scene
	{
	private:
		class Impl;
		std::unique_ptr<Impl> _pImpl;

	public:
		EditorScene(const InitData& init);
		~EditorScene();

		void update() override;
		void draw() const override;
	};
}
