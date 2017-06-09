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

    // ��ǵ�ǰ�ֵ���һ������
    Chess::COLOR _turn;
    
    // ����ѡ�е����ӵ�id
    int _select_id;

    // ѡ���
    cocos2d::Sprite * _select_sprite;
    // ������
    cocos2d::Sprite * _end_sprite;

    // ��ʾһ�����ӵ�ֱ��
    float _d;
    // ��һ��������ԭ���λ��
    cocos2d::Vec2 _chess_offset;

    // ��������ϵ�32������
    Chess * _c[32];
    
    // ͨ����������ȡ����id�������ǰλ��û�����ӣ�����-1
    int getChess(int x, int y);
    // �ѵ������ת��Ϊ�������
    bool getClickPos(cocos2d::Vec2 ptInWin, int &x, int &y);
    // ��ȡ����λ�ã����������ת��Ϊ��Ļ����
    cocos2d::Vec2 getChessPos(int x, int y);

    // �����Ļ�����������������Ƿ�㵽����
    void addTouchListener();
    // ����ʱ�俪ʼ����Ļص�����
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);

    // �ƶ�����
    void moveChess(int moveId, int killId, int x, int y);

    // �������
    bool canMove(int moveId, int killId, int x, int y);
    // ����ʿ���ࡢ�������ڡ������������
    bool canMoveJiang(int moveId, int killId, int x, int y);
    bool canMoveShi(int moveId, int killId, int x, int y);
    bool canMoveXiang(int moveId, int killId, int x, int y);
    bool canMoveChe(int moveId, int killId, int x, int y);
    bool canMoveMa(int moveId, int killId, int x, int y);
    bool canMovePao(int moveId, int killId, int x, int y);
    bool canMoveBing(int moveId, int killId, int x, int y);

    // ����������������֮������ӵ�����
    int getChessCount(int xo, int yo, int x, int y);

    // ��Ӳ˵���
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
