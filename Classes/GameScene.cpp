#include "GameScene.h"
#include "string"
USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    // Some Preset
    this->_d = 60;
    this->_chess_offset = Vec2(60 + 23, 35 + 23);
    this->_select_id = -1;
    this->_turn = Chess::COLOR::RED;

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    //װ�����Ӻ�����
    Sprite * desk = Sprite::create("floor.jpg");
    desk->setPosition(Size(visibleSize.width / 2, visibleSize.height / 2));
    desk->setScaleX(visibleSize.width / desk->getContentSize().width);
    desk->setScaleY(visibleSize.height / desk->getContentSize().height);
    this->addChild(desk, 1);

    //debug = TextField::create();
    //debug->setString("I Am Bug");
    //debug->setFontSize(30);
    //debug->setPosition(Vec2(600, 500));
    //debug->setAnchorPoint(Vec2::ZERO);
    //this->addChild(debug, 1000000);

    //Turn = TextField::create();
    //Turn->setString("TURN");
    //Turn->setFontSize(30);
    //Turn->setPosition(Vec2(800, 300));
    //Turn->setAnchorPoint(Vec2::ZERO);
    //this->addChild(Turn, 1000000);

    //װ������
    auto _board_offset = Size(20, 15);
    Sprite * board = Sprite::create("background.png");
    board->setScale((visibleSize.height - _board_offset.height * 2) / board->getContentSize().height);
    board->setAnchorPoint(Vec2::ZERO);
    board->setPosition(_board_offset);
    this->addChild(board, 2);
    
    // ��ӵ���¼���ͨ��������������ӵ��ƶ�
    addTouchListener();
    
    // ���ѡ���
    _select_sprite = Sprite::create("selected.png");
    _select_sprite->setPosition(GameScene::getChessPos(2, 2));
    _select_sprite->setVisible(false);
    this->addChild(_select_sprite, 10);

    // �����Ϸ������
    _end_sprite = Sprite::create("yingjiemian.png");
    _end_sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    _end_sprite->setVisible(false);
    this->addChild(_end_sprite, 100);



    addMenuBar();


    // ������Ӳ��Ұ����Ƿ��ú�
    auto color = Chess::COLOR::RED;
    for (int i = 0; i < 32; i++)
    {
        _c[i] = Chess::create(i, color);
        _c[i]->setPosition(rand_0_1() * visibleSize);
        _c[i]->setVisible(false);
        this->addChild(_c[i], 5);
    }
    return true;
}


int GameScene::getChess(int x, int y)
{
    Chess * c;
    for (int i = 0; i < 32; i++)
    {
        c = _c[i];
        if (c->getX() == x && c->getY() == y && c->_dead == false)
        {
            return c->getID();
        }
    }
    return -1;
}

bool GameScene::getClickPos(Vec2 ptInWin, int & x, int & y)
{
    for (x = 0; x <= 8; x++)
    {
        for (y = 0; y <= 9; y++)
        {
            Vec2 ptInBoard = getChessPos(x, y);
            if (ptInWin.getDistance(ptInBoard) < _d / 2)
            {
                return true;
            }
        }
    }
    return false;
}

Vec2 GameScene::getChessPos(int x, int y)
{
    int xx = x * _d;
    int yy = y * _d;

    return Vec2(xx, yy) + _chess_offset;
}

