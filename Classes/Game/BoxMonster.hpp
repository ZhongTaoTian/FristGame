//
//  BoxMonster.hpp
//  FristGame
//
//  Created by sfbest on 2017/6/19.
//
//  宝箱怪物

#ifndef BoxMonster_hpp
#define BoxMonster_hpp

#include "Const.hpp"
#include "Balloon.hpp"

class BoxMonster : public Layer {
    
public:
    virtual bool init();
    CREATE_FUNC(BoxMonster);
    
    // 开始出现
    void startAppear();
    // 消除气球
    void cleanWithType(BalloonType type);
    // 销毁
    void startBobm();
    
    void startFragmentBobmAnimationWithPosition(Vec2 position);
    virtual void pause();
    virtual void resume();
    bool hasBoxMonster;
    
private:
    Sprite *_monster;
    Sprite *_ice;
    Sprite *_graph;
    Sprite *_box;
    BalloonType _type;
    int _lifeCount;
    void randBoxType();
    
    bool _isOpenTimeUp = false;
    Vector<Sprite *> golds;
    
    void addOneGoldFall(Vec2 posi, Vec2 offset);
    Vec2 randPosition(Vec2 position);
    
    void update(float dt);
};

#endif /* BoxMonster_hpp */
