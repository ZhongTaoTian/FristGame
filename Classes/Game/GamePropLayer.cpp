//
//  GamePropLayer.cpp
//  FristGame
//
//  Created by sfbest on 2017/11/29.
//
//

#include "GamePropLayer.hpp"


bool GamePropLayer::init(GameType gameType, GamePropModel *model)
{
    Layer::init();
    _gameProp = model;
    _gameType = gameType;
    
    auto csb = CSLoader::createNode("GameProp.csb");
    addChild(csb);
    
    _decelerationSp = (Sprite *)csb->getChildByTag(102);
    _bombSp = (Sprite *)csb->getChildByTag(103);
    _goldSp = (Sprite *)csb->getChildByTag(104);
    
    for (int i = 100; i < 102; i++) {
        Button *btn = (Button *)csb->getChildByTag(i);
        btn->addTouchEventListener(CC_CALLBACK_2(GamePropLayer::propClick, this));
        
        if (i == 100) {
            _decelerationBtn = btn;
        } else if (i == 101) {
            _bombBtn = btn;
        }
    }
    
    setDecelerationCount(model->hasDeceleration ? 1 : 0);
    setBombCount(model->hasBomb ? 1 : 0);
    setGoldCount(model->hasDoubleGold ? 1 : 0);
    
    return true;
}

GamePropLayer * GamePropLayer::create(GameType gameType, GamePropModel *model)
{
    auto layer = new GamePropLayer();
    if (layer && layer->init(gameType, model)) {
        layer->autorelease();
    } else {
        delete layer;
        layer = nullptr;
    }
    
    return layer;
}

void GamePropLayer::propClick(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED) {
        if (((Button *)sender)->getTag() == 100) {
            if (_decelerationCount > 0) {
                setDecelerationCount(_decelerationCount - 1);
                EventCustom e("PropEventName");
                e.setUserData((void *)"Deceleration");
                Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
            } else {
                CCLOG("数量不够啊");
            }
        } else {
            
            if (_bombCount > 0) {
                setBombCount(_bombCount - 1);
                EventCustom e("PropEventName");
                e.setUserData((void *)"Bomb");
                Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
            } else {
                CCLOG("炸弹数量不够");
            }
        }
    }
}

void GamePropLayer::addBobm()
{
    // 增加炸弹
    _bombBtn->runAction(Sequence::create(ScaleTo::create(0.1, 1.15), ScaleTo::create(0.1, 1), ScaleTo::create(0.1, 1.1), ScaleTo::create(0.1, 1), NULL));
    setBombCount((_bombCount + 1));
}

void GamePropLayer::addDeceleration()
{
    // 增加减速
    _decelerationBtn->runAction(Sequence::create(ScaleTo::create(0.1, 1.15), ScaleTo::create(0.1, 1), ScaleTo::create(0.1, 1.1), ScaleTo::create(0.1, 1), NULL));
    setDecelerationCount(_decelerationCount + 1);
}

void GamePropLayer::setDecelerationCount(int var)
{
    if (var > 0) {
        _decelerationCount = var;
        _decelerationSp->setVisible(true);
        
        string name = "multiple_x4_icon.png";
        switch (var) {
            case 1:
                name = "multiple_x1_icon.png";
                break;
            case 2:
                name = "multiple_x2_icon.png";
                break;
            case 3:
                name = "multiple_x3_icon.png";
                break;
                
            default:
                break;
        }
        
        _decelerationSp->setTexture(name);
    } else {
        _decelerationSp->setVisible(false);
        _decelerationCount = 0;
    }
}

void GamePropLayer::setBombCount(int var)
{
    if (var > 0) {
        _bombSp->setVisible(true);
        _bombCount = var;
        string name = "multiple_x4_icon.png";
        switch (var) {
            case 1:
                name = "multiple_x1_icon.png";
                break;
            case 2:
                name = "multiple_x2_icon.png";
                break;
            case 3:
                name = "multiple_x3_icon.png";
                break;
                
            default:
                break;
        }
        
        _bombSp->setTexture(name);
    } else {
        _bombCount = 0;
        _bombSp->setVisible(false);
    }
}

void GamePropLayer::setGoldCount(int var)
{
    string name = "multiple_x1_icon.png";
    switch (var) {
        case 1:
            name = "multiple_x2_icon.png";
            break;
        case 2:
            name = "multiple_x3_icon.png";
            break;
        case 3:
            name = "multiple_x4_icon.png";
            break;
            
        default:
            break;
    }
    _goldSp->setTexture(name);
}
