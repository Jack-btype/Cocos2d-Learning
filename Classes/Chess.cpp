#include "Chess.h"

Chess::Chess()
{
}


//����һ�ű�
Chess::InitPos Chess::_initPos[16] =
{
    //����λ��(0,0)
    { 0, 0, Chess::TYPE::CHE },

    //���λ��(1,0)
    { 1, 0, Chess::TYPE::MA },

    //���λ��(2,0)
    { 2, 0, Chess::TYPE::XIANG },

    //ʿ��λ��(3,0)
    { 3, 0, Chess::TYPE::SHI },

    //����λ��(4,0)
    { 4, 0, Chess::TYPE::JIANG },

    //ʿ��λ��(5,0)
    { 5, 0, Chess::TYPE::SHI },

    //���λ��(6,0)
    { 6, 0, Chess::TYPE::XIANG },

    //���λ��(7,0)
    { 7, 0, Chess::TYPE::MA },

    //����λ��(8,0)
    { 8, 0, Chess::TYPE::CHE },

    //�ڵ�λ��(1,2)
    { 1, 2, Chess::TYPE::PAO },

    //�ڵ�λ��(7,2)
    { 7, 2, Chess::TYPE::PAO },

    //����λ��(0,3)
    { 0, 3, Chess::TYPE::BING },

    //����λ��(2,3)
    { 2, 3, Chess::TYPE::BING },

    //����λ��(4,3)
    { 4, 3, Chess::TYPE::BING },

    //����λ��(6,3)
    { 6, 3, Chess::TYPE::BING },

    //����λ��(8,3)
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
        "rshuai.png", //(��ɫ)˧
        "rshi.png",   //(��ɫ)ʿ
        "rxiang.png", //(��ɫ)��
        "rche.png",   //(��ɫ)��
        "rma.png",    //(��ɫ)��
        "rpao.png",   //(��ɫ)��
        "rbing.png",  //(��ɫ)��

        "bjiang.png", //(��ɫ)��
        "bshi.png",   //(��ɫ)ʿ
        "bxiang.png", //(��ɫ)��
        "bche.png",   //(��ɫ)��
        "bma.png",    //(��ɫ)��
        "bpao.png",   //(��ɫ)��
        "bzu.png"     //(��ɫ)��
    };

    //�������ϣ�ÿ����ͬ�������7��id
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
    {//��ҵ����ӵ���ɫΪ��ɫ

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