void GameScene::addTouchListener()
{
    this->setTouchEnabled(true);
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool GameScene::onTouchBegan(Touch * touch, Event * event)
{
    auto clickPoint = touch->getLocation();

    if (_end_sprite->boundingBox().containsPoint(clickPoint) && _end_sprite->isVisible() == true)
    {
        //������Ϸ���
        _end_sprite->setVisible(false);

        newClick(this);
    }
    int x, y;
    if (!getClickPos(clickPoint, x, y))
    {
        return false;
    }

    int clickId = getChess(x, y);


    std::string toPrt = std::to_string(x) + ", " + std::to_string(y);
    //debug->setString(toPrt);


    if (-1 == _select_id)
    {
        if (clickId >= 0 && clickId < 32)
        {
            if (_c[clickId]->_color != _turn)
            {
                return false;
            }
            else {
                _select_sprite->setPosition(getChessPos(x, y));
                _select_sprite->setVisible(true);
                this->_select_id = clickId;
            }
        }
        else {
            return false;
        }
    }
    else if (-1 != _select_id){

        //�ƶ�����
        //��һ���������ƶ������ӵ�id
        //�ڶ���������ͨ���������λ���жϴ��������Ƿ�������
        //�������������������x����
        //���ĸ��������������y����
        //moveChessִ������������ѡ�������
        //ѡ���ӣ���_selectid == clickidʱ����ʾѡ����idΪ_selectid������
        //�����ӣ���selectid != clickidʱ�� ��ʾ��idΪ_selectid�������ƶ���(x,y)���ڵ�λ����
        moveChess(_select_id, clickId, x, y);
    }
    else
    {
        _select_id = -1;
    }

    return true;
}

void GameScene::moveChess(int moveId, int killId, int x, int y)
{
    // ����㵽�Լ�������һ�����ӣ�����ѡ���
    if (-1 != killId && _c[moveId]->_color == _c[killId]->_color)
    {
        this->_select_id = killId;
        _select_sprite->setPosition(getChessPos(x, y));
        _select_sprite->setVisible(true);

        return;
    }


    bool bCanMove = canMove(moveId, killId, x, y);

    if (false == bCanMove)
    {
        return;
    }

    _c[moveId]->setX(x);
    _c[moveId]->setY(y);

    auto moveTo = MoveTo::create(0.1f, getChessPos(x, y));
    auto callBackFunc = CallFunc::create([this, moveId, killId, x, y] {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        Sprite * moveChess = nullptr;
        if (moveId >= 0 && moveId < 32)
        {
            moveChess = _c[moveId];
            moveChess->setZOrder(moveChess->getZOrder() - 1);
        }
        if (-1 != killId)
        {
            _c[killId]->_dead = true;
            _c[killId]->setVisible(false);
            if (Chess::JIANG == _c[killId]->_type)
            {
                _end_sprite->setVisible(true);
            }
        }

        _select_sprite->setVisible(false);
        _select_id = -1;


        if (_turn == Chess::COLOR::RED)
        {
            //this->Turn->setString("Black");

            _turn = Chess::COLOR::BLACK;
        }
        else
        {
            //this->Turn->setString("Red");

            _turn = Chess::COLOR::RED;
        }
    });
    auto seq = Sequence::create(moveTo, callBackFunc,  nullptr);

    _c[moveId]->setZOrder(_c[moveId]->getZOrder() + 1);

    _c[moveId]->runAction(seq);
}


bool GameScene::canMove(int moveId, int killId, int x, int y)
{
    Chess * c;
    if (moveId >= 0 && moveId < 32)
    {
        c = _c[moveId];
    }
    else
    {
        return false;
    }

    switch (c->_type)
    {
    case Chess::TYPE::JIANG:
        return canMoveJiang(moveId, killId, x, y);
        break;
    case Chess::TYPE::SHI:
        return canMoveShi(moveId, killId, x, y);
        break;
    case Chess::TYPE::XIANG:
        return canMoveXiang(moveId, killId, x, y);
        break;
    case Chess::TYPE::CHE:
        return canMoveChe(moveId, killId, x, y);
        break;
    case Chess::TYPE::MA:
        return canMoveMa(moveId, killId, x, y);
        break;
    case Chess::TYPE::PAO:
        return canMovePao(moveId, killId, x, y);
        break;
    case Chess::TYPE::BING:
        return canMoveBing(moveId, killId, x, y);
        break;
    default:
        break;
    }

    return false;
}

bool GameScene::canMoveJiang(int moveId, int killId, int x, int y)
{
    if (killId >= 0 && killId < 32)
    {
        Chess* skill = _c[killId];
        if (skill->_type == Chess::TYPE::JIANG)
        {
            return canMoveChe(moveId, killId, x, y);
        }
    }

    //�����������
    //1��һ����һ��
    //2�����ܳ��Ź���


    std::string line1 = "x=" + std::to_string(x) + ", y=" + std::to_string(y);
    std::string line2 = "moveId=" + std::to_string(moveId) + ", killId=" + std::to_string(killId);
    std::string toPrt = line1 + "\n" + line2;
    //debug->setString(toPrt);
    //CCLog("x=%d, y=%d", x, y);
    //CCLog("moveid=%d, killid=%d", moveId, killId);

    //���Ķ�ɱ


    //ͨ�����ӵ�ID�õ�����
    Chess* c = _c[moveId];

    //��ý���ǰ��λ��
    int xo = c->getX();
    int yo = c->getY();

    //��ý��ߵĸ���
    //(x,y)��ʾ���ߵ���λ��
    int xoff = abs(xo - x);
    int yoff = abs(yo - y);

    int d = xoff * 10 + yoff;

    //�߽���ʱ�����������
    //xoff=1, yoff=0�������������
    //xoff=0, yoff=1������ǰ�����
    if (d != 1 && d != 10)
    {
        return false;
    }

    //�жϽ��Ƿ���˾Ź�
    //��ɫ�Ľ��ͺ�ɫ�Ľ���x����ķ�Χ����3<=x<=5
    if (x<3 || x>5)
    {
        return false;
    }

    //�����ҵ������Ǻ���
    if ((_c[moveId]->_color == Chess::COLOR::RED) && _turn == Chess::COLOR::RED)
    {
        //�жϽ��Ƿ���˾Ź�
        if (y<0 || y>2)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else if ((_c[moveId]->_color == Chess::COLOR::BLACK) && _turn == Chess::COLOR::BLACK)
    {
        //�жϽ��Ƿ���˾Ź�
        if (y>9 || y<7)
        {
            return false;
        }
        else {
            return true;
        }
    }
    else {
        return false;
    }
}

bool GameScene::canMoveShi(int moveId, int killId, int x, int y)
{
    //ʿ���������
    //1��һ����һ��
    //2�����ܳ��Ź���
    //3��б����

    //ͨ�����ӵ�ID�õ�����
    Chess* c = _c[moveId];

    //���������ǰ��λ��
    int xo = c->getX();
    int yo = c->getY();

    //����ߵĸ���
    //(x,y)��ʾ���ߵ���λ��
    int xoff = abs(xo - x);
    int yoff = abs(yo - y);

    int d = xoff * 10 + yoff;

    //ʿÿ��һ��x������1��,y������1��
    //���ߵĸ�������1��ʱ
    //����false
    if (d != 11)
    {
        return false;
    }

    //�ж�ʿ�Ƿ���˾Ź�
    //��ɫ��ʿ�ͺ�ɫ��ʿ��x����ķ�Χ����3<=x<=5
    if (x<3 || x>5)
    {
        return false;
    }

    //�����ҵ������Ǻ���
    if ((_c[moveId]->_color == Chess::COLOR::RED) && _turn == Chess::COLOR::RED)
    {
        //�жϽ��Ƿ���˾Ź�
        if (y<0 || y>2)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else if ((_c[moveId]->_color == Chess::COLOR::BLACK) && _turn == Chess::COLOR::BLACK)
    {
        //�жϽ��Ƿ���˾Ź�
        if (y>9 || y<7)
        {
            return false;
        }
        else {
            return true;
        }
    }
    else {
        return false;
    }
}

bool GameScene::canMoveXiang(int moveId, int killId, int x, int y)
{
    //����������
    //ÿ��һ��x�ƶ�2��,y�ƶ�2��
    //���ܹ���


    //ͨ�����ӵ�ID�õ�����
    Chess* c = _c[moveId];

    //���������ǰ��λ��
    int xo = c->getX();
    int yo = c->getY();

    //������ߵĸ���
    //(x,y)��ʾ���ߵ���λ��
    int xoff = abs(xo - x);
    int yoff = abs(yo - y);

    int d = xoff * 10 + yoff;

    //��ÿһ��x������2����,y������2��
    //���ߵĸ�������2��ʱ
    //����false
    if (d != 22)
    {
        return false;
    }

    //��������������е�����
    int xm = (xo + x) / 2;
    int ym = (yo + y) / 2;

    //�õ�(xm,ym)�ϵ�����
    int id = getChess(xm, ym);

    //��(xm,ym)�������ӵ�ʱ��
    if (id != -1)
    {
        //��������
        return false;
    }

    //�����಻�ܹ���
    //�����ҵ������Ǻ���
    if (c->_color == Chess::COLOR::RED)
    {
        //�ж����Ƿ���˺�
        if (y > 4)
        {
            return false;
        }
    }
    else//�жϺ�ɫ����ķ�Χ
    {
        //�ж����Ƿ���˺�
        if (y < 5)
        {
            return false;
        }
    }

    return true;
}

bool GameScene::canMoveChe(int moveId, int killId, int x, int y)
{
    Chess * c = _c[moveId];

    //��ó�����ǰ��λ��
    int xo = c->getX();
    int yo = c->getY();

    //������֮�������ӵ�ʱ�򳵲�����
    if (getChessCount(xo, yo, x, y) != 0)
    {
        return false;
    }

    return true;
}

bool GameScene::canMoveMa(int moveId, int killId, int x, int y)
{
    //ͨ�����ӵ�ID�õ�����
    Chess* c = _c[moveId];

    //���������ǰ��λ��
    int xo = c->getX();
    int yo = c->getY();

    //������ߵĸ���
    //(x,y)��ʾ���ߵ���λ��
    //�������������
    //��һ�������������ǰ�������1�����������������2��
    //�ڶ�����������������������1��������ǰ�������2��
    int xoff = abs(xo - x);
    int yoff = abs(yo - y);

    //CCLog("x=%d", x);
    //CCLog("y=%d", y);

    int d = xoff * 10 + yoff;

    //CCLog("d=%d", d);

    if (d != 12 && d != 21)
    {
        return false;
    }

    int xm, ym;//��¼��ŵ�����

    if (d == 12)//�����ߵ��ǵ�һ�����
    {
        xm = xo;//��ŵ��x����Ϊ����ǰ���x����
        ym = (yo + y) / 2;//��ŵ��y����Ϊ����ǰ���y�������������y������е�����
    }
    else//�����ߵ��ǵڶ������
    {
        xm = (xo + x) / 2;//��ŵ��x����Ϊ����ǰ���x�������������x������е�����
        ym = yo;;//��ŵ��y����Ϊ����ǰ���y����
    }

    //CCLog("xm=%d", xm);
    //CCLog("ym=%d", ym);

    //����ŵ�������ʱ,������
    if (getChess(xm, ym) != -1)
    {
        return false;
    }

    return true;
}

bool GameScene::canMovePao(int moveId, int killId, int x, int y)
{
    //ͨ�����ӵ�ID�õ�����
    Chess* c = _c[moveId];

    //���������ǰ��λ��
    int xo = c->getX();
    int yo = c->getY();

    //������������һ������
    //��������֮��ֻ��һ�����ӵ�ʱ��
    //�ڳԵ��������ϵ�����
    if (killId != -1 && this->getChessCount(xo, yo, x, y) == 1)
    {
        return true;
    }

    if (killId == -1 && this->getChessCount(xo, yo, x, y) == 0)
    {
        return true;
    }

    return false;
}

bool GameScene::canMoveBing(int moveId, int killId, int x, int y)
{
    //�����������
    //1��һ����һ��
    //2��ǰ��һ����ܺ���
    //3�����Ӻ�ſ��������ƶ�

    //ͨ�����ӵ�ID�õ�����
    Chess* c = _c[moveId];

    //��ý���ǰ��λ��
    int xo = c->getX();
    int yo = c->getY();

    //��ñ��ߵĸ���
    //(x,y)��ʾ���ߵ���λ��
    int xoff = abs(xo - x);
    int yoff = abs(yo - y);

    int d = xoff * 10 + yoff;

    //�߽���ʱ�����������
    //xoff=1, yoff=0�������������
    //xoff=0, yoff=1������ǰ�����
    if (d != 1 && d != 10)
    {
        return false;
    }


    if (_c[moveId]->_color == Chess::COLOR::RED)
    {
        //���ƺ�ɫ�ı����ܺ���
        if (y < yo)
        {
            return false;
        }

        //��ɫ�ı�û�й��Ӳ��������ƶ�
        if (yo <= 4 && y == yo)
        {
            return false;
        }
    }
    else//�жϺ�ɫ�ı�
    {
        //���ƺ�ɫ�ı����ܺ���
        if (y > yo)
        {
            return false;
        }

        //��ɫ�ı�û�й��Ӳ��������ƶ�
        if (yo >= 5 && y == yo)
        {
            return false;
        }
    }

    return true;
}

// ����������֮�����������
// ����-1�����Ϸ�������
int GameScene::getChessCount(int xo, int yo, int x, int y)
{
    int ret = 0;

    //(xo,yo)��(x,y)����ͬһ��ֱ����
    if (xo != x && yo != y)
    {
        return -1;
    }

    // (xo, yo)��(x, y)��ͬһ����
    if (xo == x && yo == y)
    {
        return -1;
    }

    //������ͬһ��������
    if (xo == x)
    {
        //minΪ��������y������С�ĵ��y����
        int min = yo < y ? yo : y;

        //maxΪ��������y�������ĵ��y����
        int max = yo > y ? yo : y;

        //����ͬһ������������֮���������
        for (int yy = min + 1; yy<max; yy++)
        {
            //������֮�������ӵ�ʱ��
            if (getChess(x, yy) != -1)
            {
                ++ret;//��������1
            }
        }
    }
    else//������ͬһ��������yo == y
    {
        //minΪ��������x������С�ĵ��x����
        int min = xo < x ? xo : x;

        //maxΪ��������x�������ĵ��x����
        int max = xo > x ? xo : x;

        //����ͬһ������������֮���������
        for (int xx = min + 1; xx<max; xx++)
        {
            //������֮�������ӵ�ʱ��
            if (getChess(xx, y) != -1)
            {
                ++ret;//��������1
            }
        }
    }


    return ret;
}

void GameScene::addMenuBar()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    auto buttonStart = MenuItemImage::create("start.jpg", "start.jpg", CC_CALLBACK_1(GameScene::startClick, this));
    buttonStart->setPosition(Vec2(800, 560));

    auto buttonNew = MenuItemImage::create("new.jpg", "new.jpg", CC_CALLBACK_1(GameScene::newClick, this));
    buttonNew->setPosition(Vec2(800, 460));

    auto menu = Menu::create(buttonNew, buttonStart, nullptr);
    menu->setPosition(Vec2::ZERO);
    
    
    this->addChild(menu, 10000);
}

void GameScene::startClick(Ref * pSender)
{
    for (int i = 0; i < 32; i++)
    {
        _c[i]->setVisible(true);

        auto move = MoveTo::create(1.0f, this->getChessPos(_c[i]->getX(), _c[i]->getY()));
        _c[i]->runAction(move);
        
    }
}

void GameScene::newClick(Ref * pSender)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();




    for (int i = 0; i < 32; i++)
    {
        if (i < 16)
        {
            _c[i]->reset(Chess::COLOR::RED);
        }
        else
        {
            _c[i]->reset(Chess::COLOR::BLACK);
        }
        _c[i]->setPosition(rand_0_1() * visibleSize);
        _c[i]->setVisible(false);
    }
}



