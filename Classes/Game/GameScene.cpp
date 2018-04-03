//
//  GameScene.cpp
//  FristGame
//
//  Created by sfbest on 2017/4/14.
//
//

#include "GameScene.hpp"
#include "HomeScene.hpp"

Game *Game::create(GameType gameType, GamePropModel *model)
{
    auto game = new Game();
    
    if (game && game->init(gameType, model)) {
        game->autorelease();
    } else {
        delete game;
        game = nullptr;
    }
    
    return game;
}

Scene *Game::createScene(GameType gameType, GamePropModel *model)
{
    auto scene = Scene::createWithPhysics();
//    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = Game::create(gameType, model);
    scene->getPhysicsWorld()->setGravity(Vec2(0, -1500));
    scene->addChild(layer);
    
    // addPhysicsEdge
    auto edge = Node::create();
    edge->addComponent(PhysicsBody::createEdgeBox(Size(kWinSizeWidth, 20), PHYSICSBODY_MATERIAL_DEFAULT, 30));
    edge->getPhysicsBody()->setDynamic(false);
    edge->setContentSize(Size(kWinSizeWidth, 10));
    edge->setPosition(Vec2(kWinSizeWidth * 0.5, kWinSizeHeight + 35));
    
    scene->addChild(edge);
    
    return scene;
}

bool Game::init(GameType gameType, GamePropModel *model)
{
    if (!Layer::init()) return false;
    
    _gameType = gameType;
    
    if (gameType == Time) {
        CCLOG("时间");
    } else {
        CCLOG("守护");
    }
    
    
    Sprite *bgSprite = Sprite::create("game_bg_0.png");
    bgSprite->setScale(0.86);
    bgSprite->setPosition(kWinSizeWidth * 0.5, kWinSizeHeight * 0.5);
    addChild(bgSprite);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game_01.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("goldimages.plist");
    
    // 添加怪物层
    _monsterLayer = MonsterLayer::create(gameType);
    addChild(_monsterLayer);

    // 添加道具层
    _gamePropLayer = GamePropLayer::create(gameType, model);
    this->addChild(_gamePropLayer);
    
    // 添加游戏控制层
    _gameControl = GameControl::create(gameType, model);
    addChild(_gameControl);
    
    // 添加用户绘制层
    _userDrawLayer = UserDrawLayer::create();
    addChild(_userDrawLayer);
    
    // 添加自定义事件监听回调
    addCustomActions();
    
    startGame();
    
    return true;
}

//添加自定义事件监听回调
void Game::addCustomActions()
{
    // 消除气球回调
    auto cleanBalloonLis = EventListenerCustom::create("CleanBalloon", [&](EventCustom *event){
        BalloonResult *res = static_cast<BalloonResult *>(event->getUserData());
        _monsterLayer->removeBalloonWithBalloonType(res->getType());
        
    });
    
    // 守护模式下 漏掉一个Monster
    auto passMonsterLis = EventListenerCustom::create("PassMonster", [&](EventCustom *event){
        _gameControl->lifeCutDown();
    });
    
    // 消除Monster
    auto cleanMonster = EventListenerCustom::create("killMonster", [&](EventCustom *event){
        int cleanCount = atoi(((char *)event->getUserData()));
        
        int goldNum = 0;
        switch (cleanCount) {
            case 1:
                goldNum = 2;
                break;
            case 2:
                goldNum = 5;
                break;
            case 3:
                goldNum = 10;
                break;
            case 4:
                goldNum = 20;
                break;
            case 6:
                goldNum = 20;
            default:
                goldNum = 20;
                break;
        }
        
        if (cleanCount == 1001) {
            // add bobm
            _gamePropLayer->addBobm();
        } else if (cleanCount == 1002) {
            // add 减速
            _gamePropLayer->addDeceleration();
        } else {        
            // 增加金币
            _gameControl->addGoldCount(goldNum);
            // 增加击杀个数
            _gameControl->killCountAdd(cleanCount == 6 ? 0 : cleanCount);
            _gameControl->showContinuousHitAndKill(cleanCount);
        }
    });
    
    auto timeOver = EventListenerCustom::create("TimeOver", [&](EventCustom *event){
        // 暂停绘画功能
        _userDrawLayer->setVisible(false);
        // 停止怪物掉落
        _monsterLayer->pause();
        // 显示游戏结束
        _gameControl->showEndUI();
        // 隐藏道具层
        _gamePropLayer->setVisible(false);
    });
    
    // 暂停
    auto gamePaues = EventListenerCustom::create("GamePause", [&](EventCustom *event){
        _gameControl->showGamePauseLayer();
        _userDrawLayer->setVisible(false);
        _gameControl->pause();
        
        _monsterLayer->pause();
    });
    
    // 回到首页
    auto goHome = EventListenerCustom::create("GoHome", [](EventCustom *event){
        Director::getInstance()->replaceScene(Home::createScene());
    });
    
    // 继续
    auto gameResume = EventListenerCustom::create("GameResume", [&](EventCustom *event){
        _gameControl->resume();
        _monsterLayer->resume();
        _userDrawLayer->setVisible(true);
    });
    
    // 添加道具使用
    auto propListener = EventListenerCustom::create("PropEventName", [=](EventCustom *custom){
        char *type = (char *)(custom->getUserData());
        if (type == "Bomb") {
            _monsterLayer->useProp(true);
        } else {
            _monsterLayer->useProp(false);
        }
    });
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(propListener, 1);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(cleanBalloonLis, 1);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(passMonsterLis, 1);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(cleanMonster, 1);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(timeOver, 1);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(gamePaues, 1);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(goHome, 1);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(gameResume, 1);
}

void Game::onExit()
{
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("PropEventName");
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("GamePause");
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("TimeOver");
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("killMonster");
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("PassMonster");
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("CleanBalloon");
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("GoHome");
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("GameResume");
    
    Layer::onExit();
}

// 开始游戏
void Game::startGame()
{
    startAppearMonster();
    _gameControl->startClocking();
}

// 开始出现怪物
void Game::startAppearMonster()
{
    _monsterLayer->startAppearMonster();
}




