//
//  Video.h
//  Video
//
//  Created by um on 16/07/26.
//  Copyright (c) 2016年 um. All rights reserved.
//
#import <UIKit/UIKit.h>

/*
 * 视频下载失败通知。userinfo有下载失败的视频的名字
 */

/*
 *video download fail notification. video infos contained in notification userinfo
 */
extern NSString *VideoFileDownloadFailNotification;

/*
 * 视频即将播放
 */
/*
 * video will play notification
 */
extern NSString *VideoWillPlayNotification;

/*
 * 播放过程中下载视频超时检测
 */
/*
 * video download over time notification
 */
extern NSString *VideoDownloadOverTimeNotification;

/*
 * 全屏按钮按下后的通知，一般用于状态栏的隐藏与显示
 */
/*
 * button full screen pressed notification
 */
extern NSString *VideoFullButtonPressNotification;


@class UMBannerView;
@interface UMVideoAd : UIWebView

#pragma mark - 公共接口 common API

/**
 *
 * 1:初始化appid和appsecret (您需要先在官网注册并创建一个应用，在项目初始化的时候调用本方法传入对应的值)
 * 2:ynCache为YES代表预缓存视频， 为NO代表不预缓存视频 (预缓存仅在wifi环境下生效)
 */

/**
* init video SDK
* parameter: appId,appKey。 (apply from  )
* parameter: cachevideo。  'yes' means precache video , 'no' means not
*/
+(void)initAppID:(NSString *)appId appKey:(NSString *)appKey cacheVideo:(BOOL)ynCache;

/*
 * sdk版本号
 */
/*
 * SDK version
 */
+(NSString *)sdkVersion;




#pragma mark - 全屏播放视频的相关接口  full screen player API

/*
 *
 * 开始播放视频 ［全屏］ ［必须］
 *
 * 传入当前的viewController。视频将会以viewController presentMoviePlayerViewControllerAnimated:VideoController的方式呈现
 * Unity3D或者其他游戏引擎最好传入[[[UIApplication sharedApplication] keyWindow] rootViewController]
 *
 * VideoPlayFinishCallBackBlock是视频播放完成后马上回调,isFinishPlay为true则是用户完全播放，为false则是用户中途退出
 *
 * VideoPlayConfigCallBackBlock会在sdk的服务器最终确认这次播放是否有效后回掉（播放完成后有网络请求，网络不好可能有延时）。
 *
 * 注意：  isLegal在（app有联网，并且注册的appkey后台审核通过）的情况下才返回yes, 否则都是返回no.
 */
/*
 
 
 *
 * show video player ［full screen］ ［Required］
 *
 * set paramete viewController with your UIViewController。 SDK will show video player by [viewController presentMoviePlayerViewControllerAnimated:VideoController]
 * Unity3D or other game engine prefer [[[UIApplication sharedApplication] keyWindow] rootViewController]
 *
 * ideoPlayFinishCallBackBlock will be called immediately when video play finish or exit. block paramete isFinishplay will set to NO when video is dropped out by user.
 *
 * videoPlayConfigCallBackBlock will be called after YouMi server confirm. If parameter isLegal is setted to yes, income comes for you.
 *
 */
+(void)videoPlay:(UIViewController *)viewController videoPlayFinishCallBackBlock:(void(^)(BOOL isFinishPlay))block videoPlayConfigCallBackBlock:(void(^)(BOOL isLegal))configBlock;


#pragma mark - 原生模式，嵌入式视频  Custom video player

/*
 * 开始播放视频［原生模式］［必须］
 *
 * 传入当前的viewController。Unity3D或者其他游戏引擎最好传入[[[UIApplication shareApplication] keyWindow] rootViewController]
 * VideoPlayerFrame是视频的frame,superView是用于 ［addSubView：视频view］的，一般情况下可以传入viewController.view
 *
 * 注意：  isLegal在（app有联网，并且注册的appkey后台审核通过）的情况下才返回yes, 否则都是返回no.
 */
/*
 * native video player［Custom mode］［Required］
 *
 * set paramete viewController with your UIViewController. Unity3D or other game engine prefer [[[UIApplication sharedApplication] keyWindow] rootViewController]
 * superView is used for ［addSubView：videoView］, viewController.view general
 * VideoPlayerFrame used for setting video frame
 * ideoPlayFinishCallBackBlock will be called immediately when video play finish or exit. block paramete isFinishplay will set to NO when video is dropped out by user.
 * videoPlayConfigCallBackBlock will be called after YouMi server confirm. If parameter isLegal is setted to yes, income comes for you.
 */
+(void)videoPlay:(UIViewController *)viewController videoSuperView:(UIView *)superView videoPlayerFrame:(CGRect)VideoPlayerFrame videoPlayFinishCallBackBlock:(void(^)(BOOL isFinish))block videoPlayConfigCallBackBlock:(void(^)(BOOL isLegal))configBlock;


#pragma mark - 插页视频  Spot Video
/**
 * 开始播放插页视频 ［插页］［只适用于竖屏］
 *
 * 传入当前viewController.superView是用于 ［addSubView：视频view］的，一般情况下可以传入viewController.view
 *
 */
