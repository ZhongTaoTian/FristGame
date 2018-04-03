//
//  GameControlLayer.cpp
//  FristGame
//
//  Created by sfbest on 2017/5/27.
//
//

#include "GameControlLayer.hpp"
#include "UserDataManager.hpp"
#include "HomeScene.hpp"
#include "ShopCartLayer.hpp"
#include "SelectGamePropLayer.hpp"

bool GameControl::init(GameType type, GamePropModel *model)
{
    Node::init();
    
    // 背景
    auto csb = CSLoader::createNode("Game.csb");
    addChild(csb);
    
    _killTipsSp = (Sprite *)(csb->getChildByTag(35)->getChildByTag(86));
    
    int addLife = 0;
    int addTime = 0;
    if (model) {
        if (model->hasAdd1) {
            addLife += 2;
            addTime += 30;
        }
    
        if (model->hasAdd2) {
            addLife += 4;
            addTime += 60;
        }
        
        if (model->hasAdd3) {
            addLife += 6;
            addTime += 90;
        }
    }
    
    _life = 5 + addLife;
    _gameType = type;
    
    _pauseBtn = (Button *)(csb->getChildByTag(35))->getChildByTag(30);
    _pauseBtn->addTouchEventListener([=](Ref* sender,Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            EventCustom event("GamePause");
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }
    });
    Texture2D *numT = TextureCache::getInstance()->addImage("res/num_yellow.png");
    _goldLabel = LabelAtlas::create("0", "res/num_yellow.png", numT->getContentSize().width / 12, numT->getContentSize().height, '.');
    _goldLabel->setAnchorPoint(Vec2(0, 1));
    this->addChild(_goldLabel);
    
    auto gold = (csb->getChildByTag(35))->getChildByTag(996);
    _goldLabel->setPosition(gold->getBoundingBox().getMaxX() + 10, gold->getBoundingBox().getMaxY() - _goldLabel->getContentSize().height * 0.25);
    
    _lifeSprite = (Sprite *)(csb->getChildByTag(35))->getChildByTag(28);
    _timeSprite = (Sprite *)(csb->getChildByTag(35))->getChildByTag(32);
    _goldSp = (Sprite *)(csb->getChildByTag(35))->getChildByTag(996);
    
    string lifeText = "5";
    if (type == Time) {
        _lifeSprite->setVisible(false);
    } else {
        _timeSprite->setVisible(false);
    }
    
    _time = 120 + addTime;
    
    _lifeLabel = LabelAtlas::create(_gameType == Time ? to_string(_time) : lifeText, "res/num_green.png", numT->getContentSize().width / 12, numT->getContentSize().height, '.');
    _lifeLabel->setAnchorPoint(Vec2(0.5, 1));
    _lifeLabel->setScale(0.8);
    addChild(_lifeLabel);
    _lifeLabel->setPosition(Vec2(kWinSizeWidth * 0.19, _goldLabel->getPosition().y - 2));
    
    _killCountLabel = Label::createWithSystemFont("0", "Marker Felt", 250);
    _killCountLabel->setPosition(kWinSizeWidth * 0.5, kWinSizeHeight * 0.6);
    _killCountLabel->setOpacity(50);
    
    addChild(_killCountLabel);
    
    
    return true;
}

void GameControl::killCountAdd(int killCount)
{
    _killCount += killCount;
    
    _killCountLabel->setString(to_string(_killCount));
}

void GameControl::addGoldCount(int goldCount)
{
    _goldCount += goldCount;;
    _goldLabel->setString(to_string(_goldCount));
}

