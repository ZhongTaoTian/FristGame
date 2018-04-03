//
//  UserDrawLayer.cpp
//  FristGame
//
//  Created by sfbest on 2017/4/20.
//
//

#include "UserDrawLayer.hpp"
#include "Fragment.hpp"

bool UserDrawLayer::init()
{
    Layer::init();
    
    // 添加绘制层
    _target = RenderTexture::create(kWinSizeWidth, kWinSizeHeight, Texture2D::PixelFormat::RGBA8888);
    _target->retain();
    _target->setPosition(Vec2(kWinSizeWidth / 2, kWinSizeHeight / 2));
    this->addChild(_target);
    
    // 加载图像模板
    g_rGemertricRecognizer = new DollarRecognizer::GeometricRecognizer;
    g_rGemertricRecognizer->loadTemplates();
    
    // 添加手势
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesMoved = CC_CALLBACK_2(UserDrawLayer::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(UserDrawLayer::onTouchesEnded, this);
    listener->onTouchesBegan = CC_CALLBACK_2(UserDrawLayer::onTouchesBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

bool UserDrawLayer::onTouchesBegin(const std::vector<Touch *> &touches, cocos2d::Event *unused_event)
{
    
    if (!isVisible())
    {
        return false;
    }
    
    // 清除上次的路径集合
    p_2dPath.clear();
    
    _points.clear();
    
    return true;
}

void UserDrawLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event* event)
{
    if (!isVisible()) return;
    
    auto touch = touches[0];
    auto start = touch->getLocation();
    auto end = touch->getPreviousLocation();
    
    _target->begin();
    auto tmp = touch->getLocationInView();
    p_2dPath.push_back(Point2D(tmp.x, tmp.y));
    
    float distance = start.getDistance(end);
    if (distance > 1)
    {
        int d = (int)distance;
        _brushs.clear();
        for(int i = 0; i < d; ++i)
        {
            auto lc = LayerColor::create(_drawColor);
            lc->setContentSize(Size(10, 10));
            _brushs.pushBack(lc);
            _lasts.pushBack(lc);
            
            float difx = end.x - start.x;
            float dify = end.y - start.y;
            float delta = (float)i / distance;
            lc->setPosition(Vec2(start.x + (difx * delta), start.y + (dify * delta)));
            _points.push_back(lc->getPosition());
            lc->visit();
        }
    }
    
    _target->end();
}

void UserDrawLayer::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{
    if (!isVisible()) return;
    
    auto color = _drawColor;
    
    if (p_2dPath.size() > 5){
        
        RecognitionResult result = g_rGemertricRecognizer->recognize(p_2dPath);
        
        CCLOG("%f-----%s", result.score, result.name.c_str());
        
        // 根据名字和结果获取绘制类型
        BalloonType type = Balloon::getTypeWithName(result.name, result.score);
        
        if (type != BalloonType::UnKnow) {
            // 显示是碎片
            color = Balloon::getColorWithBalloonType(type);
            Fragment::showFragmentToView(this, _points, color);
            
            // 有绘制图形对应的模板,通知MonsterLayer层移除气泡
            EventCustom event("CleanBalloon");
            BalloonResult *res = new BalloonResult();
            res->setType(type);
            event.setUserData((void *)res);
            
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }
    }
    
    _target->clear(0, 0, 0, 0);
    
    for (int i = 0; i < _lasts.size(); i++) {
        auto cl = _lasts.at(i);
        
        auto ss = Sequence::create(CallFuncN::create([color](Node *node){
            node->setColor(Color3B(color.r, color.g, color.b));
        }), FadeTo::create((color == _drawColor ? 0.2 : 0.5), 0), CallFuncN::create([](Node *n){
            n->removeFromParent();
        }), NULL);
        
        cl->runAction(ss);
        addChild(cl, 2);
    }
    
    _lasts.clear();
}
