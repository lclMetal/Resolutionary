#define FIELD_WIDTH 9
#define FIELD_HEIGHT 6

const int JEWEL_TYPES = 5;
const int JEWEL_WIDTH = 65;

const double JEWEL_SPEED = 3.0;

typedef struct coordStuct
{
    int x;
    int y;
}Coord;

typedef struct jewelStruct
{
    int jewelType;
    Actor *jp;
    Coord pos;
}Jewel;

Coord fieldStart;

Jewel gameField[FIELD_HEIGHT][FIELD_WIDTH];

Actor *swap1 = NULL;
Actor *swap2 = NULL;

int jewelsMoving = 0;
int result[FIELD_HEIGHT][FIELD_WIDTH];

Coord clickPos = { -1, -1 };
const int dragThreshold = 30;

int checkMatches(void);
void fixLevel(void);
void swapJewels(Coord from, Coord to);

void addJewel(Coord pos, int startY)
{
    Actor *jewel;
    int jewelType = (int)rand(JEWEL_TYPES);
    
    gameField[pos.y][pos.x].jewelType = jewelType;
    
    jewel = CreateActor("jewel", "jewel", "(none)", "(none)",
                fieldStart.x + pos.x * JEWEL_WIDTH, fieldStart.y - JEWEL_WIDTH * (1 + startY), true);
                
    MoveTo(jewel->clonename,
        fieldStart.x + pos.x * JEWEL_WIDTH,
        fieldStart.y + pos.y * JEWEL_WIDTH,
        JEWEL_SPEED * (pos.y + 1 + startY), "Game Center", "");
    jewelsMoving ++;
                
    jewel->type = jewelType;
    
    switch (jewel->type)
    {
        case 0: jewel->b = 0; break;
        case 1: jewel->g = jewel->b = 0; break;
        case 2: jewel->r = jewel->b = 0; break;
        case 3: jewel->r = 0; break;
        case 4: jewel->g = 0; break;
    }
    
    gameField[pos.y][pos.x].jp = jewel;
    gameField[pos.y][pos.x].pos.x = pos.x + 1;
    gameField[pos.y][pos.x].pos.y = pos.y + 1;
}

void startGame(Coord start)
{
    Coord pos;
    
    fieldStart = start;
 
    for (pos.y = 0; pos.y < FIELD_HEIGHT; pos.y ++)
    {
        for (pos.x = 0; pos.x < FIELD_WIDTH; pos.x ++)
        {
            addJewel(pos, FIELD_HEIGHT - 1 - pos.y);
        }
    }
}

Jewel *findJewel(Actor *ap)
{
    int i, j;
    
    for (i = 0; i < FIELD_HEIGHT; i ++)
    {
        for (j = 0; j < FIELD_WIDTH; j ++)
        {
            if (gameField[i][j].jp == ap)
                return &gameField[i][j];
        }
    }
    
    return NULL;
}

Coord jewelsDist(Jewel *jp1, Jewel *jp2)
{
    Coord coordDiff;
    
    coordDiff.x = (int)abs(jp1->pos.x - jp2->pos.x);
    coordDiff.y = (int)abs(jp1->pos.y - jp2->pos.y);
    
    return coordDiff;
}

int adjacent(Jewel *jp1, Jewel *jp2)
{
    Coord coordDiff = jewelsDist(jp1, jp2);
    
    return ((coordDiff.x == 1 && coordDiff.y == 0) || 
            (coordDiff.x == 0 && coordDiff.y == 1));
}

int trySwap(void)
{
    Jewel *j1;
    Jewel *j2;
    
    Jewel temp;
    
    Coord to;
    Coord from;
    
    if (!swap1 || !swap2)
    {
        swap1 = NULL;
        swap2 = NULL;

        return 1;
    }
    
    j1 = findJewel(swap1);
    j2 = findJewel(swap2);
    
    if (!j1 || !j2 || !adjacent(j1, j2))
    {
        swap1 = NULL;
        swap2 = NULL;
        
        return 1;
    }
    
    to.x = j1->pos.x - 1;
    to.y = j1->pos.y - 1;
    from.x = j2->pos.x - 1;
    from.y = j2->pos.y - 1;
    
    temp.jewelType = gameField[to.y][to.x].jewelType;
    temp.jp = gameField[to.y][to.x].jp;
    
    gameField[to.y][to.x].jewelType = gameField[from.y][from.x].jewelType;
    gameField[to.y][to.x].jp = gameField[from.y][from.x].jp;
    gameField[from.y][from.x].jewelType = temp.jewelType;
    gameField[from.y][from.x].jp = temp.jp;
    
    if (!checkMatches()) // Illegal move
    {
        temp.jewelType = gameField[to.y][to.x].jewelType;
        temp.jp = gameField[to.y][to.x].jp;
    
        gameField[to.y][to.x].jewelType = gameField[from.y][from.x].jewelType;
        gameField[to.y][to.x].jp = gameField[from.y][from.x].jp;
        gameField[from.y][from.x].jewelType = temp.jewelType;
        gameField[from.y][from.x].jp = temp.jp;
        
        swap1 = NULL;
        swap2 = NULL;
        
        return 1;
    }
    
    temp.jewelType = gameField[to.y][to.x].jewelType;
    temp.jp = gameField[to.y][to.x].jp;
    
    gameField[to.y][to.x].jewelType = gameField[from.y][from.x].jewelType;
    gameField[to.y][to.x].jp = gameField[from.y][from.x].jp;
    gameField[from.y][from.x].jewelType = temp.jewelType;
    gameField[from.y][from.x].jp = temp.jp;
    
    swapJewels(to, from);
    
    swap1 = NULL;
    swap2 = NULL;
    
    return 0;
}

