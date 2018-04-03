//
//  GamePropLayer.hpp
//  FristGame
//
//  Created by sfbest on 2017/11/29.
//
//

#ifndef GamePropLayer_hpp
#define GamePropLayer_hpp

#include "Const.hpp"
#include "GamePropModel.hpp"

class GamePropLayer: public Layer {
    
public:
    virtual bool init(GameType gameType, GamePropModel *model);
    static GamePropLayer *create(GameType gameType, GamePropModel *model);
    
    int _decelerationCount;
    int _bombCount;
    
    void addBobm();
    void addDeceleration();
    Sprite *_decelerationSp;
    Sprite *_bombSp;
    
    Button *_bombBtn;
    Button *_decelerationBtn;
private:
    GamePropModel *_gameProp;
    GameType _gameType;    
    Sprite *_goldSp;
    
    int _goldCount;
    
    void setDecelerationCount(int var);
    void setBombCount(int var);
    void setGoldCount(int var);
    
    void propClick(Ref *sender, Widget::TouchEventType type);
};

#endif /* GamePropLayer_hpp */
