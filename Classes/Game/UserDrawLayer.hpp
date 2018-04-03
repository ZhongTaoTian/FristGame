//
//  UserDrawLayer.hpp
//  FristGame
//
//  Created by sfbest on 2017/4/20.
//
//  记录用户绘制Layer层

#ifndef UserDrawLayer_hpp
#define UserDrawLayer_hpp

#include "Const.hpp"
#include "GeometricRecognizer.h"
#include "Balloon.hpp"

using namespace DollarRecognizer;

class UserDrawLayer : public Layer {
    
public:
    virtual bool init();
    CREATE_FUNC(UserDrawLayer);
    
private:
    DollarRecognizer::GeometricRecognizer* g_rGemertricRecognizer;
    DollarRecognizer::Path2D p_2dPath;
    RenderTexture * _target;
    
    Vector<cocos2d::LayerColor*> _brushs;
    Vector<cocos2d::LayerColor*> _lasts;
    // 路径碎片点集合(星星)
    vector<Vec2> _points;
    
    
    // 默认绘制路径的颜色
    Color4B _drawColor = Color4B(32, 189, 255, 255);

private:
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    bool onTouchesBegin(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    
};

#endif /* UserDrawLayer_hpp */
