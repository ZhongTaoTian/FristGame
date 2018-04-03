//
//  Monster.hpp
//  FristGame
//
//  Created by sfbest on 2017/4/19.
//
//

#ifndef Monster_hpp
#define Monster_hpp

#include "Const.hpp"
#include "Balloon.hpp"

enum MonsterBalloonType
{
    OneBalloon = 0,
    TwoBalloon,
    ThreeBalloon,
    More
};

enum MonsterType
{
    YellowMonster = 0,
    BlueMonster,
    GreenMonster,
    ShellMonster,
    BigMonster
};

class Monster : public Layer {
    
public:
    virtual bool init(MonsterBalloonType type);
    static Monster * create(MonsterBalloonType type);
    
    void removeBalloon(BalloonType type);
    Vector<Balloon *>_balloons;
    bool _isInScreen;
    MonsterBalloonType _type;
    MonsterType _monsterType;
    
    void dieAnimation();
    void bombAnim();
private:
    Sprite *_monster;
    
    void changeBalloonAnimation();
    void blowUpAnimation(Vec2 position);
};

#endif /* Monster_hpp */
