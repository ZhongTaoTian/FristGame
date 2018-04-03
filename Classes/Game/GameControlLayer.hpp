//
//  GameControlLayer.hpp
//  FristGame
//
//  Created by sfbest on 2017/5/27.
//
//

#ifndef GameControlLayer_hpp
#define GameControlLayer_hpp

#include "Const.hpp"
#include "GamePropModel.hpp"

class GameControl : public Node {
    
public:
    virtual bool init(GameType type, GamePropModel *model);
    static GameControl *create(GameType type, GamePropModel *model);
    int _life;
    void lifeCutDown();
    void addGoldCount(int goldCount);
    int _goldCount;
    void killCountAdd(int killCount);
    
    void startClocking();
    void stopClocking();
    virtual void update(float dt);
    
    void showGamePauseLayer();
    void hidePauseLayer();
    
    
    // 显示连续击杀效果
    void showContinuousHitAndKill(int killNum);
    
    // 显示结束UI
    void showEndUI();
private:
    int _time;
    GameType _gameType;
    
    Button *_pauseBtn;
    LabelAtlas *_lifeLabel;
    LabelAtlas *_goldLabel;
    Label *_killCountLabel;
    int _killCount;
    
    Sprite *_timeSprite;
    Sprite *_lifeSprite;
    Sprite *_killTipsSp;
    Sprite *_goldSp;
    
    int _buyCount;
    Button *_userGoldCountBtn;
    
    void btnClick(Ref *sender, Widget::TouchEventType type);
    void finishAnimationFinish();
    void showShopCart();
    void buyPropWithTag(int tag);
    void replayGame();
};

#endif /* GameControlLayer_hpp */
