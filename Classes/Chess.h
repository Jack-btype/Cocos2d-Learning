#ifndef _CHESS_H_
#define _CHESS_H_

#include "cocos2d.h"

class Chess : public cocos2d::Sprite
{
public:
    
    Chess();

    
    enum TYPE {
        JIANG, // ��
        SHI,   // ʿ
        XIANG, // ��
        CHE,   // ��
        MA,    // ��
        PAO,   // ��
        BING   // ��
    }; 
    enum COLOR {
        RED,    // �췽
        BLACK   // �ڷ�
    };

    //ÿһ��ľ���
    static float _offset;

    static Chess * create(int id, Chess::COLOR color);

    bool init(int id, Chess::COLOR color);

    void reset(Chess::COLOR color);

    static struct InitPos
    {
        int _x;
        int _y;
        Chess::TYPE _type;
    } _initPos[16];



    void setX(int x);
    void setY(int y);
    int getX();
    int getY();
    int getID();

    int x;
    int y;
    int _id;
    bool _dead;
    Chess::TYPE _type;
    Chess::COLOR _color;

};


#endif // _CHESS_H_