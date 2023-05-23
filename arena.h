#ifndef arena
#define arena

void setBothColors()
{
    backgroundColor = COLOR(bgol[0].n, bgol[1].n, bgol[2].n);
    outlineColor = COLOR(bgol[3].n, bgol[4].n, bgol[5].n);
    setbkcolor(backgroundColor);
    setcolor(outlineColor);
    setfillstyle(1, backgroundColor);
}

void setBothPages(void f())
{
    f();
    swapbuffers();
    f();
}

void setBothPages(void f(char[]), char s[])
{
    f(s);
    swapbuffers();
    f(s);
}

void paintNumberOfTurns()
{
    if (st[0].v)
    {
        outtextxy(1050, 410, "Number of");
        outtextxy(1050, 450, "turns left:");
    }
    else
    {
        outtextxy(1050, 410, "Numar de");
        outtextxy(1050, 450, "ture ramase:");
    }
    bar (901, 470, 1200, 510);
    convToChar(turnLimitCopy / 2);
    outtextxy(1050, 490, number);
}

void initSC()
{
    sc[0][0].x = 300; sc[0][0].y = 200;
    sc[0][1].x = 300; sc[0][1].y = 350;
    sc[0][2].x = 300; sc[0][2].y = 500;
    sc[0][3].x = 300; sc[0][3].y = 650;

    sc[1][0].x = 450; sc[1][0].y = 200;
    sc[1][1].x = 450; sc[1][1].y = 350;
    sc[1][2].x = 450; sc[1][2].y = 500;
    sc[1][3].x = 450; sc[1][3].y = 650;

    sc[2][0].x = 600; sc[2][0].y = 200;
    sc[2][1].x = 600; sc[2][1].y = 350;
    sc[2][2].x = 600; sc[2][2].y = 500;
    sc[2][3].x = 600; sc[2][3].y = 650;

    sc[3][0].x = 750; sc[3][0].y = 200;
    sc[3][1].x = 750; sc[3][1].y = 350;
    sc[3][2].x = 750; sc[3][2].y = 500;
    sc[3][3].x = 750; sc[3][3].y = 650;
}

void paintBoardLines()
{
    line (300, 200, 900, 200);
    line (300, 350, 900, 350);
    line (300, 500, 900, 500);
    line (300, 650, 900, 650);
    line (300, 800, 900, 800);

    line (300, 200, 300, 800);
    line (450, 200, 450, 800);
    line (600, 200, 600, 800);
    line (750, 200, 750, 800);
    line (900, 200, 900, 800);
}

bool hoveringOverBoard_s()
{
    return mousex() > 300 && mousex() < 900 && mousey() > 200 && mousey() < 800;
}

bool hoveringOver(COORDINATES A)
{
    return mousex() > A.x && mousex() < A.x + 150 && mousey() > A.y && mousey() < A.y + 150;
}

bool hoveringOverBoard_c(int x, int y)
{
    return x >= 0 && x < 4 && y >= 0 && y < 4;
}

void setCellCoord(int &x, int &y)
{
    x = mousex(); y = mousey();
    if (hoveringOverBoard_s())
    {
        x = (mousex() - 300) / 150;
        y = (mousey() - 200) / 150;
    }
    else
    {
        x = -1;
        y = -1;
    }
}

void paintCell(COORDINATES A, char c, bool semi)
{
    if (c == 'e') bar(A.x + 1, A.y + 1, A.x + 150, A.y + 150);
    else if (c == 'r')
    {
        if (semi) setfillpattern(pattern, COLOR(p12[0].n, p12[1].n, p12[2].n));
        else setfillstyle(1, COLOR(p12[0].n, p12[1].n, p12[2].n));
        bar(A.x + 1, A.y + 1, A.x + 150, A.y + 150);
        setfillstyle(1, backgroundColor);
    }
    else if (c == 'b')
    {
        if (semi) setfillpattern(pattern, COLOR(p12[3].n, p12[4].n, p12[5].n));
        else setfillstyle(1, COLOR(p12[3].n, p12[4].n, p12[5].n));
        bar(A.x + 1, A.y + 1, A.x + 150, A.y + 150);
        setfillstyle(1, backgroundColor);
    }
    else if (c == 'n')
    {
        bar(A.x + 1, A.y + 1, A.x + 150, A.y + 150);
        circle(A.x + 75, A.y + 75, 60);
        if (semi) setfillpattern(pattern, outlineColor);
        else setfillstyle(1, outlineColor);
        floodfill(A.x + 75, A.y + 75, outlineColor);
        setfillstyle(1, backgroundColor);
    }
}

