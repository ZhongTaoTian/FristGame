//
//  Monster.cpp
//  FristGame
//
//  Created by sfbest on 2017/4/19.
//

#include "Monster.hpp"

#define kBalloonToMonsterDis 100

bool Monster::init(MonsterBalloonType type)
{
    Layer::init();
    _type = type;
    
    string name;
    // 随机生成Monster
    if (type != More) {
        // 正常monster(随机生成)
        _monsterType = MonsterType(arc4random_uniform(4));
        
        switch (_monsterType) {
            case YellowMonster:
                name = "yellow_fly_0.png";
                break;
            case BlueMonster:
                name = "blue_fly_0.png";
                break;
            case GreenMonster:
                name = "green_fly_0.png";
                break;
            case ShellMonster:
                name = "iron_fly_0.png";
                break;
            default:
                break;
        }
    }
    
    _monster = Sprite::createWithSpriteFrameName(name);
    
    _monster->setAnchorPoint(Vec2(0.5, 0));
    float scale = 0.8;
    if (_monsterType == BlueMonster) {
        scale = 1.2;
    }
    _monster->setScale(scale);
    _monster->setContentSize(Size(_monster->getContentSize().width * scale, _monster->getContentSize().height * scale));
    _monster->setPosition(Vec2(_monster->getContentSize().width * 0.5 * scale, 0));
    this->addChild(_monster, 2);
    
    vector<BalloonType> types;
    // 气球1
    auto balloon = Balloon::createRandomWithLineHeight(kBalloonToMonsterDis, types);
    balloon->setAnchorPoint(Vec2(0.5, 0));
    balloon->setTag(10);
    this->addChild(balloon);
    _balloons.pushBack(balloon);
    
    if (type != OneBalloon) {
        // 气球2
        types.push_back(balloon->getType());
        auto balloon2 = Balloon::createRandomWithLineHeight(kBalloonToMonsterDis, types);
        balloon2->setAnchorPoint(Vec2(0.5, 0));
        balloon2->setTag(11);
        this->addChild(balloon2);
        _balloons.pushBack(balloon2);
        
        if (type == ThreeBalloon) {
            // 气球三
            types.push_back(balloon2->getType());
            auto balloon3 = Balloon::createRandomWithLineHeight(kBalloonToMonsterDis, types);
            balloon3->setAnchorPoint(Vec2(0.5, 0));
            balloon3->setTag(12);
            addChild(balloon3);
            _balloons.pushBack(balloon3);
        }
    }
    
    float allWidth = _monster->getContentSize().width;
    
    switch (type) {
        case OneBalloon:
              balloon->setPosition(Vec2((_monster->getContentSize().width - balloon->getContentSize().width) * 0.5, _monster->getContentSize().height - 30));
            break;
        case TwoBalloon:
        {
            auto balloon2 = _balloons.at(1);
            balloon->setRotation(15);
            balloon2->setRotation(-15);
            balloon->setPosition(Vec2((_monster->getContentSize().width - balloon->getContentSize().width) * 0.5 + 17, _monster->getContentSize().height - 30));
            balloon2->setPosition(Vec2((_monster->getContentSize().width - balloon->getContentSize().width) * 0.5 - 17, _monster->getContentSize().height - 30));
        }
            break;
        case ThreeBalloon:
        {
            auto balloon2 = _balloons.at(1);
            auto balloon3 = _balloons.at(2);
            balloon2->setRotation(30);
            balloon->setRotation(-30);
            balloon3->setLineHeight(kBalloonToMonsterDis + 40, false);
            balloon->setPosition(Vec2((_monster->getContentSize().width - balloon->getContentSize().width) * 0.5 - 17, _monster->getContentSize().height - 30));
            balloon2->setPosition(Vec2((_monster->getContentSize().width - balloon->getContentSize().width) * 0.5 + 17, _monster->getContentSize().height - 30));
            balloon3->setPosition(Vec2((_monster->getContentSize().width - balloon->getContentSize().width) * 0.5, _monster->getContentSize().height - 30));
        }
            break;
        default:
            break;
    }
    
    this->setContentSize(Size(allWidth, _monster->getContentSize().height + balloon->getContentSize().height));
    
    return true;
}

