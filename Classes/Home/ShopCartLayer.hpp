//
//  ShopCartLayer.hpp
//  FristGame
//
//  Created by sfbest on 2017/12/13.
//
//

#ifndef ShopCartLayer_hpp
#define ShopCartLayer_hpp

#include "Const.hpp"

class ShopCartLayer : public Layer {
    
public:
    virtual bool init();
    CREATE_FUNC(ShopCartLayer);
    
private:
    void buttonClick(Ref *sender, Widget::TouchEventType type);
    void buyGoldWithTag(int tag);
};

#endif /* ShopCartLayer_hpp */
