//
//  HomeScene.cpp
//  FristGame
//
//  Created by sfbest on 2017/4/14.
//
//

#include "HomeScene.hpp"
#include "GameScene.hpp"
#include "UserDataManager.hpp"
#include "GamePropModel.hpp"
#include "SelectGamePropLayer.hpp"
#include "ShopCartLayer.hpp"


USING_NS_CC;

Scene *Home::createScene()
{
    auto homeScene = Scene::create();
    auto homeLayer = Home::create();
    homeScene->addChild(homeLayer);
    
    return homeScene;
}

bool Home::init()
{
    if (!Layer::init()) return false;
    
    // 加载图片资源
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("start_home.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("prop_image.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("prop_icon.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("pauseGame.plist");
    
    auto csb = CSLoader::createNode("MainScene.csb");
    addChild(csb);
    
    
    // 初始化属性
    Layout *layout = (Layout *)csb->getChildByTag(4);
    _yellowMonster = (Sprite *)layout->getChildByTag(15);
    _eyeSprite = (Sprite *)layout->getChildByTag(12);
    _bigMonster = (Sprite *)layout->getChildByTag(13);
    _blueMonster = (Sprite *)layout->getChildByTag(14);
        
    // 初始化首页按钮
    for (int i = 101; i < 113; i++) {
        Button *btn = (Button *)layout->getChildByTag(i);
        btn->addTouchEventListener(CC_CALLBACK_2(Home::buttonClickAction, this));
    }
    
    // add Action
    auto listener1 = EventListenerTouchOneByOne::create();
    listener1->setSwallowTouches(true);
    
    listener1->onTouchBegan = [this](Touch* touch, Event* event){
        auto target = (Sprite *)event->getCurrentTarget();
        
        cocos2d::Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
        cocos2d::Size s = target->getContentSize();
        cocos2d::Rect rect = cocos2d::Rect(0, 0, s.width, s.height);
        
        if (rect.containsPoint(locationInNode)) {
            switch (target->getTag()) {
                case 13:
                    this->playBigMonsnterClickAnimation();
                    return true;
                case 14:
                    this->playBlueMonsterClickAnimation();
                    return true;
                case 15:
                    this->playYellowMosnterClickAnimation();
                    return true;
                default:
                    break;
            }
        }
        
        return false;
    };
    
    _startBtn = (Button *)layout->getChildByTag(103);
    _backBtn = (Button *)layout->getChildByTag(110);
    _soundBtn = (Button *)layout->getChildByTag(106);
    _musicBtn = (Button *)layout->getChildByTag(107);
    _blogBtn = (Button *)layout->getChildByTag(109);
    _likeBtn = (Button *)layout->getChildByTag(108);
    _guardBtn = (Button *)layout->getChildByTag(112);
    _timeBtn = (Button *)layout->getChildByTag(111);
    
    // 获取设置状态
    updateSettingBtnStatus(false);
    updateSettingBtnStatus(true);
    
    // add Action
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, _yellowMonster);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1->clone(), _blueMonster);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1->clone(), _bigMonster);
    
    return true;
}

// 进入
void Home::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    if (!_isLoading) {
        // 首次进入 开启默认动画
        playEyeShakeAnimation();
        
        playYellowMonsterNormalAnimation();
        
        _isLoading = true;
    }
}

void Home::playEyeShakeAnimation()
{
    if (!_eyeSprite->isVisible()) {
        _eyeSprite->setVisible(true);
    }
    
    _eyeSprite->stopAllActions();
    
    // 眼睛动画
    Animate *eyeAnim = Help::getAnimate("eye", 4, 0.5);
    Animate *blinkAnim = Help::getAnimate("eye_blink", 5, 0.15);
    _eyeSprite->runAction(RepeatForever::create(Sequence::create(DelayTime::create(7), eyeAnim, blinkAnim, NULL)));
}

void Home::playYellowMonsterNormalAnimation()
{
    // 黄色Monster 动画
    auto yellowNormalAnim = Help::getAnimate("yellow_nomarl", 6, 0.2);
    _yellowMonster->runAction(RepeatForever::create(Sequence::create(DelayTime::create(8), yellowNormalAnim, yellowNormalAnim, NULL)));
}

void Home::playBigMonsnterClickAnimation()
{
    if (_bigIsAnimation) {
        return;
    }
    
    _bigIsAnimation = true;
    Animate *anim = Help::getAnimate("blue", 12, 0.15);
    
    _bigMonster->runAction( Sequence::create(anim, CallFunc::create([this](){
        _bigIsAnimation = false;
    }), NULL));
}

