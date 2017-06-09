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


    //装载桌子和棋盘
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

    //装载棋盘
    auto _board_offset = Size(20, 15);
    Sprite * board = Sprite::create("background.png");
    board->setScale((visibleSize.height - _board_offset.height * 2) / board->getContentSize().height);
    board->setAnchorPoint(Vec2::ZERO);
    board->setPosition(_board_offset);
    this->addChild(board, 2);
    
    // 添加点击事件，通过鼠标来控制棋子的移动
    addTouchListener();
    
    // 添加选择框
    _select_sprite = Sprite::create("selected.png");
    _select_sprite->setPosition(GameScene::getChessPos(2, 2));
    _select_sprite->setVisible(false);
    this->addChild(_select_sprite, 10);

    // 添加游戏结束框
    _end_sprite = Sprite::create("yingjiemian.png");
    _end_sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    _end_sprite->setVisible(false);
    this->addChild(_end_sprite, 100);



    addMenuBar();


    // 添加棋子并且把它们放置好
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
        //隐藏游戏结果
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

        //移动棋子
        //第一个参数：移动的棋子的id
        //第二个参数：通过触摸点的位置判断触摸点上是否有棋子
        //第三个参数：触摸点的x坐标
        //第四个参数：触摸点的y坐标
        //moveChess执行了两个步骤选棋和走棋
        //选棋子：当_selectid == clickid时，表示选定了id为_selectid的棋子
        //走棋子：当selectid != clickid时， 表示将id为_selectid的棋子移动到(x,y)所在的位置上
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
    // 如果点到自己方的另一个棋子，更换选择框
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

    //将的走棋规则：
    //1、一次走一格
    //2、不能出九宫格


    std::string line1 = "x=" + std::to_string(x) + ", y=" + std::to_string(y);
    std::string line2 = "moveId=" + std::to_string(moveId) + ", killId=" + std::to_string(killId);
    std::string toPrt = line1 + "\n" + line2;
    //debug->setString(toPrt);
    //CCLog("x=%d, y=%d", x, y);
    //CCLog("moveid=%d, killid=%d", moveId, killId);

    //将的对杀


    //通过棋子的ID得到棋子
    Chess* c = _c[moveId];

    //获得将当前的位置
    int xo = c->getX();
    int yo = c->getY();

    //获得将走的格数
    //(x,y)表示将走到的位置
    int xoff = abs(xo - x);
    int yoff = abs(yo - y);

    int d = xoff * 10 + yoff;

    //走将的时候有两种情况
    //xoff=1, yoff=0：将向左或向右
    //xoff=0, yoff=1：将向前或向后
    if (d != 1 && d != 10)
    {
        return false;
    }

    //判断将是否出了九宫
    //红色的将和黑色的将的x坐标的范围都是3<=x<=5
    if (x<3 || x>5)
    {
        return false;
    }

    //如果玩家的棋子是红棋
    if ((_c[moveId]->_color == Chess::COLOR::RED) && _turn == Chess::COLOR::RED)
    {
        //判断将是否出了九宫
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
        //判断将是否出了九宫
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
    //士的走棋规则：
    //1、一次走一格
    //2、不能出九宫格
    //3、斜着走

    //通过棋子的ID得到棋子
    Chess* c = _c[moveId];

    //获得相走棋前的位置
    int xo = c->getX();
    int yo = c->getY();

    //获得走的格数
    //(x,y)表示将走到的位置
    int xoff = abs(xo - x);
    int yoff = abs(yo - y);

    int d = xoff * 10 + yoff;

    //士每走一步x方向走1格,y方向走1格
    //当走的格数大于1格时
    //返回false
    if (d != 11)
    {
        return false;
    }

    //判断士是否出了九宫
    //红色的士和黑色的士的x坐标的范围都是3<=x<=5
    if (x<3 || x>5)
    {
        return false;
    }

    //如果玩家的棋子是红棋
    if ((_c[moveId]->_color == Chess::COLOR::RED) && _turn == Chess::COLOR::RED)
    {
        //判断将是否出了九宫
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
        //判断将是否出了九宫
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
    //相的走棋规则：
    //每走一次x移动2格,y移动2格
    //不能过河


    //通过棋子的ID得到棋子
    Chess* c = _c[moveId];

    //获得相走棋前的位置
    int xo = c->getX();
    int yo = c->getY();

    //获得相走的格数
    //(x,y)表示将走到的位置
    int xoff = abs(xo - x);
    int yoff = abs(yo - y);

    int d = xoff * 10 + yoff;

    //相每一次x方向走2格子,y方向走2格
    //当走的格数大于2格时
    //返回false
    if (d != 22)
    {
        return false;
    }

    //计算两个坐标的中点坐标
    int xm = (xo + x) / 2;
    int ym = (yo + y) / 2;

    //得到(xm,ym)上的棋子
    int id = getChess(xm, ym);

    //当(xm,ym)上有棋子的时候
    if (id != -1)
    {
        //不能走相
        return false;
    }

    //限制相不能过河
    //如果玩家的棋子是红棋
    if (c->_color == Chess::COLOR::RED)
    {
        //判断相是否过了河
        if (y > 4)
        {
            return false;
        }
    }
    else//判断黑色的相的范围
    {
        //判断相是否过了河
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

    //获得车走棋前的位置
    int xo = c->getX();
    int yo = c->getY();

    //当两点之间有棋子的时候车不能走
    if (getChessCount(xo, yo, x, y) != 0)
    {
        return false;
    }

    return true;
}

bool GameScene::canMoveMa(int moveId, int killId, int x, int y)
{
    //通过棋子的ID得到棋子
    Chess* c = _c[moveId];

    //获得马走棋前的位置
    int xo = c->getX();
    int yo = c->getY();

    //获得马走的格数
    //(x,y)表示马走到的位置
    //马有两种情况：
    //第一种情况：马先向前或向后走1步，再向左或向右走2步
    //第二种情况：马先向左或向右走1不，再向前或向后走2步
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

    int xm, ym;//记录绑脚点坐标

    if (d == 12)//当马走的是第一种情况
    {
        xm = xo;//绑脚点的x坐标为走棋前马的x坐标
        ym = (yo + y) / 2;//绑脚点的y坐标为走棋前马的y坐标和走棋后马的y坐标的中点坐标
    }
    else//当马走的是第二种情况
    {
        xm = (xo + x) / 2;//绑脚点的x坐标为走棋前马的x坐标和走棋后马的x坐标的中点坐标
        ym = yo;;//绑脚点的y坐标为走棋前马的y坐标
    }

    //CCLog("xm=%d", xm);
    //CCLog("ym=%d", ym);

    //当绑脚点有棋子时,不能走
    if (getChess(xm, ym) != -1)
    {
        return false;
    }

    return true;
}

bool GameScene::canMovePao(int moveId, int killId, int x, int y)
{
    //通过棋子的ID得到棋子
    Chess* c = _c[moveId];

    //获得炮走棋前的位置
    int xo = c->getX();
    int yo = c->getY();

    //当触摸点上有一个棋子
    //而且两点之间只有一个棋子的时候
    //炮吃掉触摸点上的棋子
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
    //兵的走棋规则：
    //1、一次走一格
    //2、前进一格后不能后退
    //3、过河后才可以左右移动

    //通过棋子的ID得到棋子
    Chess* c = _c[moveId];

    //获得将当前的位置
    int xo = c->getX();
    int yo = c->getY();

    //获得兵走的格数
    //(x,y)表示将走到的位置
    int xoff = abs(xo - x);
    int yoff = abs(yo - y);

    int d = xoff * 10 + yoff;

    //走将的时候有两种情况
    //xoff=1, yoff=0：将向左或向右
    //xoff=0, yoff=1：将向前或向后
    if (d != 1 && d != 10)
    {
        return false;
    }


    if (_c[moveId]->_color == Chess::COLOR::RED)
    {
        //限制红色的兵不能后退
        if (y < yo)
        {
            return false;
        }

        //红色的兵没有过河不能左右移动
        if (yo <= 4 && y == yo)
        {
            return false;
        }
    }
    else//判断黑色的兵
    {
        //限制黑色的兵不能后退
        if (y > yo)
        {
            return false;
        }

        //黑色的兵没有过河不能左右移动
        if (yo >= 5 && y == yo)
        {
            return false;
        }
    }

    return true;
}

// 返回两个点之间的棋子数量
// 返回-1代表不合法的输入
int GameScene::getChessCount(int xo, int yo, int x, int y)
{
    int ret = 0;

    //(xo,yo)和(x,y)不在同一条直线上
    if (xo != x && yo != y)
    {
        return -1;
    }

    // (xo, yo)和(x, y)在同一点上
    if (xo == x && yo == y)
    {
        return -1;
    }

    //两点在同一条竖线上
    if (xo == x)
    {
        //min为两个点中y坐标最小的点的y坐标
        int min = yo < y ? yo : y;

        //max为两个点中y坐标最大的点的y坐标
        int max = yo > y ? yo : y;

        //查找同一条竖线上两点之间的棋子数
        for (int yy = min + 1; yy<max; yy++)
        {
            //当两点之间有棋子的时候
            if (getChess(x, yy) != -1)
            {
                ++ret;//棋子数加1
            }
        }
    }
    else//两点在同一条横线上yo == y
    {
        //min为两个点中x坐标最小的点的x坐标
        int min = xo < x ? xo : x;

        //max为两个点中x坐标最大的点的x坐标
        int max = xo > x ? xo : x;

        //查找同一条竖线上两点之间的棋子数
        for (int xx = min + 1; xx<max; xx++)
        {
            //当两点之间有棋子的时候
            if (getChess(xx, y) != -1)
            {
                ++ret;//棋子数加1
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