void paintBoard()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            paintCell(sc[i][j], game[0][i][j], 0);
        }
    }
}

void paintSettingsPieces()
{
    paintCell(sc[1][0], 'r', 0);
    paintCell(sc[2][0], 'r', 0);
    paintCell(sc[2][1], 'r', 0);
    paintCell(sc[2][2], 'r', 0);

    paintCell(sc[1][1], 'b', 0);
    paintCell(sc[1][2], 'b', 0);
    paintCell(sc[1][3], 'b', 0);
    paintCell(sc[2][3], 'b', 0);
}

void erasePiece(bool semi)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (game[0][i][j] == (turn ? 'b' : 'r'))
            {
                if (semi == 0) game[0][i][j] = 'e';
                paintCell(sc[i][j], semi ? (turn ? 'b' : 'r') : 'e', semi);
            }
        }
    }
}

void placePlayerPiece()
{
    for (int i = 0; i < 4; i++)
    {
        game[0][t[i].x][t[i].y] = turn ? 'b' : 'r';
        paintCell(sc[t[i].x][t[i].y], (turn ? 'b' : 'r'), 0);
    }
}

void placeAIPiece(MOVE A, char s[4][4], bool paint, bool del)
{
    s[A.x][A.y] = del ? 'e' : (turn ? 'b' : 'r');
    if (paint) paintCell(sc[A.x][A.y], del ? 'e' : (turn ? 'b' : 'r'), 0);
    for (int i = 0; i < 3; i++)
    {
        s[A.x + m[A.m][i].x][A.y + m[A.m][i].y] = del ? 'e' : (turn ? 'b' : 'r');
        if (paint) paintCell(sc[A.x + m[A.m][i].x][A.y + m[A.m][i].y], del ? 'e' : (turn ? 'b' : 'r'), 0);
    }
}

void replaceAINeutral(COORDINATES SRC, COORDINATES DST, char s[4][4], bool paint)
{
    s[SRC.x][SRC.y] = 'e';
    if (paint) paintCell(sc[SRC.x][SRC.y], 'e', 0);
    s[DST.x][DST.y] = 'n';
    if (paint) paintCell(sc[DST.x][DST.y], 'n', 0);
}

void initNormalStart()
{
    game[0][0][1] = game[0][0][2] = game[0][0][3] = game[0][3][0] = game[0][3][1] = game[0][3][2] = 'e';
    game[0][1][0] = game[0][2][0] = game[0][2][1] = game[0][2][2] = 'r';
    game[0][1][1] = game[0][1][2] = game[0][1][3] = game[0][2][3] = 'b';
    game[0][0][0] = game[0][3][3] = 'n';
}

void initEmptyStart()
{
    for (int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            game[0][i][j] = 'e';
        }
    }
}

void setGame(int x, char s[])
{
    if (x == 2)
    {
        cgp = cgp -> next -> next;
        if (st[6].v) turnLimitCopy = turnLimitCopy - 2;
    }
    else if (x == 1)
    {
        cgp = cgp -> next;
        turn = !turn;
        if (st[6].v) turnLimitCopy--;
    }
    else if (x == 0) turnEnded = !turnEnded;
    else if (x == -1)
    {
        cgp = cgp -> prev;
        turn = !turn;
        if (st[6].v) turnLimitCopy++;
    }
    else if (x == -2)
    {
        cgp = cgp -> prev -> prev;
        if (st[6].v) turnLimitCopy = turnLimitCopy + 2;
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            game[0][i][j] = cgp -> g[i][j];
        }
    }
    paintBoard();
    makeMessage(s);
    if (st[6].v) paintNumberOfTurns();
    swapbuffers();
    paintBoard();
    bar(0, 100, 1200, 200);
    outtextxy(600, 160, message);
    if (st[6].v) paintNumberOfTurns();
}

void setNextGame(bool c)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (game[c][i][j] != (turn ? 'b' : 'r'))
            {
                game[c + 1][i][j] = game[c][i][j];
            }
            else game[c + 1][i][j] = 'e';
        }
    }
}

#endif