/**
 * show spot video player ［Spot mode］［only for Portrait APP］
 *
 * viewController.superView used for ［addSubView：videoView], viewController.view general
 *
 */
+(void)videoSpotPlay:(UIViewController *)viewController videoSuperView:(UIView *)superView videoPlayFinishCallBackBlock:(void(^)(BOOL isFinish))block videoPlayConfigCallBackBlock:(void(^)(BOOL isLegal))configBlock;


/**
 *
 *开始展示banner条［可选］
 *
 * VideoPlayerFrame是视频的frame,
 * closeBlock是关闭回调
 */
/**
 *
 *adding video banner in Custom mode [Custom mode] [Optional]
 *
 */
+(UMBannerView *)videoBannerPlayerFrame:(CGRect)VideoPlayerFrame videoBannerPlayCloseCallBackBlock:(void(^)(BOOL isLegal))closeBlock;



/**
 * 是否有可播放的缓存视频
 */
/**
 * is video which will be play have cached
 */
+(BOOL)videoIsCacheVideoFile;

/*
 * 是否还有视频可以播放  ［可选］
 * isHaveVideoStatue的值目前有两个
 * 0：有视频可以播放
 * 1：暂时没有可播放视频
 * 2：网络状态不好
 */

/*
 isHaveVideoStatue  [Optional]
 0：there are videos for playing
 1：there are no videos for playing
 2：network error
 */
+(void)videoHasCanPlayVideo:(void(^)(int isHaveVideoStatue))backCallBlock;

/*
 * 关闭视频  ［可选］
 */
/*
 * close video player  ［Optional］
 */
+(void)videoCloseVideoPlayer;

/*
 * 点击关闭按钮不显示『中途退出没有奖励』弹出框  ［可选］ 默认显示
 */
/*
 * close alert view [Optional]
 */
+(void)videoCloseAlertViewWhenWantExit:(BOOL)isClose;



/**
 * 设置是否强制横屏，默认是强制横屏 [全屏] ［可选］
 */
/**
 * playing screen orientation setting [full screen] ［Optional］
 */
+(void)videoIsForceLandscape:(BOOL)isForce;



/*
 * 开发者需要自定义界面的时候用到，隐藏右下角的<全屏>按钮
 */
/*
 * hide full screen button [custom mode] [Optional]
 */
+(void)videoHiddenFullScreenButtonView:(BOOL)ynHide;


/*
 * [可选]
 * 是否在非wifi模式下开启缓存视频文件： YES开启  /  NO不开启
 * [默认不开启]
 */
+(void)videoDownloadOnUNWifi:(BOOL)ynDown;

/*
 * 剩余时间，还没开始播放时返回－1。开发者需要设置一个定时器。每秒获取一次
 */
/*
 * get remaining time. return -1 before playing
 */
+(int)videoGetRestPlayerTime;

/**
 * 静音
 * YES静音 / NO非静音
 * 注：调用展示广告前调用本方法，本方法非实时控制音效
 */
+(void)videoMute:(BOOL)ynMute;

/**
 * 静音或者开启
 */
/**
 * silence or music
 */
+(void)videoButtonVoicePressed;


/**
 * 关闭
 */
/**
 * close video pressed
 */
+(void)videobtnClosePressed;


/**
 *
 * 是否显示右上角倒计时 【默认：NO】
 */
+(void)videoShowProgressTime:(BOOL)ynShow;




/*
 *暂停视频播放
 */
/*
 *pause
 */
+(void)pauseVideoPlay;

/*
 *继续视频播放
 */
/*
 *continue playing
 */
+(void)continueVideoPlay;

/** 设置关闭弹出框的内容
 *  注：默认内容为（确认退出观看视频？）， 但是有些开发者需要做点不一样的提示，如：中途退出无法获得奖励哟～
 */
/**
 *  self-define alert view message
 */
+(void)videosetCloseAlertContent:(NSString *)content;


// !!!:开发者设置的用户ID，默认为空（50个字符串以内）（可选）
+(void)videosetUserId:(NSString *)userId;

/**
 * 隐藏前贴片详情页的重放按钮 (可选)
 */
/**
 * hide custom mode AD's detail view replay button
 */
+(void)hideDetailViewReplayBtn:(BOOL)ynHide;


/**
 * 设置播放界面视频下载超时时间 ［可选］ 
 * 默认60秒
 */
/**
 * set video download timeout duration
 */
+(void)videoDownloadOverTime:(float)time;

#pragma 原生视频相关
/**
 *
 * 重置原生视频的初始化frame
 * 重置时视频frame不会立即改变，在视频点击全屏按钮恢复时才会改变
 */
+(void)videoResetFreeSizeFrame:(CGRect)frame;

/**
 * 全屏
 */
/**
 * fullscreen
 */
+(void)videoButtonFullScreenPressed;

/*
 * 设置左旋转全屏还是右旋转全屏［非全屏］［可选］
 * 0:复原
 * 1:左旋转全屏
 * 2:右旋转全屏
 */

/*
 * set to full screen when playing［custom mode］［Optional］
 * 0:restoration
 * 1:rotate left to full screen
 * 2:rotate right to full screen
 */
+(void)videoIslandscapeLeftOrRight:(int)vidoeShowStuts;
@end
