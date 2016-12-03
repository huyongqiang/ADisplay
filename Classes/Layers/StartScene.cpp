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
	auto intervalValue = interval->GetText();



	
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
	
	
	
	
		

	return true;
}

void StartScene::menuCallback(Ref* sender)
{
	Director::getInstance()->replaceScene(SplashLayer::createScene());
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
