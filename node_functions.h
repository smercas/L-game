#ifndef trail_functions
#define trail_functions

void initT()
{
    for (int i = 0; i < tl; i++)
    {
        t[i].x = -1;
        t[i].y = -1;
    }
    tl = 0;
}

void tPush(int x, int y)
{
    t[tl].x = x;
    t[tl].y = y;
    tl++;
}

void tPop()
{
    t[tl - 1].x = 0;
    t[tl - 1].y = 0;
    tl--;
}

bool notInT(int x, int y)
{
    for (int i = 0; i < tl; i++)
    {
        if (t[i].x == x && t[i].y == y) return 0;
    }
    return 1;
}

void initPM()
{
    for (int i = 0; i < pml; i++)
    {
        pm[i].x = -1;
        pm[i].y = -1;
        pm[i].m = -1;
    }
    pml = 0;
}

void pmPush(int x, int y, int m)
{
    pm[pml].x = x;
    pm[pml].y = y;
    pm[pml].m = m;
    pml++;
}

void setEC(bool v)
{
    int c = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (game[v][i][j] == 'e')
            {
                ec[c].x = i;
                ec[c].y = j;
                c++;
            }
        }
    }
}

void setNP()
{
    int c = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (game[0][i][j] == 'n')
            {
                np[c].x = i;
                np[c].y = j;
                c++;
            }
        }
    }
}

void gpPush()
{
    BOARD *A = new BOARD;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            A -> g[i][j] = game[0][i][j];
        }
    }
    A -> next = NULL;
    if (gp.l == 0)
    {
        gp.first = A;
    }
    else
    {
        gp.last -> next = A;
    }
    A -> prev = gp.last;
    gp.last = A;
    gp.l++;
}

void gpDeleteFrom(BOARD *X)
{
    BOARD *A = gp.last;
    while (gp.last != X)
    {
        gp.last = gp.last -> prev;
        delete A;
        A = gp.last;
        gp.l--;
    }
    gp.last -> next = NULL;
}

void gpDelete()
{
    if (gp.first)
    {
        gpDeleteFrom(gp.first);
        delete gp.first;
    }
}

void initGP()
{
    gpDelete();
    gp.l = 0;
    gp.first = NULL;
    gp.last = NULL;
    gpPush();
    cgp = gp.first;
}

void addGPToStack()
{
    if (cgp != gp.last && st[3].v) gpDeleteFrom(cgp);
    gpPush();
    cgp = cgp -> next;
}

SAVE* llAdd(char name[], char type[], bool turnLimitActive, unsigned long long turnLimitCopy, bool turn)
{
    SAVE *A = new SAVE;
    strcpy(A -> name, name);
    strcpy(A -> type, type);
    A -> turnLimitActive = turnLimitActive;
    A -> turnLimitCopy = turnLimitCopy;
    A -> turn = turn;
    if (ll.l == 0)
    {
        A -> prev = NULL;
        A -> next = NULL;
        ll.first = A;
        ll.last = A;
    }
    else
    {
        SAVE *B = ll.first;
        while (strcmp(B -> name, A -> name) < 0 && B -> next != NULL)
        {
            B = B -> next;
        }
        if (strcmp(B -> name, A -> name) == 0)
        {
            unsigned long long c = 0;
            while (strcmp(B -> name, A -> name) == 0 && B -> next != NULL)
            {
                c++;
                convToChar(c);
                strcpy(A -> name, name);
                strcat(A -> name, "(");
                strcat(A -> name, number);
                strcat(A -> name, ")");
                B = B -> next;
            }
            if (strcmp(B -> name, A -> name) == 0 && ll.last == B)
            {
                c++;
                convToChar(c);
                strcpy(A -> name, name);
                strcat(A -> name, "(");
                strcat(A -> name, number);
                strcat(A -> name, ")");
                A -> prev = B;
                A -> next = B -> next;
                B -> next = A;
                ll.last = A;
            }
            else
            {
                B = B -> prev;
                A -> next = B -> next;
                B -> next -> prev = A;
                A -> prev = B;
                B -> next = A;
            }
        }
        else if (B == ll.first && strcmp(B -> name, A -> name) > 0)
        {
            A -> next = B;
            A -> prev = B -> prev;
            B -> prev = A;
            ll.first = A;
        }
        else if (B == ll.last && strcmp(B -> name, A -> name) < 0)
        {
            A -> prev = B;
            A -> next = B -> next;
            B -> next = A;
            ll.last = A;
        }
        else
        {
            B = B -> prev;
            A -> next = B -> next;
            B -> next -> prev = A;
            A -> prev = B;
            B -> next = A;
        }
    }
    ll.l++;
    return A;
}

void llDelete()
{
    SAVE *A = llpos;
    SAVE *I;
    if (ll.l != 1)
    {
        if (A -> prev == NULL)
        {
            ll.first = A -> next;
            ll.first -> prev = NULL;
            llpos = llpos -> next;
            llfs = llfs -> next;
        }
        else if (A -> next == NULL)
        {
            ll.last = A -> prev;
            ll.last -> next = NULL;
            llpos = llpos -> prev;
            if (llfs -> prev == NULL)
            {
                llrpos--;
            }
            else
            {
                llfs = llfs -> prev;
            }
        }
        else
        {
            A -> prev -> next = A -> next;
            A -> next -> prev = A -> prev;
            if (ll.l > 7 && llfs -> next -> next -> next -> next -> next -> next == NULL)
            {
                llfs = llfs -> prev;
                llpos = llpos -> prev;
            }
            else
            {
                llpos = llpos -> next;
            }
        }
    }
    else
    {
        ll.first = NULL;
        ll.last = NULL;
    }
    delete A;
    ll.l--;
}

void initLL()
{
    SAVE *A = ll.last;
    while (ll.last != ll.first)
    {
        ll.last = ll.last -> prev;
        delete A;
        A = ll.last;
        ll.l--;
    }
    delete ll.first;
    ll.l = NULL;
    ll.first = NULL;
    ll.last = NULL;
}

void mpAdd(char a[672][4][4], int &l, char s[4][4], bool t)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (t) a[l][i][j] = s[i][j];
            else
            {
                if (s[i][j] == 'b') a[l][i][j] = 'r';
                else if (s[i][j] == 'r') a[l][i][j] = 'b';
                else a[l][i][j] = s[i][j];
            }
        }
    }
    l++;
}

void initMP(char a[672][4][4], int &l)
{
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                a[i][j][k] = -1;
            }
        }
    }
    l = 0;
}

int mpPick(char a[672][4][4], int l, char s[])
{
    int r = rand() % l, n = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            game[0][i][j] = a[r][i][j];
        }
    }
    turn = !turn;
    if (st[6].v) turnLimitCopy--;
    addGPToStack();
    paintBoard();
    makeMessage(s);
    if (st[6].v) paintNumberOfTurns();
    swapbuffers();
    paintBoard();
    bar(0, 100, 1200, 200);
    outtextxy(600, 160, message);
    if (st[6].v) paintNumberOfTurns();
    return n;
}
#endif