void Monster::dieAnimation()
{
    Animate *anim = nullptr;
    
    switch (_monsterType) {
        case YellowMonster:
            anim = Help::getAnimate(0, "yellow_die", 2, 0.1, false);
            break;
        case BlueMonster:
            anim = Help::getAnimate(0, "blue_die", 2, 0.1, false);
            break;
        case GreenMonster:
            anim = Help::getAnimate(0, "green_die", 2, 0.1, false);
            break;
        case ShellMonster:
            anim = Help::getAnimate(0, "purple_die", 2, 0.1, false);
            break;
        default:
            break;
    }
    
    _monster->runAction(Sequence::create(anim, FadeOut::create(0.2),  CallFuncN::create([](Node *node){
        node->removeFromParent();
    }), NULL));

    // 粒子爆炸效果
    blowUpAnimation(_monster->getPosition());
}

void Monster::bombAnim()
{
    //爆炸效果
    for (int i = (int)_balloons.size() - 1; i >= 0; i--) {
        auto balloon = _balloons.at(i);
        // todo(爆炸动画),
        balloon->bobmAnimationWithFinish();
    }
    
    dieAnimation();
}

Monster * Monster::create(MonsterBalloonType type)
{
    auto monster = new Monster();
    if (monster && monster->init(type)) {
        monster->autorelease();
    } else {
        delete monster;
        monster = nullptr;
    }
    
    return monster;
}

// 移除对应类型的气球
void Monster::removeBalloon(BalloonType type)
{
    for (int i = (int)_balloons.size() - 1; i >= 0; i--) {
        auto balloon = _balloons.at(i);
        if (balloon->getType() == type) {
            // 俩种类型相同移除掉气球
            _balloons.eraseObject(balloon);
            // todo(爆炸动画),
            balloon->bobmAnimationWithFinish();
            
            // 如果有多个气球,改变剩余气球位置
            if (_balloons.size() > 0) {
                changeBalloonAnimation();
            }
        }
    }
    
    if (_balloons.size() == 0) {
        // 怪物身上没有气球了
        CCLOG("销毁怪物");
    }
}

void Monster::blowUpAnimation(Vec2 position)
{
    //爆炸效果
    
}

// 执行改变气球位置动画
void Monster::changeBalloonAnimation()
{
    if (_balloons.size() == 2) {
        bool re = false;
        for (int i = 0; i < _balloons.size(); i++) {
            auto balloon = _balloons.at(i);
            balloon->setLineHeight(kBalloonToMonsterDis, false);
            balloon->stopAllActions();
            if (i == 0) {
                if (balloon->getTag() == 10) {
                    balloon->runAction(MoveTo::create(0.1, Vec2((_monster->getContentSize().width - balloon->getContentSize().width) * 0.5 - 17, _monster->getContentSize().height - 30)));
                    balloon->setRotation(-15);
                    re = true;
                } else {
                    balloon->runAction(MoveTo::create(0.1, Vec2((_monster->getContentSize().width - balloon->getContentSize().width) * 0.5 + 17, _monster->getContentSize().height - 30)));
                    balloon->setRotation(15);
                }
            } else {
                if (re) {
                    balloon->runAction(MoveTo::create(0.1, Vec2((_monster->getContentSize().width - balloon->getContentSize().width) * 0.5 + 17, _monster->getContentSize().height - 30)));
                    balloon->setRotation(15);
                } else {
                    balloon->runAction(MoveTo::create(0.1, Vec2((_monster->getContentSize().width - balloon->getContentSize().width) * 0.5 - 17, _monster->getContentSize().height - 30)));
                    balloon->setRotation(-15);
                }

            }
        }
    } else if (_balloons.size() == 1) {
        auto balloon = _balloons.at(0);
        balloon->setRotation(0);
        balloon->runAction(MoveTo::create(0.2, Vec2((_monster->getContentSize().width - balloon->getContentSize().width) * 0.5, _monster->getContentSize().height - 30)));
    }
}
