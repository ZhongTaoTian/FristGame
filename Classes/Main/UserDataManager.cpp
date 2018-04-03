//
//  UserDataManager.cpp
//  GoldMiner
//
//  Created by tianzhongtao on 2016/11/28.
//

#include "UserDataManager.hpp"

#define kUserTimeBestScore "kUserTimeBestScore"
#define kUserGuardBestScore "kUserGuardBestScore"
#define kUserGoldCount "kUserGoldCount"

static UserDataManager *s_SharedUserDataManager = nullptr;

UserDataManager *UserDataManager::getInstance()
{
    if (s_SharedUserDataManager == nullptr) {
        s_SharedUserDataManager = new UserDataManager();
        
        s_SharedUserDataManager->_musicMute = SoundTool::getInstance()->getMusicIsMute();
        s_SharedUserDataManager->_soundMute = SoundTool::getInstance()->getEffectIsMute();
        s_SharedUserDataManager->_timeBestScore = UserDefault::getInstance()->getIntegerForKey(kUserTimeBestScore);
        s_SharedUserDataManager->_guardBestScore = UserDefault::getInstance()->getIntegerForKey(kUserGuardBestScore);
        s_SharedUserDataManager->_userGoldCount = UserDefault::getInstance()->getIntegerForKey(kUserGoldCount);
    }
    
    return s_SharedUserDataManager;
}

void UserDataManager::setMusicMute(bool mute)
{
    _musicMute = mute;
    
    SoundTool::getInstance()->setMusicMute(mute);
}

void UserDataManager::setSoundMute(bool mute)
{
    _soundMute = mute;
    
    SoundTool::getInstance()->setEffectMute(mute);
}

void UserDataManager::setTimeBestScore(int var)
{
    if (_timeBestScore >= var) {
        return;
    }
    
    _timeBestScore = var;
    UserDefault::getInstance()->setIntegerForKey(kUserTimeBestScore, _timeBestScore);
}

void UserDataManager::setGuardBestScore(int var)
{
    if (_guardBestScore >= var) {
        return;
    }
    
    _guardBestScore = var;
    UserDefault::getInstance()->setIntegerForKey(kUserGuardBestScore, _guardBestScore);
}

void UserDataManager::addUserGoldCount(int count)
{
    _userGoldCount += count;
    
    UserDefault::getInstance()->setIntegerForKey(kUserGoldCount, _userGoldCount);
}
