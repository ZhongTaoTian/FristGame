//
//  Help.hpp
//  FristGame
//
//  Created by sfbest on 2017/5/12.
//
//

#ifndef Help_hpp
#define Help_hpp

#include <stdio.h>
#include "Const.hpp"

class Help {
    
public:
    static cocos2d::Animate *getAnimate(std::string startName, int count, float delayPerUnit);
    static cocos2d::Animate *getAnimate(int startIndex, std::string startName, int count, float delayPerUnit, bool restoreOriginalFrame = true);
};

#endif /* Help_hpp */
