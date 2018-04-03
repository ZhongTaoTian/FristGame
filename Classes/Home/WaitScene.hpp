//
//  WaitScene.hpp
//  FristGame
//
//  Created by sfbest on 2017/12/14.
//
//

#ifndef WaitScene_hpp
#define WaitScene_hpp

#include "Const.hpp"
#include "GamePropModel.hpp"

class WaitScene : public Layer {
    
    
public:
    virtual bool init(GameType type, GamePropModel *model);
    static Scene* createScene(GameType type, GamePropModel *model);
    static WaitScene * create(GameType type, GamePropModel *model);
};

#endif /* WaitScene_hpp */
