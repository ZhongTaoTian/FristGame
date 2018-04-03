//
//  HomeScene.hpp
//  FristGame
//
//  Created by sfbest on 2017/4/14.
//
//

#ifndef HomeScene_hpp
#define HomeScene_hpp

#include "Const.hpp"

class Home : Layer {
    
public:
    
    virtual bool init();
    static Scene *createScene();
    CREATE_FUNC(Home);
  
private:
    Sprite *_blueMonster;
    Sprite *_eyeSprite;
    Sprite *_yellowMonster;
    Sprite *_bigMonster;
    
    bool _bigIsAnimation;
    bool _blueIsAnimation;
    bool _yellowIsAnimation;
    
    Button *_startBtn;
    Button *_backBtn;
    Button *_soundBtn;
    Button *_musicBtn;
    Button *_blogBtn;
    Button *_likeBtn;
    Button *_guardBtn;
    Button *_timeBtn;
    // flag
    bool _isLoading;
    bool _settingSelected;
    bool _messageSelected;
    
private:
    // 各种动画
    void playEyeShakeAnimation();
    void playYellowMonsterNormalAnimation();
    void playBlueMonsterClickAnimation();
    void playYellowMosnterClickAnimation();
    void playBigMonsnterClickAnimation();
    void showPropView(bool isTimeModel);
    void showShopingCar();
    // 周期
    virtual void onEnterTransitionDidFinish();
    
    // button click action
    void buttonClickAction(Ref *sender, Widget::TouchEventType type);
    
    // 更新音效音乐状态
    void updateSettingBtnStatus(bool isSound);
    
    // 显示广告
    void showAdView(bool isFull);
};

#endif /* HomeScene_hpp */
