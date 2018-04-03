//
//  Balloon.cpp
//  FristGame
//
//  Created by sfbest on 2017/4/18.
//
//

#include "Balloon.hpp"

bool Balloon::init(BalloonType type, float lineH)
{
    std::string name;
    switch (type) {
        case Circle:
            name = "balloon_circle.png";//
            break;
        case Star:
            name = "balloon_star.png";
            break;
        case LessThan:
            name = "balloon_lessthan.png"; //
            break;
        case Lightning:
            name = "balloon_lightning.png";//
            break;
        case Z:
            name = "balloon_z.png";//
            break;
        case M:
            name = "balloon_m.png";//
            break;
        case Plait:
            name = "balloon_plait.png";//
            break;
        case Triangle:
            name = "balloon_triangle.png";//
            break;
        case Wave:
            name = "balloon_wave.png";//
            break;
        case Bow:
            name = "balloon_half.png";//
            break;
        case S:
            name = "balloon_s.png";
            break;
        case Ladder:
            name = "balloon_ladder.png";
            break;
        case Eight:
            name = "balloon_eight.png";//
            break;
        case Knot:
            name = "balloon_knot.png";
            break;
        default:
            return false;
    }
    
    _type = type;
    
    Layer::init();
    	
    _balloon = Sprite::createWithSpriteFrameName(name);
    _balloon->setAnchorPoint(Vec2(0.5, 0));
    addChild(_balloon, 3);
    
    _line = LayerColor::create(Color4B::BLACK);
    _line->setContentSize(Size(2, lineH));
    _line->setAnchorPoint(Vec2(0.5, 0));
    _line->setPosition(Vec2(_balloon->getContentSize().width * 0.5, 0));
    _balloon->setPosition(Vec2(_balloon->getContentSize().width * 0.5, _line->getContentSize().height));
    addChild(_line, 1);
    
    setContentSize(Size(_balloon->getContentSize().width, lineH + _balloon->getContentSize().height));
    
    return true;
}

Balloon* Balloon::create(BalloonType type, float lineH)
{
    auto bubble = new Balloon();
    if (bubble && bubble->init(type, lineH)) {
        bubble->autorelease();
    } else {
        delete bubble;
        bubble = nullptr;
    }
    
    return bubble;
}

// 随机创建一个气球
Balloon* Balloon::createRandomWithLineHeight(float lineH, vector<BalloonType> excludeTypes)
{
    auto type = BalloonType(arc4random_uniform(14));
    
    if (excludeTypes.size() == 1) {
        while (type == excludeTypes.at(0)) {
            type = BalloonType(arc4random_uniform(14));
        }
    }
    
    if (excludeTypes.size() == 2) {
        while (type == excludeTypes.at(0) || type == excludeTypes.at(1)) {
            type = BalloonType(arc4random_uniform(14));
        }
    }
    
    auto balloon = Balloon::create(type, lineH);
    balloon->setScale(0.7);
    return balloon;
}

// 根据绘制图形名字获取对应气球类型(如果没有,返回Unknow类型)
BalloonType Balloon::getTypeWithName(const std::string name, float score)
{
    auto type = UnKnow;
    
    if (name == "Circle") {
        if (score > 0.75) {
            type = Circle;
        }
    } else if (name == "Star") {
        if (score > 0.70) {
            type = Star;
        }
    } else if (name == "LessThan") {
        if (score > 0.75) {
            type = LessThan;
        }
    } else if (name == "Lightning") {
        if (score > 0.7) {
            type = Lightning;
        }
    } else if (name == "Z") {
        if (score > 0.7) {
            type = Z;
        }
    } else if (name == "M") {
        if (score > 0.7) {
            type = M;
        }
    } else if (name == "Plait") {
        if (score > 0.6) {
            type = Plait;
        }
    } else if (name == "Triangle") {
        if (score > 0.75) {
            type = Triangle;
        }
    } else if (name == "Wave") {
        if (score > 0.55) {
            type = Wave;
        }
    } else if (name == "Bow") {
        if (score > 0.6) {
            type = Bow;
        }
    } else if (name == "S") {
        if (score > 0.7) {
            type = S;
        }
    } else if (name == "Ladder") {
        if (score > 0.8) {
            type = Ladder;
        }
    } else if (name == "Eight") {
        if (score > 0.65) {
            type = Eight;
        }
    } else if (name == "Knot") {
        if (score > 0.6) {
            type = Knot;
        }
    }
    
    return type;
}

