#pragma once
#include "cocos2d.h"
#include "Layers\BaseLayer.h"
#include <iostream>
#include <io.h>
#include <string>

using namespace std;

USING_NS_CC;

class StartScene : public BaseLayer
{
public:
	StartScene();
	~StartScene();
	CREATE_FUNC(StartScene);
	virtual bool init() override;
	static Scene* createScene();

private:
	void getFileList(string path);
	void updateContent(float dt);
	float getSpriteResizeScale(Sprite* sp, float width=0);
	void judgeIndexOverflow();

private:
	vector<string> fileList;
	Sprite *preSp, *curSp, *nextSp, *nextNextSp;
	Vec2 prePos, curPos, nextPos, nextNextPos;
	int index;
};

