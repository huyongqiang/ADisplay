#include "StartScene.h"
#include "SplashLayer.h"
#include "GameManager.h"
#include "../external/tinyxml2/tinyxml2.h"

StartScene::StartScene()
{
}

StartScene::~StartScene()
{
}

Scene* StartScene::createScene()
{
	auto scene = Scene::create();
	auto layer = StartScene::create();
	scene->addChild(layer);

	return scene;
}

bool StartScene::init()
{
	if (!BaseLayer::init())
		return false;

	//esc to exit
	auto keypadListener = EventListenerKeyboard::create();
	keypadListener->onKeyReleased = [](EventKeyboard::KeyCode keyCode, Event* event){
		if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
			Director::getInstance()->end();
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keypadListener, this);

	//get the files' list
	auto paths = FileUtils::getInstance()->getSearchPaths();
	getFileList(paths.at(1).c_str());

	//get interval value of the settings
	auto doc = new tinyxml2::XMLDocument();
	doc->LoadFile(FileUtils::getInstance()->fullPathForFilename("settings/settings.xml").c_str());
	auto root = doc->RootElement();
	auto interval = root->FirstChildElement();
	auto intervalText = interval->GetText();
	float intervalValue = stof(intervalText);
	delete doc;

	auto progressBar = ProgressTimer::create(Sprite::create("res/progressbar.png"));
	progressBar->setPosition(Vec2(size.width / 2, progressBar->getContentSize().height / 2));
	progressBar->setType(ProgressTimer::Type::BAR);
	progressBar->setBarChangeRate(Vec2(1, 0));
	progressBar->setMidpoint(Vec2(0, 0));
	progressBar->setReverseDirection(true);
	addChild(progressBar, 10);
	auto pt = Sequence::createWithTwoActions(ProgressTo::create(intervalValue, 100), ProgressTo::create(0, 0));
	progressBar->runAction(RepeatForever::create(pt));

	
	/*for (auto item : fileList){
		auto sp = Sprite::create(item);
		sp->setPosition(Vec2(size.width / 2, size.height / 2));
		addChild(sp);
	}*/

	/*auto play = MenuItemFont::create("Play", CC_CALLBACK_1(StartScene::menuCallback, this));
	play->setPosition(size.width / 2, size.height / 2);
	auto menu = Menu::create(play, NULL);
	menu->setPosition(Vec2::ZERO);
	addChild(menu);*/

	prePos = Vec2(-size.width * 0.25, size.height / 2);
	curPos = Vec2(size.width / 2, size.height / 2);
	nextPos = Vec2(size.width * 1.25, size.height / 2);
	nextNextPos = Vec2(size.width * 1.5, size.height / 2);
	
	preSp = Sprite::create(fileList.at(0));
	preSp->setPosition(prePos);
	addChild(preSp);

	curSp = Sprite::create(fileList.at(1));
	curSp->setPosition(curPos);
	addChild(curSp, 1);

	nextSp = Sprite::create(fileList.at(2));
	nextSp->setPosition(nextPos);
	addChild(nextSp);

	nextNextSp = Sprite::create(fileList.at(3));
	nextNextSp->setPosition(nextNextPos);
	addChild(nextNextSp);

	curSp->setScale(getSpriteResizeScale(curSp));
	preSp->setScale(getSpriteResizeScale(preSp, size.width*0.25));
	nextSp->setScale(getSpriteResizeScale(nextSp, size.width*0.25));
	nextNextSp->setScale(getSpriteResizeScale(nextNextSp, size.width*0.25));

	schedule(schedule_selector(StartScene::updateContent), intervalValue);
	
	

	return true;
}

float StartScene::getSpriteResizeScale(Sprite* sp, float width)
{
	if (width == 0){
		auto imgWidth = sp->getContentSize().width;
		auto imgHeight = sp->getContentSize().height;
		auto scaleHeight = size.height / imgHeight;
		auto scaleWidth = size.width / imgWidth;
		if (scaleWidth > scaleHeight)
			return scaleHeight;
		else
			return scaleWidth;
	}
	else
	{
		return width / sp->getContentSize().width;
	}
}

void StartScene::menuCallback(Ref* sender)
{
	Director::getInstance()->replaceScene(SplashLayer::createScene());
}

void StartScene::updateContent(float dt)
{
	auto time = 2.0f;
	curSp->runAction(EaseExponentialOut::create(MoveTo::create(time, prePos)));
	curSp->runAction(EaseExponentialOut::create(ScaleTo::create(time, getSpriteResizeScale(curSp, size.width * 0.25))));
	nextSp->runAction(EaseExponentialOut::create(MoveTo::create(time, curPos)));
	nextSp->runAction(EaseExponentialOut::create(ScaleTo::create(time, getSpriteResizeScale(nextSp))));
	nextNextSp->runAction(Sequence::create(MoveTo::create(time, nextPos),
		CallFunc::create([=](){
		auto temp = preSp;
		preSp = curSp;
		curSp = nextSp;
		nextSp = nextNextSp;
		nextNextSp = temp;

		nextNextSp->setPosition(nextNextPos);
	}), NULL));
}

void StartScene::getFileList(string path)
{
	long hFile = 0;
	struct _finddata_t fileInfo;
	string pathName, exdName;

	if ((hFile = _findfirst(pathName.assign(path).append("\\*").c_str(), &fileInfo)) == -1) {
		return;
	}
	do {
		//cout << fileInfo.name << (fileInfo.attrib&_A_SUBDIR ? "[folder]" : "[file]") << endl;
		//log("%s: %s", fileInfo.name, (fileInfo.attrib&_A_SUBDIR ? "[folder]" : "[file]"));
		if (!(fileInfo.attrib&_A_SUBDIR)){
			fileList.push_back(fileInfo.name);
		}
	} while (_findnext(hFile, &fileInfo) == 0);
	_findclose(hFile);
	return;
}
