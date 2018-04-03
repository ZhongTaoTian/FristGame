//
//  Const.hpp
//
//  Created by 维尼的小熊 on 2017/2/8.

#ifndef Const_hpp
#define Const_hpp

#include <stdio.h>
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Help.hpp"
#include "UserDataManager.hpp"

using namespace cocos2d::ui;

USING_NS_CC;
using namespace std;

enum GameType
{
    Time = 0,
    Guard
};

#define PI 3.1415926
#define kWinSizeWidth Director::getInstance()->getVisibleSize().width
#define kWinSizeHeight Director::getInstance()->getVisibleSize().height
#define kWinSize Director::getInstance()->getVisibleSize()
#define ABS(A) (A >= 0 ? A : -A)

// prop price
#define kDoubleGoldPrice 100
#define kDecelerationPrice 100
#define kBombPrice 200
#define kAdd1Price 100
#define kAdd2Price 200
#define kAdd3Price 300

#endif /* Const_hpp */