void GameControl::lifeCutDown()
{
    if (_gameType == _time) return;
    // 掉落+1
    _life--;
    
    _lifeLabel->setString(to_string(_life));
    
    if (_life == 0) {
        // 游戏失败
        EventCustom event("TimeOver");
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
}

void GameControl::startClocking()
{
    if (_gameType == Guard) return;
    schedule(schedule_selector(GameControl::update), 1);
}

// 显示连杀
void GameControl::showContinuousHitAndKill(int killNum)
{
    switch (killNum) {
        case 2:
            _killTipsSp->setTexture("tips_Good.png");
            break;
        case 3:
            _killTipsSp->setTexture("tips_Great.png");
            break;
        case 4:
            _killTipsSp->setTexture("tips_Perfect.png");
            break;
        default:
            break;
    }
    _killTipsSp->setVisible(killNum != 1);
    
    if (_killTipsSp->isVisible()) {
        _killTipsSp->runAction(Sequence::create(ScaleTo::create(0.2, 0.4), ScaleTo::create(0.15, 0.55), ScaleTo::create(0.1, 0.45), ScaleTo::create(0.1, 0.5), CallFunc::create([=](){
            _killTipsSp->setVisible(false);
        }) ,NULL));
    }
}

/// 显示完成样式
void GameControl::showEndUI()
{
    _goldLabel->setVisible(false);
    _killCountLabel->setVisible(false);
    _pauseBtn->setVisible(false);
    _lifeLabel->setVisible(false);
    _goldSp->setVisible(false);
    
    if (_gameType == GameType::Guard) {
        _lifeSprite->setVisible(false);
    } else {
        _timeSprite->setVisible(false);
    }
    
    auto finishCsb = CSLoader::createNode("GameFinish.csb");
    this->addChild(finishCsb);
    
    Text *bestScoreText = (Text *)finishCsb->getChildByTag(401);
    
    int bsetScore = 0;
    
    // 设置最好成绩
    if (_gameType == GameType::Time) {
        UserDataManager::getInstance()->setTimeBestScore(_killCount);
        bsetScore = UserDataManager::getInstance()->getTimeBestScore();
    } else {
        UserDataManager::getInstance()->setGuardBestScore(_killCount);
        bsetScore = UserDataManager::getInstance()->getGuardBestScore();
    }
    
    if (_goldCount > 0) {
        UserDataManager::getInstance()->addUserGoldCount(_goldCount);
    }
    
    char name[50];
    sprintf(name, "%s:%d", "最佳成绩", bsetScore);
    bestScoreText->setString(name);
    
    Sprite *icon = (Sprite *)finishCsb->getChildByTag(402);
    // 人物出现动画
    ScaleTo *scale = ScaleTo::create(0.4, 1);
    Vec2 position = icon->getPosition();
    auto move = Sequence::create(MoveTo::create(0.1, Vec2(position.x + 20, position.y + 20)), MoveTo::create(0.1, Vec2(position.x, position.y - 20)),MoveTo::create(0.1, Vec2(position.x + 20, position.y)),MoveTo::create(0.08, Vec2(position.x - 10, position.y - 17)),MoveTo::create(0.08, Vec2(position.x + 10, position.y + 10)),MoveTo::create(0.08, Vec2(position.x + 10, position.y - 10)),MoveTo::create(0.07, Vec2(position.x - 10, position.y + 10)),MoveTo::create(0.06, Vec2(position.x + 6, position.y - 6)),MoveTo::create(0.06, Vec2(position.x + 8, position.y + 8)),MoveTo::create(0.06, Vec2(position.x, position.y )), NULL);
    
    Sprite *fullSrar1 = (Sprite *)finishCsb->getChildByTag(403);
    Sprite *fullSrar2 = (Sprite *)finishCsb->getChildByTag(405);
    Sprite *fullSrar3 = (Sprite *)finishCsb->getChildByTag(408);
    Sprite *emptyStar1 = (Sprite *)finishCsb->getChildByTag(404);
    Sprite *emptyStar2 = (Sprite *)finishCsb->getChildByTag(406);
    Sprite *emptyStar3 = (Sprite *)finishCsb->getChildByTag(407);
    
    Sprite *monster = (Sprite *)finishCsb->getChildByTag(409);
    Sprite *score = (Sprite *)finishCsb->getChildByTag(410);
    Text *monsterText = (Text *)finishCsb->getChildByTag(411);
    Text *scoreText = (Text *)finishCsb->getChildByTag(412);
    monsterText->setString(to_string(_killCount));
    scoreText->setString(to_string(_goldCount));
    
    Button *btn1 = (Button *)finishCsb->getChildByTag(444);
    btn1->addTouchEventListener(CC_CALLBACK_2(GameControl::btnClick, this));
    
    Button *btn2 = (Button *)finishCsb->getChildByTag(445);
    btn2->addTouchEventListener(CC_CALLBACK_2(GameControl::btnClick, this));
    
    Button *btn3 = (Button *)finishCsb->getChildByTag(446);
    btn3->addTouchEventListener(CC_CALLBACK_2(GameControl::btnClick, this));
    
    Button *btn4 = (Button *)finishCsb->getChildByTag(447);
    btn4->addTouchEventListener(CC_CALLBACK_2(GameControl::btnClick, this));
    icon->runAction(Sequence::create(Spawn::create(scale, move, NULL), CallFunc::create([=](){
        
        monster->runAction(MoveTo::create(0.25, Vec2(kWinSizeWidth * 0.2, monster->getPosition().y)));
        monsterText->runAction(MoveTo::create(0.25, Vec2(kWinSizeWidth * 0.426, monsterText->getPosition().y)));
        score->runAction(MoveTo::create(0.25, Vec2(kWinSizeWidth * 0.72, score->getPosition().y)));
        scoreText->runAction(Sequence::create(MoveTo::create(0.15, Vec2(kWinSizeWidth * 0.9, scoreText->getPosition().y)), CallFunc::create([=](){
            emptyStar1->runAction(FadeTo::create(0.1, 100));
            emptyStar2->runAction(FadeTo::create(0.1, 100));
            emptyStar3->runAction(Sequence::create(FadeTo::create(0.1, 100), CallFunc::create([=](){
                
                if (_killCount >= 100) {
                    // 满分
                    fullSrar1->runAction(Sequence::create(ScaleTo::create(0.15, 1.1), ScaleTo::create(0.1, 0.9), ScaleTo::create(0.09, 1.05), ScaleTo::create(0.08, 0.95), ScaleTo::create(0.08, 1), CallFunc::create([=](){
                        fullSrar2->runAction(Sequence::create(ScaleTo::create(0.15, 1.1), ScaleTo::create(0.1, 0.9), ScaleTo::create(0.09, 1.05), ScaleTo::create(0.08, 0.95), ScaleTo::create(0.08, 1),  CallFunc::create([=](){
                            fullSrar3->runAction(Sequence::create(ScaleTo::create(0.15, 1.1), ScaleTo::create(0.1, 0.9), ScaleTo::create(0.09, 1.05), ScaleTo::create(0.08, 0.95), ScaleTo::create(0.08, 1), CallFunc::create([=](){
                                this->finishAnimationFinish();
                            }),  DelayTime::create(1.2), CallFunc::create([=](){
                                btn1->runAction(MoveTo::create(0.3, Vec2(btn1->getPosition().x, kWinSizeHeight * 0.12)));
                                btn2->runAction(MoveTo::create(0.3, Vec2(btn2->getPosition().x, kWinSizeHeight * 0.12)));
                                btn3->runAction(MoveTo::create(0.3, Vec2(btn3->getPosition().x, kWinSizeHeight * 0.12)));
                                btn4->runAction(MoveTo::create(0.3, Vec2(btn4->getPosition().x, kWinSizeHeight * 0.12)));
                            }), NULL));
                        }), NULL));
                    }), NULL));
                    
                } else if (_killCount >= 50 && _killCount <= 100) {
                    // 中分
                    fullSrar1->runAction(Sequence::create(ScaleTo::create(0.15, 1.1), ScaleTo::create(0.1, 0.9), ScaleTo::create(0.09, 1.05), ScaleTo::create(0.08, 0.95), ScaleTo::create(0.08, 1), CallFunc::create([=](){
                        fullSrar2->runAction(Sequence::create(ScaleTo::create(0.15, 1.1), ScaleTo::create(0.1, 0.9), ScaleTo::create(0.09, 1.05), ScaleTo::create(0.08, 0.95), ScaleTo::create(0.08, 1), CallFunc::create([=](){
                            this->finishAnimationFinish();
                        }),  DelayTime::create(1.2), CallFunc::create([=](){
                            
                            btn1->runAction(MoveTo::create(0.3, Vec2(btn1->getPosition().x, kWinSizeHeight * 0.12)));
                            btn2->runAction(MoveTo::create(0.3, Vec2(btn2->getPosition().x, kWinSizeHeight * 0.12)));
                            btn3->runAction(MoveTo::create(0.3, Vec2(btn3->getPosition().x, kWinSizeHeight * 0.12)));
                            btn4->runAction(MoveTo::create(0.3, Vec2(btn4->getPosition().x, kWinSizeHeight * 0.12)));
                        }), NULL));
                    }), NULL));
                } else {
                    // 低分
                    fullSrar1->runAction(Sequence::create(ScaleTo::create(0.15, 1.1), ScaleTo::create(0.1, 0.9), ScaleTo::create(0.09, 1.05), ScaleTo::create(0.08, 0.95), ScaleTo::create(0.08, 1), CallFunc::create([=](){
                        this->finishAnimationFinish();
                    }), DelayTime::create(1.2), CallFunc::create([=](){
                        
                        btn1->runAction(MoveTo::create(0.3, Vec2(btn1->getPosition().x, kWinSizeHeight * 0.12)));
                        btn2->runAction(MoveTo::create(0.3, Vec2(btn2->getPosition().x, kWinSizeHeight * 0.12)));
                        btn3->runAction(MoveTo::create(0.3, Vec2(btn3->getPosition().x, kWinSizeHeight * 0.12)));
                        btn4->runAction(MoveTo::create(0.3, Vec2(btn4->getPosition().x, kWinSizeHeight * 0.12)));
                    }), NULL));
                }
                
                
            }), NULL));
        }), NULL));
        
    }), NULL));
}

void GameControl::finishAnimationFinish()
{
    CCLOG("完成");
}

void GameControl::btnClick(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED) {
        switch (((Button *)sender)->getTag()) {
            case 444:
                Director::getInstance()->replaceScene(Home::createScene());
                break;
            case 445:
                this->showShopCart();
                break;
            case 446:
                /// 重新开始
                this->replayGame();
                break;
            case 447:
                CCLOG("分享");
                break;
            default:
                break;
        }
    }
}

