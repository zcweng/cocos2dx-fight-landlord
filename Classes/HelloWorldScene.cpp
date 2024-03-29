#include "HelloWorldScene.h"
#include "NetworkCommunication.h"
#include "CocoStudio\Json\rapidjson\writer.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

	CCMenuItemImage *pConnectButton = CCMenuItemImage::create(
											"test/bt_orange.png",
											"test/bt_selected.png",
											this,
											menu_selector(HelloWorld::menuConnectCallback));

	pConnectButton->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));


	CCMenuItemImage *pRenameButton = CCMenuItemImage::create(
											"test/bt_green.png",
											"test/bt_selected.png",
											this,
											menu_selector(HelloWorld::menuConnectCallback));

	pRenameButton->setPosition(ccp(origin.x + visibleSize.width / 2 + 100, origin.y + visibleSize.height / 2));

	CCMenuItemImage *pGetInfoButton = CCMenuItemImage::create(
											"test/bt_red.png",
											"test/bt_selected.png",
											this,
											menu_selector(HelloWorld::menuConnectCallback));

	pGetInfoButton->setPosition(ccp(origin.x + visibleSize.width / 2 + 200, origin.y + visibleSize.height / 2));

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::create(pConnectButton, pRenameButton, pGetInfoButton, NULL);
    pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);




    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("test/bg.png");
	/*pSprite->setScale();*/

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);

	extension::NetworkCommunication::getInstance()->setConnectResultCallback(this, connectresult_selector(HelloWorld::onConnectResult));
	extension::NetworkCommunication::getInstance()->setResponseCallback(this, httpresponse2_selector(HelloWorld::onResponse));
    
    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

void HelloWorld::menuConnectCallback(CCObject * pSender) {
	extension::NetworkCommunication::getInstance()->connect("172.20.133.41", 4000);
}

void HelloWorld::menuRenameCallback(CCObject * pSender) {
	char * jsonData = "{\"command\":2, \"new_name\":\"Jonathan\"}";
	extension::NetworkCommunication::getInstance()->send(jsonData, strlen(jsonData));
}

void HelloWorld::menuGetInfoCallback(CCObject * pSender) {
	char * jsonData = "{\"command\":4}";
	extension::NetworkCommunication::getInstance()->send(jsonData, strlen(jsonData));
}

void HelloWorld::onResponse(char * response) {
	CCLog("Jonathan: in HelloWorld::onResponse. response = %s", response);
	
	if (response != NULL) {
		CCLabelTTF* pLabel = CCLabelTTF::create(response, "Arial", 16);
    
		// position the label on the center of the screen
		pLabel->setPosition(ccp(200, 200));

		this->addChild(pLabel, 2);
	}
}

void HelloWorld::onConnectResult(int resultCode, char * desc) {
	CCLog("Jonathan: in onConnectResult. resultCode = %d", resultCode);
	char * jsonData = "{\"command\":4}";
	extension::NetworkCommunication::getInstance()->send(jsonData, strlen(jsonData));
}

