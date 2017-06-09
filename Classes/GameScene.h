#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Chess.h"
using namespace cocos2d::ui;

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // 标记当前轮到哪一方下子
    Chess::COLOR _turn;
    
    // 保存选中的棋子的id
    int _select_id;

    // 选择框
    cocos2d::Sprite * _select_sprite;
    // 结束框
    cocos2d::Sprite * _end_sprite;

    // 表示一个棋子的直径
    float _d;
    // 第一个棋子离原点的位置
    cocos2d::Vec2 _chess_offset;

    // 存放棋盘上的32个棋子
    Chess * _c[32];
    
    // 通过相对坐标获取棋子id，如果当前位置没有棋子，返回-1
    int getChess(int x, int y);
    // 把点击坐标转化为相对坐标
    bool getClickPos(cocos2d::Vec2 ptInWin, int &x, int &y);
    // 获取棋子位置，把相对坐标转换为屏幕坐标
    cocos2d::Vec2 getChessPos(int x, int y);

    // 添加屏幕触摸监听器，监听是否点到棋子
    void addTouchListener();
    // 触摸时间开始点击的回调函数
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);

    // 移动棋子
    void moveChess(int moveId, int killId, int x, int y);

    // 走棋规则
    bool canMove(int moveId, int killId, int x, int y);
    // 将、士、相、车、马、炮、兵的走棋规则：
    bool canMoveJiang(int moveId, int killId, int x, int y);
    bool canMoveShi(int moveId, int killId, int x, int y);
    bool canMoveXiang(int moveId, int killId, int x, int y);
    bool canMoveChe(int moveId, int killId, int x, int y);
    bool canMoveMa(int moveId, int killId, int x, int y);
    bool canMovePao(int moveId, int killId, int x, int y);
    bool canMoveBing(int moveId, int killId, int x, int y);

    // 计算两个相对坐标点之间的棋子的数量
    int getChessCount(int xo, int yo, int x, int y);

    // 添加菜单栏
    void addMenuBar();

    void startClick(Ref * pSender);
    void newClick(Ref * pSender);

    // DEBUG TEXTFIELD
    //TextField * debug;
    //TextField * Turn;

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
};

#endif // __GAME_SCENE_H__
