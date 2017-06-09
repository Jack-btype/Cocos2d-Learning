#include "Chess.h"

Chess::Chess()
{
}


//创建一张表
Chess::InitPos Chess::_initPos[16] =
{
    //车的位置(0,0)
    { 0, 0, Chess::TYPE::CHE },

    //马的位置(1,0)
    { 1, 0, Chess::TYPE::MA },

    //相的位置(2,0)
    { 2, 0, Chess::TYPE::XIANG },

    //士的位置(3,0)
    { 3, 0, Chess::TYPE::SHI },

    //将的位置(4,0)
    { 4, 0, Chess::TYPE::JIANG },

    //士的位置(5,0)
    { 5, 0, Chess::TYPE::SHI },

    //相的位置(6,0)
    { 6, 0, Chess::TYPE::XIANG },

    //马的位置(7,0)
    { 7, 0, Chess::TYPE::MA },

    //车的位置(8,0)
    { 8, 0, Chess::TYPE::CHE },

    //炮的位置(1,2)
    { 1, 2, Chess::TYPE::PAO },

    //炮的位置(7,2)
    { 7, 2, Chess::TYPE::PAO },

    //兵的位置(0,3)
    { 0, 3, Chess::TYPE::BING },

    //兵的位置(2,3)
    { 2, 3, Chess::TYPE::BING },

    //兵的位置(4,3)
    { 4, 3, Chess::TYPE::BING },

    //兵的位置(6,3)
    { 6, 3, Chess::TYPE::BING },

    //兵的位置(8,3)
    { 8, 3, Chess::TYPE::BING },
};

float Chess::_offset = 45.0f;

Chess * Chess::create(int id, Chess::COLOR color)
{
    
    Chess* chess = new Chess();
    chess->init(id, color);
    chess->autorelease();
    return chess;
}

bool Chess::init(int id, Chess::COLOR color)
{
    this->_id = id;
    this->_color = color;

    if (_id < 16)
    {
        this->_type = _initPos[_id]._type;
    }
    else
    {
        this->_type = _initPos[_id - 16]._type;
        if (this->_color == Chess::COLOR::RED)
            this->_color = Chess::COLOR::BLACK;
        else
            this->_color = Chess::COLOR::RED;
    }


    const char * chessPic[14] = {
        "rshuai.png", //(红色)帅
        "rshi.png",   //(红色)士
        "rxiang.png", //(红色)相
        "rche.png",   //(红色)车
        "rma.png",    //(红色)马
        "rpao.png",   //(红色)炮
        "rbing.png",  //(红色)兵

        "bjiang.png", //(黑色)将
        "bshi.png",   //(黑色)士
        "bxiang.png", //(黑色)相
        "bche.png",   //(黑色)车
        "bma.png",    //(黑色)马
        "bpao.png",   //(黑色)炮
        "bzu.png"     //(黑色)卒
    };

    //黑子在上，每个相同的子相差7个id
    int picId = 7 * _color + _type;

    if(picId >= 0 && picId < 14)
        cocos2d::Sprite::initWithFile(chessPic[picId]);

    //setScale(0.8f);

    reset(_color);

    return true;
}

void Chess::reset(Chess::COLOR color)
{
    this->_dead = false;

    if (color == Chess::COLOR::RED)
    {//玩家的棋子的颜色为红色

        if (_id < 16)
        {
            this->setX(_initPos[_id]._x);
            this->setY(_initPos[_id]._y);
        }
        else
        {
            this->setX(_initPos[_id - 16]._x);
            this->setY(_initPos[_id - 16]._y);
        }
    }
    else
    {
        if (_id < 16)
        {
            this->setX(_initPos[_id]._x);
            this->setY(9 - _initPos[_id]._y);
        }
        else
        {
            this->setX(_initPos[_id - 16]._x);
            this->setY(9 - _initPos[_id - 16]._y);
        }
    }
}



void Chess::setX(int x)
{
    this->x = x;
}

void Chess::setY(int y)
{
    this->y = y;
}
int Chess::getX()
{
    return this->x;
}

int Chess::getY()
{
    return this->y;
}

int Chess::getID()
{
    return this->_id;
}
