#include "AppDelegate.h"
#include "Layers\StartScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
	//glfwInit();
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
		/*int monitorCount = 0;
		auto monitors = glfwGetMonitors(&monitorCount);
		log("monitor %d.", monitorCount);*/
		
		/*if (monitorCount >= 1){*/
			glview = GLViewImpl::createWithFullScreen("ADisplay");
			//glview = GLViewImpl::createWithRect("ADisplay", Rect(30, 500, 1245, 700));
			director->setOpenGLView(glview);
		//}
		//else
		//{
		//	/*MessageBox("number of monitor is less 2", "Waring");
		//	return false;*/
		//	glview = GLViewImpl::createWithRect("ADisplay", Rect(30, 500, 405, 300));
		//	director->setOpenGLView(glview);
		//}
    }

	glview->setDesignResolutionSize(1366, 768, ResolutionPolicy::FIXED_HEIGHT);

	FileUtils::getInstance()->addSearchPath("images");
    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
	auto scene = StartScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    //CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
	//CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