// 根据气球类型获取对应颜色
Color4B Balloon::getColorWithBalloonType(BalloonType type)
{
    Color4B color = Color4B(32, 189, 255, 255);
    
    switch (type) {
        case Circle:
            color = Color4B(100, 117, 221, 255);
            break;
        case Star:
            color = Color4B(255, 128, 44, 255);
            break;
        case LessThan:
            color = Color4B(221, 51, 34, 255);
            break;
        case Lightning:
            color = Color4B(14, 166, 216, 255);
            break;
        case Z:
            color = Color4B(0, 131, 126, 255);
            break;
        case M:
            color = Color4B(235, 115, 191, 255);
            break;
        case Plait:
            color = Color4B(119, 69, 145, 255);
            break;
        case Triangle:
            color = Color4B(171, 225, 116, 255);
            break;
        case Wave:
            color = Color4B(255, 247, 116, 255);
            break;
        case Bow:
            color = Color4B(76, 252, 233, 255);
            break;
        case S:
            color = Color4B(241, 105, 82, 255);
            break;
        case Ladder:
            color = Color4B(111, 160, 167, 255);
            break;
        case Eight:
            color = Color4B(185, 122, 188, 255);
            break;
        case Knot:
            color = Color4B(164, 75, 209, 255);
            break;
        default:
            break;
    }
    
    return color;
}

void Balloon::bobmAnimationWithFinish()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("balloons_atlas.plist");
    auto aaaa = Sprite::createWithSpriteFrameName("ge_ballon_pop_05.png");
    aaaa->setPosition(Vec2(_balloon->getPosition().x, _balloon->getPosition().y + 55));
    aaaa->setScale(2.5);
    addChild(aaaa, 100);
    
    _balloon->removeFromParent();
    
    Vector<SpriteFrame *>frames;
    for (int i = 1; i < 12; i++) {
        char name[30];
        if (i > 9) {
            sprintf(name, "ge_ballon_pop_%d.png", i);
        } else {
            sprintf(name, "ge_ballon_pop_0%d.png", i);
        }
        auto sf = SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
        frames.pushBack(sf);
    }
    auto animation = Animation::createWithSpriteFrames(frames);
    animation->setDelayPerUnit(0.03);
    auto anim = Animate::create(animation);

    aaaa->runAction(Sequence::create(anim, CallFunc::create([this](){
        this->removeFromParent();
    }), NULL));
    
    _line->removeFromParent();
    
//    _balloon->runAction(FadeOut::create(0.15));
}

void Balloon::setLineHeight(float lineH, bool needAnimation)
{
    _line->setContentSize(Size(2, lineH));
    if (needAnimation) {
        _balloon->runAction(MoveTo::create(0.2, Vec2(_balloon->getPosition().x, _line->getContentSize().height)));
    } else{
        _balloon->setPosition(Vec2(_balloon->getPosition().x, _line->getContentSize().height));
    }
}

// 重写旋转 让线旋转 气球只偏移
void Balloon::setRotation(float rotation)
{
    _line->stopAllActions();
    _balloon->stopAllActions();
    _line->setRotation(rotation);
    _line->runAction(RotateTo::create(0.15, rotation));
    float offsetX = _line->getContentSize().height * cos((90.0 - abs(rotation)) / 180.0 * PI);
    
    if (rotation < 0) {
        offsetX = -offsetX;
    }
    
    _balloon->runAction(MoveTo::create(0.15, Vec2(_balloon->getContentSize().width * 0.5 + offsetX, _balloon->getPosition().y - abs(rotation) * 0.9)));
}

