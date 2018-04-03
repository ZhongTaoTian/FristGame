//
//  Fragment.hpp
//  DefendHomeland
//
//  Created by sfbest on 2017/3/27.
//
//

#ifndef Fragment_hpp
#define Fragment_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class Fragment : public LayerColor {
    
public:
    
    static void showFragmentToView(Layer *parent, std::vector<Vec2> points, Color4B color);

private:
    static float randPositiveNegativeNumber(float nubmer);
};

#endif /* Fragment_hpp */