void Home::showAdView(bool isFull)
{
    [[NSNotificationCenter defaultCenter] postNotificationName:@"showVideoNotification" object:nil];
}

void Home::buttonClickAction(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED) {
        Button *btn = (Button *)sender;
        switch (btn->getTag()) {
            case 101:
                CCLOG("分享");
                break;
            case 102:
                showShopingCar();
                break;
            case 103:
                btn->setVisible(false);
                _backBtn->setVisible(true);
                _timeBtn->setPosition(Vec2(_timeBtn->getPosition().x, kWinSizeHeight * 0.552));
                _guardBtn->cocos2d::Node::setPosition(Vec2(_guardBtn->getPosition().x, kWinSizeHeight * 0.552));
                _timeBtn->setVisible(true);
                _guardBtn->setVisible(true);
                
                // 开启下滑动画 45.37  35.74    55.2
            {
                auto move1 = MoveTo::create(0.1, Vec2(_timeBtn->getPosition().x, kWinSizeHeight * 0.4537));
                auto move2 = MoveTo::create(0.2, Vec2(_guardBtn->getPosition().x, kWinSizeHeight * 0.3574));
                _timeBtn->stopAllActions();
                _guardBtn->stopAllActions();
                _timeBtn->runAction(move1);
                _guardBtn->runAction(move2);
            }
                
                break;
            case 104:
                // 设置
                _settingSelected = !_settingSelected;
                _musicBtn->stopAllActions();
                _soundBtn->stopAllActions();
                
                if (!_settingSelected) { // 35.8  45.4 55.44
                    _musicBtn->setPosition(Vec2(_musicBtn->getPosition().x, kWinSizeHeight * 0.358));
                    _soundBtn->setPosition(Vec2(_soundBtn->getPosition().x, kWinSizeHeight * 0.454));
                    
                    auto move1 = MoveTo::create(0.1, Vec2(_musicBtn->getPosition().x, kWinSizeHeight * 0.5544));
                    auto move2 = MoveTo::create(0.2, Vec2(_soundBtn->getPosition().x, kWinSizeHeight * 0.5544));
                    
                    _musicBtn->runAction(Sequence::create(move1, CallFuncN::create([](Node *node){
                        node->setVisible(false);
                    }), NULL));
                    _soundBtn->runAction(Sequence::create(move2, CallFuncN::create([](Node *node){
                        node->setVisible(false);
                    }), NULL));
                    
                } else {
                    _musicBtn->setPosition(Vec2(_musicBtn->getPosition().x, kWinSizeHeight * 0.5544));
                    _soundBtn->setPosition(Vec2(_soundBtn->getPosition().x, kWinSizeHeight * 0.5544));
                    _soundBtn->setVisible(true);
                    _musicBtn->setVisible(true);
                    auto move2 = MoveTo::create(0.2 ,Vec2(_musicBtn->getPosition().x, kWinSizeHeight * 0.358));
                    auto move1 = MoveTo::create(0.1, Vec2(_soundBtn->getPosition().x, kWinSizeHeight * 0.454));
                    _soundBtn->runAction(move1);
                    _musicBtn->runAction(move2);
                }
                
                break;
            case 105:
                // 喜欢
                _messageSelected = !_messageSelected;
                _likeBtn->stopAllActions();
                _blogBtn->stopAllActions();
                
                if (_messageSelected) {
                    _likeBtn->setPosition(Vec2(_likeBtn->getPosition().x, kWinSizeHeight * 0.5494));
                    _blogBtn->setPosition(Vec2(_blogBtn->getPosition().x, kWinSizeHeight * 0.5494));
                    
                    _likeBtn->setVisible(true);
                    _blogBtn->setVisible(true);
                    
                    auto move1 = MoveTo::create(0.1, Vec2(_likeBtn->getPosition().x, kWinSizeHeight * 0.454));
                    auto move2 = MoveTo::create(0.2, Vec2(_blogBtn->getPosition().x, kWinSizeHeight * 0.358));
                    _likeBtn->runAction(move1);
                    _blogBtn->runAction(move2);
                    
                } else {
                    _likeBtn->setPosition(Vec2(_likeBtn->getPosition().x, kWinSizeHeight * 0.454));
                    _blogBtn->setPosition(Vec2(_blogBtn->getPosition().x, kWinSizeHeight * 0.358));
                    auto move1 = MoveTo::create(0.1, Vec2(_likeBtn->getPosition().x, kWinSizeHeight * 0.5494));
                    auto move2 = MoveTo::create(0.2, Vec2(_blogBtn->getPosition().x, kWinSizeHeight * 0.5494));
                    
                    _likeBtn->runAction(Sequence::create(move1, CallFuncN::create([](Node *node){
                        node->setVisible(false);
                    }), NULL));
                    _blogBtn->runAction(Sequence::create(move2, CallFuncN::create([](Node *node){
                        node->setVisible(false);
                    }), NULL));
                }
                
                break;
            case 106:
                // 音效
                UserDataManager::getInstance()->setSoundMute(!UserDataManager::getInstance()->getSoundMute());
                updateSettingBtnStatus(true);
                break;
            case 107:
                // 音乐
                UserDataManager::getInstance()->setMusicMute(!UserDataManager::getInstance()->getMusicMute());
                updateSettingBtnStatus(false);
                break;
            case 108:
                CCLOG("%d", btn->getTag());
                break;
            case 109:
                CCLOG("%d", btn->getTag());
                break;
            case 110:
                // 返回
                btn->setVisible(false);
                _startBtn->setVisible(true);
                
                _timeBtn->stopAllActions();
                _guardBtn->stopAllActions();
                _timeBtn->setPosition(Vec2(_timeBtn->getPosition().x, kWinSizeHeight * 0.4537));
                _guardBtn->cocos2d::Node::setPosition(Vec2(_guardBtn->getPosition().x, kWinSizeHeight * 0.3574));
            {
                auto move1 = MoveTo::create(0.1, Vec2(_timeBtn->getPosition().x, kWinSizeHeight * 0.552));
                auto move2 = MoveTo::create(0.2, Vec2(_guardBtn->getPosition().x, kWinSizeHeight * 0.552));
                _timeBtn->runAction(Sequence::create(move1, CallFuncN::create([](Node *node){
                    node->setVisible(false);
                }), NULL));
                _guardBtn->runAction(Sequence::create(move2, CallFuncN::create([](Node *node){
                    node->setVisible(false);
                }), NULL));
            }
                
                break;
            case 111:
                // 限时模式
                showPropView(true);
                break;
            case 112:
                // 守护模式
                showPropView(false);
                break;
            default:
                break;
        }
    }
}

