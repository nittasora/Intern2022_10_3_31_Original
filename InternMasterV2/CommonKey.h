#pragma once

#include <Siv3D.hpp>

namespace MySystem::CommonKeyState
{
	static const InputGroup gINPUT_LEFT = (KeyLeft | KeyA);		//<! 左移動用キー
	static const InputGroup gINPUT_RIGHT = (KeyRight | KeyD);	//<! 右移動用キー
	static const InputGroup gINPUT_JUMP = (KeyW | KeySpace);	//<! ジャンプ用キー
	static const InputGroup gINPUT_ANY_KEY = (gINPUT_LEFT | gINPUT_RIGHT | gINPUT_JUMP);	//<! 使用しているいずれかのキー

	static const InputGroup gINPUT_SUBMIT = (MouseL);		//<! 決定用マウスボタン
	static const InputGroup gINPUT_CANCEL = (MouseR);		//<! キャンセル用マウスボタン
	static const InputGroup gINPUT_PIECE_ACTION = (MouseR);	//<! カケラ操作用マウスボタン
	static const InputGroup gINPUT_ANY_MOUSE = (gINPUT_SUBMIT | gINPUT_CANCEL | gINPUT_PIECE_ACTION);	//<! 使用しているいずれかのマウスボタン
}
