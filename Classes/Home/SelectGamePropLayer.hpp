//
//  SelectGamePropLayer.hpp
//  FristGame
//
//  Created by sfbest on 2017/12/13.
//
//

#ifndef SelectGamePropLayer_hpp
#define SelectGamePropLayer_hpp

#include "Const.hpp"
#include "GamePropModel.hpp"

class SelectGamePropLayer : public Layer {
    
    
public:
    virtual bool init(GameType type);
    static SelectGamePropLayer * create(GameType type);
    
private:
    Button *_userGoldCountBtn;
    Sprite *_propDes;
    Layout *_buyBG;
    Sprite *_buyTipsSprite;
    
    int _buyCount;
    GameType _type;
    GamePropModel *_model;
    
    // images
    string timePropDes[6] = {"gold_doble_des.png", "prop_slowDown_des.png", "bobm_des.png", "time_add30_des.png", "time_add60_des.png", "time_add90_des.png"};
    string guardPropDes[6] = {"gold_doble_des.png", "prop_slowDown_des.png", "bobm_des.png", "life_add2_des.png", "life_add4_des.png", "life_add6_des.png"};
    
    void propButtonClick(Ref *sender, Widget::TouchEventType type);
    void showShopingCar();
    
    
    // 选择或者取消道具
    bool clickProp(int gold, bool isAdd);
};


#endif /* SelectGamePropLayer_hpp */
