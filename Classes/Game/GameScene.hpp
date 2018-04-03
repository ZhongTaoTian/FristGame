//
//  GameScene.hpp
//  FristGame
//
//  Created by sfbest on 2017/4/14.
//
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include "Const.hpp"
#include "MonsterLayer.hpp"
#include "UserDrawLayer.hpp"
#include "GameControlLayer.hpp"
#include "GamePropModel.hpp"
#include "GamePropLayer.hpp"

class Game: Layer {
    
public:
    static Scene * createScene(GameType gameType, GamePropModel *model);
    virtual bool init(GameType gameType, GamePropModel *model);
    static Game *create(GameType gameType, GamePropModel *model);
    GamePropLayer *_gamePropLayer;
    
private:
    // 怪物层
    MonsterLayer *_monsterLayer;
    // 用户绘制层
    UserDrawLayer *_userDrawLayer;
    GameControl *_gameControl;
    GameType _gameType;
    // 道具层
private:
    
    void startGame();
    void startAppearMonster();
    void addCustomActions();
    

    
    void onExit();
};


#endif /* GameScene_hpp */