void swapJewels(Coord from, Coord to)
{
    Actor *a, *a2;
    Jewel temp;
    Coord coordDiff;
    
    coordDiff.x = (int)abs(from.x - to.x);
    coordDiff.y = (int)abs(from.y - to.y);
    
    temp.jewelType = gameField[to.y][to.x].jewelType;
    temp.jp = gameField[to.y][to.x].jp;
    
    gameField[to.y][to.x].jewelType = gameField[from.y][from.x].jewelType;
    gameField[to.y][to.x].jp = gameField[from.y][from.x].jp;
    gameField[from.y][from.x].jewelType = temp.jewelType;
    gameField[from.y][from.x].jp = temp.jp;
    
    if (gameField[to.y][to.x].jewelType > -1)
    {
        a = getclone(gameField[to.y][to.x].jp->clonename);
        MoveTo(a->clonename, fieldStart.x + to.x * JEWEL_WIDTH, fieldStart.y + to.y * JEWEL_WIDTH,
            JEWEL_SPEED * (max(coordDiff.x, coordDiff.y) + 1), "Game Center", "");
        jewelsMoving ++;
    }
    
    if (gameField[from.y][from.x].jewelType > -1)
    {
        a2 = getclone(gameField[from.y][from.x].jp->clonename);
        MoveTo(a2->clonename, fieldStart.x + from.x * JEWEL_WIDTH, fieldStart.y + from.y * JEWEL_WIDTH,
            JEWEL_SPEED * (max(coordDiff.x, coordDiff.y) + 1), "Game Center", "");
        jewelsMoving ++;
    }
}

void moveJewel(Coord from, Coord to)
{
    Coord coordDiff;
    
    coordDiff.x = (int)abs(from.x - to.x);
    coordDiff.y = (int)abs(from.y - to.y);
    
    gameField[to.y][to.x].jewelType = gameField[from.y][from.x].jewelType;
    gameField[to.y][to.x].jp = gameField[from.y][from.x].jp;
    gameField[from.y][from.x].jewelType = -1;
    gameField[from.y][from.x].jp = NULL;
    
    MoveTo(gameField[to.y][to.x].jp->clonename,
        fieldStart.x + to.x * JEWEL_WIDTH, fieldStart.y + to.y * JEWEL_WIDTH,
        JEWEL_SPEED * (max(coordDiff.x, coordDiff.y) + 1), "Game Center", "");
    jewelsMoving ++;
}

void destroyJewel(Coord pos)
{
    DestroyActor(gameField[pos.y][pos.x].jp->clonename);
    gameField[pos.y][pos.x].jewelType = -1;
}

