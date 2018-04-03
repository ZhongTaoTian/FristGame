//
//  SelectGamePropLayer.cpp
//  FristGame
//
//  Created by sfbest on 2017/12/13.
//
//

#include "SelectGamePropLayer.hpp"
#include "GameScene.hpp"
#include "ShopCartLayer.hpp"
#include "WaitScene.hpp"

bool SelectGamePropLayer::init(GameType type)
{
    Layer::init();
    
    auto csb = CSLoader::createNode("Prop.csb");
    addChild(csb);
    this->setOpacity(0);
    
    _model = new GamePropModel();
    
    _type = type;
    
    _userGoldCountBtn = (Button *)csb->getChildByTag(101);
    _userGoldCountBtn->setTitleText(to_string(UserDataManager::getInstance()->getUserGoldCount()));
    _userGoldCountBtn->addTouchEventListener(CC_CALLBACK_2(SelectGamePropLayer::propButtonClick, this));
    
    auto prop = csb->getChildByTag(5);
    prop->setScale(0);
    _propDes = (Sprite *)prop->getChildByTag(200);
    
    string timeNormalImages[3] = {"time_new_30_normal.png", "time_new_60_normal.png", "time_new_90_normal.png"};
    string timeLightImages[3] = {"time_new_30_hightLight.png", "time_new_60_hightLight.png", "time_new_90_hightLight.png"};
    
    string guardNormalImages[3] = {"life_new_2_normal.png", "life_new_4_normal.png", "life_new_6_normal.png"};
    string guardLightImages[3] = {"life_new_2_hightLight.png", "life_new_4_hightLight.png", "life_new_6_hightLight.png"};
    
    for (int i = 102; i < 110; i++) {
        Button *btn = (Button *)(prop->getChildByTag(i));
        btn->addTouchEventListener(CC_CALLBACK_2(SelectGamePropLayer::propButtonClick, this));
        if (i >= 106 && i <= 108) {
            string normal;
            string press;
            
            if (type == GameType::Time) {
                normal = timeNormalImages[i - 106];
                press = timeLightImages[i - 106];
            } else {
                normal = guardNormalImages[i - 106];
                press = guardLightImages[i - 106];
            }
            
            btn->loadTextures(normal, press, "", TextureResType::PLIST);
        }
        
        if (i >= 103 && i <= 108) {
            btn->setHighlighted(false);
        }
    }
    
    csb->runAction(Sequence::create(FadeTo::create(0.1, 255), CallFunc::create([=](){
        prop->runAction(ScaleTo::create(0.15, 1.4));
    }), NULL));
    
    _buyBG = (Layout *)(csb->getChildByTag(300));
    _buyTipsSprite = (Sprite *)(csb->getChildByTag(301));
    
    for (int i = 110; i < 112; i++) {
        Button *btn = (Button *)(_buyTipsSprite->getChildByTag(i));
        btn->addTouchEventListener(CC_CALLBACK_2(SelectGamePropLayer::propButtonClick, this));
    }
    
    return true;
}

SelectGamePropLayer * SelectGamePropLayer::create(GameType type)
{
    auto layer = new SelectGamePropLayer();
    if (layer && layer->init(type)) {
        layer->autorelease();
    } else {
        delete layer;
        layer = nullptr;
    }
    
    return layer;
}

void SelectGamePropLayer::showShopingCar()
{
    auto shopLayer = ShopCartLayer::create();
    this->addChild(shopLayer);
}

bool SelectGamePropLayer::clickProp(int gold, bool isAdd)
{
    if (isAdd) {
        _buyCount += gold;
        if (UserDataManager::getInstance()->getUserGoldCount() - _buyCount >= 0) {
            _userGoldCountBtn->setTitleText(to_string(UserDataManager::getInstance()->getUserGoldCount() - _buyCount));
            return true;
        } else {
            _buyCount -= gold;
            _buyBG->setVisible(true);
            _buyTipsSprite->setVisible(true);
            return false;
        }
        
    } else {
        _buyCount -= gold;
        _userGoldCountBtn->setTitleText(to_string(UserDataManager::getInstance()->getUserGoldCount() - _buyCount));
    }
    
    return true;
}

void SelectGamePropLayer::propButtonClick(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED) {
        int tag = ((Button *)sender)->getTag();
        Button *btn = (Button *)sender;
        
        switch (tag) {
            case 101:
                showShopingCar();
                break;
                
            case 102:
                // 取消
                this->removeFromParent();
                break;
            case 103:
                _propDes->setSpriteFrame(timePropDes[0]);
                if (clickProp(kDoubleGoldPrice, !_model->hasDoubleGold)) {
                    btn->setHighlighted(!_model->hasDoubleGold);
                    _model->hasDoubleGold = !_model->hasDoubleGold;
                }
                
                break;
            case 104:
                _propDes->setSpriteFrame(timePropDes[1]);
                
                if (clickProp(kDecelerationPrice, !_model->hasDeceleration)) {
                    btn->setHighlighted(!_model->hasDeceleration);
                    _model->hasDeceleration = !_model->hasDeceleration;
                }
                
                break;
            case 105:
                _propDes->setSpriteFrame(timePropDes[2]);
                if (clickProp(kBombPrice, !_model->hasBomb)) {
                    btn->setHighlighted(!_model->hasBomb);
                    _model->hasBomb = !_model->hasBomb;
                }
                
                break;
            case 106:
                _propDes->setSpriteFrame(_type == GameType::Time ? timePropDes[3] : guardPropDes[3]);
                if (clickProp(kAdd1Price, !_model->hasAdd1)) {
                    btn->setHighlighted(!_model->hasAdd1);
                    _model->hasAdd1 = !_model->hasAdd1;
                }
                
                break;
            case 107:
                _propDes->setSpriteFrame(_type == GameType::Time ? timePropDes[4] : guardPropDes[4]);
                if (clickProp(kAdd2Price, !_model->hasAdd2)) {
                    btn->setHighlighted(!_model->hasAdd2);
                    _model->hasAdd2 = !_model->hasAdd2;
                }
                
                break;
            case 108:
                _propDes->setSpriteFrame(_type == GameType::Time ? timePropDes[5] : guardPropDes[5]);
                if (clickProp(kAdd3Price, !_model->hasAdd3)) {
                    btn->setHighlighted(!_model->hasAdd3);
                    _model->hasAdd3 = !_model->hasAdd3;
                }
                break;
            case 109:
                // 开始游戏
            {
                
                if (_buyCount != 0) {
                    // 减去金币
                    UserDataManager::getInstance()->addUserGoldCount(-_buyCount);
                }
                
                auto wait = WaitScene::createScene(_type, _model);
                Director::getInstance()->replaceScene(wait);
                
            }
                break;
            case 110:
                // 取消提示
                _buyTipsSprite->setVisible(false);
                _buyBG->setVisible(false);
                break;
            case 111:
                _buyTipsSprite->setVisible(false);
                _buyBG->setVisible(false);
                showShopingCar();
                break;
            default:
                break;
        }
    }
}
