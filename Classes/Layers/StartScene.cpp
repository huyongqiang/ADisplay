#include "StartScene.h"
#include "SplashLayer.h"
#include "GameManager.h"
#include "../external/tinyxml2/tinyxml2.h"

StartScene::StartScene()
{
	index = 0;
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

	prePos = Vec2(-size.width * 0.25, size.height / 2);
	curPos = Vec2(size.width / 2, size.height / 2);
	nextPos = Vec2(size.width * 1.25, size.height / 2);
	nextNextPos = Vec2(size.width * 1.5, size.height / 2);
	
	preSp = Sprite::create(fileList.at(index));
	preSp->setPosition(prePos);
	addChild(preSp);
	index++;
	judgeIndexOverflow();

	curSp = Sprite::create(fileList.at(index));
	curSp->setPosition(curPos);
	addChild(curSp);
	index++;
	judgeIndexOverflow();

	nextSp = Sprite::create(fileList.at(index));
	nextSp->setPosition(nextPos);
	addChild(nextSp);
	index++;
	judgeIndexOverflow();

	nextNextSp = Sprite::create(fileList.at(index));
	nextNextSp->setPosition(nextNextPos);
	addChild(nextNextSp);
	index++;
	judgeIndexOverflow();

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

void StartScene::updateContent(float dt)
{
	auto time = 2.0f;
	curSp->runAction(EaseExponentialOut::create(MoveTo::create(time, prePos)));
	curSp->runAction(EaseExponentialOut::create(ScaleTo::create(time, getSpriteResizeScale(curSp, size.width * 0.25))));
	nextSp->runAction(EaseExponentialOut::create(MoveTo::create(time, curPos)));
	nextSp->runAction(EaseExponentialOut::create(ScaleTo::create(time, getSpriteResizeScale(nextSp))));
	nextNextSp->runAction(Sequence::create(MoveTo::create(time, nextPos),
		CallFunc::create([=](){
		
		preSp = curSp;
		curSp = nextSp;
		nextSp = nextNextSp;

		nextNextSp =  Sprite::create(fileList.at(index));
		nextNextSp->setPosition(nextNextPos);
		addChild(nextNextSp);
		nextNextSp->setScale(getSpriteResizeScale(nextNextSp, size.width*0.25));
		index++;

		judgeIndexOverflow();
		
	}), NULL));
}

void StartScene::judgeIndexOverflow()
{
	if (index >= fileList.size())
		index = 0;
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