// 显示道具
void Home::showPropView(bool isTimeModel)
{
    auto selectPropLayer = SelectGamePropLayer::create(isTimeModel ? GameType::Time : GameType::Guard);
    addChild(selectPropLayer);
}

// 显示购物车列表
void Home::showShopingCar()
{
    auto shopCart = ShopCartLayer::create();
    addChild(shopCart);
}

void Home::playYellowMosnterClickAnimation()
{
    if (_yellowIsAnimation) {
        return;
    }
    
    _yellowIsAnimation = true;
    _yellowMonster->stopAllActions();
    
    auto anima = Help::getAnimate("yellow", 6, 0.1);
    _yellowMonster->runAction(Sequence::create(anima, CallFunc::create([this](){
        this->playYellowMonsterNormalAnimation();
        _yellowIsAnimation = false;
    }), NULL));
}

void Home::playBlueMonsterClickAnimation()
{
    if (_blueIsAnimation) {
        return;
    }
    _blueIsAnimation = true;
    _eyeSprite->setVisible(false);
    
    auto anim = Help::getAnimate("blue_click", 3, 0.1);
    auto anim2 = Help::getAnimate(3, "blue_click", 6, 0.2);
    auto anim3 = Help::getAnimate(5, "blue_click", 7, 0.3);
    auto anim4 = Help::getAnimate(7, "blue_click", 9, 0.2);
    auto w = Help::getAnimate(3, "blue_click", 4, 1.5);
    _blueMonster->runAction(Sequence::create(anim, w, anim2, anim3, anim3, anim3, anim4, CallFunc::create([this](){
        this->playEyeShakeAnimation();
        _blueIsAnimation = false;
    }), NULL));
}

void Home::updateSettingBtnStatus(bool isSound)
{
    string normal;
    string press;
    
    if (isSound) {
        if (UserDataManager::getInstance()->getSoundMute()) {
            normal = "sound_hight.png";
            press = "sound_hight.png";
        } else {
            normal = "sound.png";
            press = "sound.png";
        }
        
        _soundBtn->loadTextures(normal, press, "", TextureResType::PLIST);
    } else {
        if (UserDataManager::getInstance()->getMusicMute()) {
            normal = "music_hight.png";
            press = "music_hight.png";
        } else {
            normal = "music.png";
            press = "music.png";
        }
        
        _musicBtn->loadTextures(normal, press, "", TextureResType::PLIST);
    }
}