int checkMatches(void)
{
    int match = 0;
    int matchCount;
    int checkType;
    Coord checkPos;
    Coord checkOrigin;
    
    checkPos.x = 0;
    checkPos.y = 0;
    
    matchCount = 0;
    checkType = -1;
    checkOrigin = checkPos;
    
    for (checkPos.y = 0; checkPos.y < FIELD_HEIGHT; checkPos.y ++)
    {
        for (checkPos.x = 0; checkPos.x < FIELD_WIDTH; checkPos.x ++)
        {
            result[checkPos.y][checkPos.x] = 0;
        }
    }
    
    for (checkPos.y = 0; checkPos.y < FIELD_HEIGHT; checkPos.y ++)
    {
        for (checkPos.x = 0; checkPos.x < FIELD_WIDTH; checkPos.x ++)
        {
            if (checkPos.x == 0)
            {
                matchCount = 0; 
                checkType = gameField[checkPos.y][checkPos.x].jewelType;
                checkOrigin = checkPos;
            }
            else if (gameField[checkPos.y][checkPos.x].jewelType == checkType && checkPos.x < FIELD_WIDTH -1)
            {
                if (checkType > -1)
                    matchCount ++;
            }
            else if (gameField[checkPos.y][checkPos.x].jewelType != checkType || checkPos.x == FIELD_WIDTH -1)
            {
                if (checkPos.x == FIELD_WIDTH -1 && gameField[checkPos.y][checkPos.x].jewelType == checkType)
                {
                    if (checkType > -1)
                        matchCount ++;
                }
                
                if (matchCount < 2)
                {
                    int i;
                    
                    for (i = checkOrigin.x; i < checkPos.x; i ++)
                            result[checkOrigin.y][i] = 0;
                    
                    if (checkPos.x < FIELD_WIDTH -1)
                    {
                        matchCount = 0; 
                        checkType = gameField[checkPos.y][checkPos.x].jewelType;
                        checkOrigin = checkPos;   
                    }
                }
                else
                {
                    int i;
                    
                    match = 1;
                    
                    for (i = checkOrigin.x; i < checkPos.x + (gameField[checkPos.y][checkPos.x].jewelType == checkType); i ++)
                            result[checkOrigin.y][i] = 1;
                    
                    if (checkPos.x < FIELD_WIDTH -1)
                    {
                        matchCount = 0; 
                        checkType = gameField[checkPos.y][checkPos.x].jewelType;
                        checkOrigin = checkPos;   
                    }
                }
            }
        }
    }
    
    for (checkPos.x = 0; checkPos.x < FIELD_WIDTH; checkPos.x ++)
    {
        for (checkPos.y = 0; checkPos.y < FIELD_HEIGHT; checkPos.y ++)
        {
            if (checkPos.y == 0)
            {
                matchCount = 0; 
                checkType = gameField[checkPos.y][checkPos.x].jewelType;
                checkOrigin = checkPos;
            }
            else if (gameField[checkPos.y][checkPos.x].jewelType == checkType && checkPos.y < FIELD_HEIGHT -1)
            {
                if (checkType > -1)
                    matchCount ++;
            }
            else if (gameField[checkPos.y][checkPos.x].jewelType != checkType || checkPos.y == FIELD_HEIGHT -1)
            {
                if (checkPos.y == FIELD_HEIGHT -1 && gameField[checkPos.y][checkPos.x].jewelType == checkType)
                {
                    if (checkType > -1)
                        matchCount ++;
                }
                
                if (matchCount < 2)
                {
                    if (checkPos.y < FIELD_HEIGHT -1)
                    {
                        matchCount = 0; 
                        checkType = gameField[checkPos.y][checkPos.x].jewelType;
                        checkOrigin = checkPos;   
                    }
                }
                else
                {
                    int i;
                    
                    match = 1;
                    
                    for (i = checkOrigin.y; i < checkPos.y + (gameField[checkPos.y][checkPos.x].jewelType == checkType); i ++)
                            result[i][checkOrigin.x] = 1;
                    
                    if (checkPos.y < FIELD_HEIGHT -1)
                    {
                        matchCount = 0; 
                        checkType = gameField[checkPos.y][checkPos.x].jewelType;
                        checkOrigin = checkPos;   
                    }
                }
            }
        }
    }
    
    return match;
}

void fixLevel(void)
{
    Coord pos;
    Coord checkPos;
    
    for (checkPos.y = 0; checkPos.y < FIELD_HEIGHT; checkPos.y ++)
    {
        for (checkPos.x = 0; checkPos.x < FIELD_WIDTH; checkPos.x ++)
        {
            if (result[checkPos.y][checkPos.x])
            {
                destroyJewel(checkPos);
            }
        }
    }
    
    for (checkPos.x = 0; checkPos.x < FIELD_WIDTH; checkPos.x ++)
    {
        int newJewelCount = 0;
        
        for (checkPos.y = FIELD_HEIGHT - 1; checkPos.y >= 0; checkPos.y --)
        {
            if (gameField[checkPos.y][checkPos.x].jewelType == -1)
            {
                for (pos.y = checkPos.y; pos.y >= 0; pos.y --)
                {
                    if (gameField[pos.y][checkPos.x].jewelType > -1)
                    {
                        pos.x = checkPos.x;
                        swapJewels(pos, checkPos);
                        break;
                    }
                }
            }
        }
        
        for (checkPos.y = FIELD_HEIGHT - 1; checkPos.y >= 0; checkPos.y --)
        {
            if (gameField[checkPos.y][checkPos.x].jewelType == -1)
            {
                addJewel(checkPos, newJewelCount);
                newJewelCount ++;
            }
        }
    }
}












