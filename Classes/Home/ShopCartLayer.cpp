//
//  ShopCartLayer.cpp
//  FristGame
//
//  Created by sfbest on 2017/12/13.
//
//

#include "ShopCartLayer.hpp"

bool ShopCartLayer::init()
{
    Layer::init();
    
    auto shopCsb = CSLoader::createNode("ShopCart.csb");
    Layout *shopCartLayout = (Layout *)shopCsb->getChildByTag(205);
    this->addChild(shopCartLayout);
    
    
    for (int i = 171; i <= 176; i++) {
        // 添加购买按钮
        Button *btn = (Button *)shopCartLayout->getChildByTag(i);
        btn->addTouchEventListener(CC_CALLBACK_2(ShopCartLayer::buttonClick, this));
    }
    
    return true;
}

void ShopCartLayer::buttonClick(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED) {
        if (((Button *)sender)->getTag() == 176) {
            removeFromParent();
        } else {
            buyGoldWithTag(((Button *)sender)->getTag());
        }
    }
}

void ShopCartLayer::buyGoldWithTag(int tag)
{
    
}
