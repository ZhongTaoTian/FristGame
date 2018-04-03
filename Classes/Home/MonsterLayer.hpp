//
//  MonsterLayer.hpp
//  FristGame
//
//  Created by sfbest on 2017/4/18.
//
//

#ifndef MonsterLayer_hpp
#define MonsterLayer_hpp

#include "Const.hpp"
#include "Monster.hpp"
#include "BoxMonster.hpp"

class MonsterLayer : public Layer {
    
public:
    virtual bool init(GameType gameType);
    static MonsterLayer *create(GameType gameType);
    void startAppearMonster();
    
    void removeBalloonWithBalloonType(BalloonType type);
    virtual void pause();
    virtual void resume();
    void useProp(bool isBomb);
    
private:
    // 出现间隔
    int _apperInterval;
    // 计时标记
    int _timeIndex;
    // 怪集合
    Vector<Monster *> _monsters;
    
    float _lastMonsterX;
    GameType _gameType;
    
    BoxMonster *_boxManster;
    
    Vector<Sprite *>golds;
    
    int _boxMonsterTime;
    int _boxMonsterIndex;
    float _speed;

    long _secondCount;
    bool _isDeceleration;
    float _lastSpeed;
    int _decelerationCount;
private:
    void update(float dt);
    void landCountAdd();
    void fallGold(bool more, Vec2 startPosition);
    void addOneGoldFall(Vec2 posi, Vec2 offset);
    
    
};

#endif /* MonsterLayer_hpp */
