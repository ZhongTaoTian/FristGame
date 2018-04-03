//
//  Balloon.hpp
//  FristGame
//
//  Created by sfbest on 2017/4/18.
//
//

#ifndef Balloon_hpp
#define Balloon_hpp

#include "Const.hpp"

enum BalloonType
{
    Circle = 0,
    Star = 1,
    LessThan = 2, // <
    Lightning = 3, // 闪电
    Z = 4,
    M = 5,
    Plait = 6, // 辫子
    Triangle = 7,
    Wave = 8,
    Bow = 9,
    S = 10,
    Ladder = 11,
    Eight = 12,
    Knot = 13, // 中国结
    UnKnow = 14
};
 
class Balloon : public Layer {
    
public:
    virtual bool init(BalloonType type, float lineH);
    static Balloon* create(BalloonType type, float lineH);
    static Balloon* createRandomWithLineHeight(float lineH, vector<BalloonType> excludeTypes);
    static BalloonType getTypeWithName(const std::string name, float score);
    static Color4B getColorWithBalloonType(BalloonType type);
    
    BalloonType getType() {
        return _type;
    };
    void setRotation(float rotation);
    void setLineHeight(float lineH, bool needAnimation = false);
    void bobmAnimationWithFinish();
private:
    BalloonType _type;
    LayerColor *_line;
    Sprite *_balloon;
};

class BalloonResult {
    
public:
    CC_SYNTHESIZE(BalloonType, _type, Type);
};


#endif /* Balloon_hpp */