/// 重新开始
void GameControl::replayGame()
{
    auto selectPropLayer = SelectGamePropLayer::create(_gameType);
    addChild(selectPropLayer);
}

/// 显示购物车
void GameControl::showShopCart()
{
    auto shopLayer = ShopCartLayer::create();
    addChild(shopLayer);
}

void GameControl::stopClocking()
{
    unschedule(schedule_selector(GameControl::update));
}

void GameControl::update(float dt)
{
    _time--;
    _lifeLabel->setString(to_string(_time));
    
    if (_time == 0) {
        unschedule(schedule_selector(GameControl::update));
        
        // 时间到了
        EventCustom event("TimeOver");
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
}

GameControl *GameControl::create(GameType type, GamePropModel *model)
{
    auto game = new GameControl();
    if (game && game->init(type, model)) {
        game->autorelease();
    } else {
        delete game;
        game = nullptr;
    }
    
    return game;
}

void GameControl::hidePauseLayer()
{
    auto csb = getChildByTag(150);
    csb->removeFromParent();
}

void GameControl::showGamePauseLayer()
{
    auto csb = CSLoader::createNode("GamePauseLayer.csb");
    csb->setTag(150);
    csb->setOpacity(0);
    this->addChild(csb);
    
    csb->runAction(FadeTo::create(0.25, 255));
    
    Layout *cover = (Layout *)csb->getChildByTag(8);
    for (int i = 20; i < 24; i++) {
        Button *btn = (Button *)cover->getChildByTag(i);
        
        if (i == 22 || i == 23) {
            
            string imageName;
            if (i == 22) {
                
                if (UserDataManager::getInstance()->getSoundMute()) {
                    imageName = "sound_off.png";
                } else {
                    imageName = "sound_on.png";
                }
            } else {
                if (UserDataManager::getInstance()->getMusicMute()) {
                    imageName = "music_off.png";
                } else {
                    imageName = "music_on.png";
                }
            }
            
            btn->loadTextures(imageName, imageName, "", TextureResType::PLIST);
        }
        
        btn->addTouchEventListener([this](Ref *ref,  Widget::TouchEventType type){
            if (type == Widget::TouchEventType::ENDED) {
                switch (((Button *)ref)->getTag()) {
                    case 20:
                    {
                        EventCustom event("GoHome");
                        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
                    }
                        break;
                    case 21:
                    {
                        EventCustom event("GameResume");
                        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
                        
                        hidePauseLayer();
                    }
                        break;
                    case 22:
                        UserDataManager::getInstance()->setSoundMute(!UserDataManager::getInstance()->getSoundMute());
                    {
                        string imageName;
                        if (UserDataManager::getInstance()->getSoundMute()) {
                            imageName = "sound_off.png";
                        } else {
                            imageName = "sound_on.png";
                        }
                        ((Button *)ref)->loadTextures(imageName, imageName, "", TextureResType::PLIST);
                    }
                        break;
                    case 23:
                        UserDataManager::getInstance()->setMusicMute(!UserDataManager::getInstance()->getMusicMute());
                    {
                        string imageName;
                        
                        if (UserDataManager::getInstance()->getMusicMute()) {
                            imageName = "music_off.png";
                        } else {
                            imageName = "music_on.png";
                        }
                        ((Button *)ref)->loadTextures(imageName, imageName, "", TextureResType::PLIST);
                    }
                        break;
                    default:
                        break;
                }
            }
        });
    }
}
