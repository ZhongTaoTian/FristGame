//
//  UserDataManager.hpp
//  GoldMiner
//
//  Created by sfbest on 2016/11/28.
//
//

#ifndef UserDataManager_hpp
#define UserDataManager_hpp

#include <stdio.h>
#include "Const.hpp"
#include "SoundTool.hpp"

class UserDataManager {

public:
    static UserDataManager *getInstance();
    bool getMusicMute() {
        return SoundTool::getInstance()->getMusicIsMute();
    };
    
    bool getSoundMute() {
        return SoundTool::getInstance()->getEffectIsMute();
    };
        
    void setMusicMute(bool mute);
    void setSoundMute(bool mute);
    
    void setTimeBestScore(int var);
    int getTimeBestScore() {
        return _timeBestScore;
    };
    
    void setGuardBestScore(int var);
    int getGuardBestScore() {
        return _guardBestScore;
    };
    
    int getUserGoldCount()  {
        return _userGoldCount;
    };
    void addUserGoldCount(int count);
    
private:
    bool _musicMute;
    bool _soundMute;
    int _timeBestScore;
    int _guardBestScore;
    int _userGoldCount;
};

#endif /* UserDataManager_hpp */
