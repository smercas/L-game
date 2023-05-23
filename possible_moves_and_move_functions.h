#ifndef possible_moves_and_ai
#define possible_moves_and_ai

void initM()    ///initiates the m matrix with all the 16 ways a piece can be drawn from a (0, 0) point of reference
{
    FILE *moves = fopen("move_coords.txt", "r");
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            fscanf(moves, "%d %d", &m[i][j].x, &m[i][j].y);
        }
    }
    fclose(moves);
}

void initES()   ///initiates all the 232 (29 * 8) ending scenarios for the current player, in the order of: 1, 2, 3 and 4 moves away from a loss
{
    char x;
    FILE *endingScenarios = fopen("ending_scenarios.txt", "r");
    for (int i = 0; i < 29; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                for (int l = 0; l < 4; l++)
                {
                    fscanf(endingScenarios, "%c", &es[i * 8 + j][l][k]);
                    fscanf(endingScenarios, "%c", &x);
                }
            }
            fscanf(endingScenarios, "%c", &x);
        }
    }
    fclose(endingScenarios);
}

int getPM(bool v) ///calculates the number of possible moves
{
    int rez = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (game[v][i][j] == 'e' || game[v][i][j] == (turn ? 'b' : 'r'))
            {
                for (int k = 0; k < 8; k++) ///k from 8 to 15 are repeats of 0 to 7 kind of (0 to 7 might still be useful in some other cases so we'll keep it)
                {
                    if ((game[v][i + m[k][0].x][j + m[k][0].y] == 'e' ||
                         game[v][i + m[k][0].x][j + m[k][0].y] == (turn ? 'b' : 'r')) &&
                        (game[v][i + m[k][1].x][j + m[k][1].y] == 'e' ||
                         game[v][i + m[k][1].x][j + m[k][1].y] == (turn ? 'b' : 'r')) &&
                        (game[v][i + m[k][2].x][j + m[k][2].y] == 'e' ||
                         game[v][i + m[k][2].x][j + m[k][2].y] == (turn ? 'b' : 'r')) &&
                        hoveringOverBoard_c(i + m[k][0].x, j + m[k][0].y) &&
                        hoveringOverBoard_c(i + m[k][1].x, j + m[k][1].y) &&
                        hoveringOverBoard_c(i + m[k][2].x, j + m[k][2].y))
                     {
                         rez++;
                     }
                }
            }
        }
    }
    return rez - 1;
}

void setPM()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (game[0][i][j] == 'e' || game[0][i][j] == (turn ? 'b' : 'r'))
            {
                for (int k = 0; k < 8; k++)
                {
                    if ((game[0][i + m[k][0].x][j + m[k][0].y] == 'e' ||
                        game[0][i + m[k][0].x][j + m[k][0].y] == (turn ? 'b' : 'r')) &&
                        (game[0][i + m[k][1].x][j + m[k][1].y] == 'e' ||
                        game[0][i + m[k][1].x][j + m[k][1].y] == (turn ? 'b' : 'r')) &&
                        (game[0][i + m[k][2].x][j + m[k][2].y] == 'e' ||
                        game[0][i + m[k][2].x][j + m[k][2].y] == (turn ? 'b' : 'r')) &&
                        hoveringOverBoard_c(i + m[k][0].x, j + m[k][0].y) &&
                        hoveringOverBoard_c(i + m[k][1].x, j + m[k][1].y) &&
                        hoveringOverBoard_c(i + m[k][2].x, j + m[k][2].y) &&
                        (game[0][i][j] == 'e' || game[0][i + m[k][0].x][j + m[k][0].y] == 'e' ||
                        game[0][i + m[k][1].x][j + m[k][1].y] == 'e' || game[0][i + m[k][2].x][j + m[k][2].y] == 'e'))
                    {
                        pmPush(i, j, k);
                    }
                }
            }
        }
    }
}

bool pieceNotInTheSamePosition()    ///returns 1 if the board changes by at least one cell after the current player moves its piece
{
    return game[0][t[0].x][t[0].y] == 'e' || game[0][t[1].x][t[1].y] == 'e' ||
           game[0][t[2].x][t[2].y] == 'e' || game[0][t[3].x][t[3].y] == 'e';
}

bool stillValidMove()   ///returns 1 if the places in the trail can form a tetromino
                        ///although it does not take potential for completing a piece into account
                        ///so you can still get a (0 2) (1 2) (1 3) which has no potential to be completed
{
    if (tl == 1) return 1;
    if (tl == 2)
    {
        for (int i = 0; i < 4; i++)
        {
            if (t[1].x == t[0].x + ax[i] && t[1].y == t[0].y + ay[i]) return 1;
        }
    }
    if (tl == 3)
    {
        for (int i = 0; i < 4; i++)
        {
            if (t[2].x == t[0].x + ax[i] && t[2].y == t[0].y + ay[i] ||
                t[2].x == t[1].x + ax[i] && t[2].y == t[1].y+ ay[i]) return 1;
        }
    }
    if (tl == 4)
    {
        if (t[2].x == t[1].x && t[1].x == t[0].x)
        {
            if (t[0].y * 2 == t[1].y + t[2].y)
            {
                t[0].y = t[0].y + t[1].y;
                t[1].y = t[0].y - t[1].y;
                t[0].y = t[0].y - t[1].y;
            }
            if (t[3].x == t[1].x + 1 && t[3].y == t[1].y - 1 ||
                t[3].x == t[1].x + 1 && t[3].y == t[1].y + 1 ||
                t[3].x == t[1].x - 1 && t[3].y == t[1].y + 1 ||
                t[3].x == t[1].x - 1 && t[3].y == t[1].y - 1) return 1;
                return 0;
        }
        if (t[2].y == t[1].y && t[1].y == t[0].y)
        {
            if (t[0].x * 2 == t[1].x + t[2].x)
            {
                t[0].x = t[0].x + t[1].x;
                t[1].x = t[0].x - t[1].x;
                t[0].x = t[0].x - t[1].x;
            }
            if (t[3].x == t[1].x + 1 && t[3].y == t[1].y - 1 ||
                t[3].x == t[1].x + 1 && t[3].y == t[1].y + 1 ||
                t[3].x == t[1].x - 1 && t[3].y == t[1].y + 1 ||
                t[3].x == t[1].x - 1 && t[3].y == t[1].y - 1) return 1;
                return 0;
        }
        if (t[0].x == t[1].x && t[0].y == t[2].y || t[0].x == t[2].x && t[0].y == t[1].y)
        {
                t[0].x = t[0].x + t[1].x;
                t[1].x = t[0].x - t[1].x;
                t[0].x = t[0].x - t[1].x;
                t[0].y = t[0].y + t[1].y;
                t[1].y = t[0].y - t[1].y;
                t[0].y = t[0].y - t[1].y;
        }
        if (t[3].x == t[2].x && t[2].y * 2 == t[1].y + t[3].y ||
            t[3].x == t[0].x && t[0].y * 2 == t[1].y + t[3].y ||
            t[3].y == t[2].y && t[2].x * 2 == t[1].x + t[3].x ||
            t[3].y == t[0].y && t[0].x * 2 == t[1].x + t[3].x) return 1;
    }
    return 0;
}

int pieceMove()
{
    int x, y;
    setCellCoord(x, y);
    if (GetKeyState(VK_LBUTTON) & 128 && (game[0][x][y] == 'e' || game[0][x][y] == (turn ? 'b' : 'r')) &&
        (st[3].v || (!st[3].v && cgp == gp.last)))
    {
        erasePiece(1);
        paintCell(sc[x][y], turn ? 'b' : 'r', 0);
        swapbuffers();
        erasePiece(1);
        paintCell(sc[x][y], turn ? 'b' : 'r', 0);
        initT();
        tPush(x, y);
        while ((GetKeyState(VK_LBUTTON) & 128) && tl < 4 && hoveringOverBoard_s())
        {
            setCellCoord(x, y);
            if ((game[0][x][y] == 'e' || game[0][x][y] == (turn ? 'b' : 'r')))
            {
                if (notInT(x, y))
                {
                    tPush(x, y);
                    if (stillValidMove())
                    {
                        paintCell(sc[x][y], turn ? 'b' : 'r', 0);
                        swapbuffers();
                        paintCell(sc[x][y], turn ? 'b' : 'r', 0);
                    }
                    else
                    {
                        tPop();
                        break; ///delete this line in case u wanna test the movement conditions
                    }
                }
            }
            else break; /// this one too
        }
        if (tl == 4 && pieceNotInTheSamePosition())
        {
            erasePiece(0);
            placePlayerPiece();
            swapbuffers();
            paintBoard();
            turnEnded = 0;
            return 1;
        }
        else
        {
            setBothPages(paintBoard);
            while (GetKeyState(VK_LBUTTON) & 128);
            return 0;
        }
    }
    return -1;
}

int neutralMove(char s[])
{
    int x, y, ox, oy, prevx, prevy;
    setCellCoord(x, y);
    if (GetKeyState(VK_LBUTTON) & 128 && game[0][x][y] == 'n')
    {
        ox = x; oy = y; prevx = x; prevy = y;
        paintCell(sc[ox][oy], 'n', 1);
        game[0][ox][oy] = 'e';
        swapbuffers();
        paintCell(sc[ox][oy], 'e', 0);
        while (GetKeyState(VK_LBUTTON) & 128)
        {
            setCellCoord(x, y);
            if (game[0][x][y] == 'e' && (prevx != x || prevy != y) && hoveringOverBoard_c(x, y))
            {
                paintCell(sc[x][y], 'n', 1);
                swapbuffers();
                paintCell(sc[prevx][prevy], 'e', 0);
                prevx = x;
                prevy = y;
            }
        }
        if (game[0][x][y] == 'e' && (x != ox || y != oy))
        {
            game[0][x][y] = 'n';
            turnEnded = 1;
            turn = !turn;
            if (st[6].v) turnLimitCopy--;
            addGPToStack();
            makeMessage(s);
            paintCell(sc[x][y], 'n', 0);
            if (st[6].v) paintNumberOfTurns();
            swapbuffers();
            bar(0, 100, 1200, 200);
            outtextxy(600, 160, message);
            paintCell(sc[x][y], 'n', 0);
            if (st[6].v) paintNumberOfTurns();
            return 3;
        }
        else
        {
            game[0][ox][oy] = 'n';
            paintCell(sc[ox][oy], 'n', 0);
            swapbuffers();
            paintCell(sc[prevx][prevy], 'e', 0);
            paintCell(sc[ox][oy], 'n', 0);
            return 2;
        }
    }
    setactivepage(!getvisualpage());
    return -1;
}

int selectedInPlayArea(char s[])
{
    if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOverBoard_s() && getPM(0) && !(st[6].v && turnLimitCopy == 1))
    {
        return 0;
    }
    else
    {
        if (!(turnEnded && cgp == gp.first) && !(!strcmp(s, "PvE") && turnEnded && !cgp -> prev -> prev)) ///prev
        {
            if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(cb[0]))
            {
                paintButton(cb[0], 0);
                swapbuffers();
                while ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(cb[0]));
                swapbuffers();
                paintButton(cb[0], 1);
                if (hoveringOver(cb[0])) return 1;
                return -1;
            }
            if (GetKeyState(VK_LEFT) & 128)
            {
                paintButton(cb[0], 0);
                swapbuffers();
                while (GetKeyState(VK_LEFT) & 128);
                swapbuffers();
                paintButton(cb[0], 1);
                return 1;
            }
        }
        if (turnEnded && (cgp != gp.last || (strcmp(s, "EvE") == 0 && (st[3].v || (!st[3].v && cgp == gp.last)) && getPM(0) && !(st[6].v && turnLimitCopy == 1)))) ///next
        {
            if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(cb[1]))
            {
                paintButton(cb[1], 0);
                swapbuffers();
                while ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(cb[1]));
                swapbuffers();
                paintButton(cb[1], 1);
                if (hoveringOver(cb[1])) return 2;
                return -1;
            }
            if (GetKeyState(VK_RIGHT) & 128)
            {
                paintButton(cb[1], 0);
                swapbuffers();
                while (GetKeyState(VK_RIGHT) & 128);
                swapbuffers();
                paintButton(cb[1], 1);
                return 2;
            }
        }
        if (turnEnded && gp.l != 1) ///save
        {
            if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(cb[2]))
            {
                paintButton(cb[2], 0);
                swapbuffers();
                while ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(cb[2]));
                swapbuffers();
                paintButton(cb[2], 1);
                if (hoveringOver(cb[2]))
                {
                    initCTextBox();
                    paintTextBox(ltb[0], 0);
                    swapbuffers();
                    paintTextBox(ltb[0], 0);
                    saving = true;
                    return 3;
                }
                return -1;
            }
            if (GetKeyState('S') & 128)
            {
                paintButton(cb[2], 0);
                swapbuffers();
                while (GetKeyState('S') & 128);
                initCTextBox();
                paintTextBox(ltb[0], 0);
                swapbuffers();
                paintButton(cb[2], 1);
                paintTextBox(ltb[0], 0);
                saving = true;
                return 3;
            }
        }
        if (!turnEnded && strcmp(s, "EvE") && st[2].v) /// skip
        {
            if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(cb[3]))
            {
                paintButton(cb[3], 0);
                swapbuffers();
                while ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(cb[3]));
                swapbuffers();
                paintButton(cb[3], 1);
                if (hoveringOver(cb[3])) return 4;
                return -1;
            }
            if (GetKeyState(VK_RETURN) & 128)
            {
                paintButton(cb[3], 0);
                swapbuffers();
                while (GetKeyState(VK_RETURN) & 128);
                swapbuffers();
                paintButton(cb[3], 1);
                return 4;
            }
        }
        if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(ltb[0]) && saving)
        {
            char temp[33];
            strcpy(temp, ltb[0].text);
            setTextBox(ltb[0], 0, 0);
            writeSaveFile(llAdd(ltb[0].text, s, st[6].v, turnLimitCopy, turn), 1);
            writeSaves();
            setBothPages(addCommonButtons, s);
            saving = false;
            return 5;
        }
        if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[0]))
        {
            paintExitButton(top[0], 0);
            swapbuffers();
            while ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[0]));
            swapbuffers();
            paintExitButton(top[0], 1);
            if (hoveringOver(top[0])) return 6;
            return -1;
        }
        if (GetKeyState(VK_ESCAPE) & 128)
        {
            paintExitButton(top[0], 0);
            swapbuffers();
            while (GetKeyState(VK_ESCAPE) & 128);
            swapbuffers();
            paintExitButton(top[0], 1);
            return 6;
        }
        if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[1]))
        {
            paintMainMenuButton(top[1], 0);
            swapbuffers();
            while ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[1]));
            swapbuffers();
            paintMainMenuButton(top[1], 1);
            if (hoveringOver(top[1])) return 7;
            return -1;
        }
        if (GetKeyState('M') & 128)
        {
            paintMainMenuButton(top[1], 0);
            swapbuffers();
            while (GetKeyState('M') & 128);
            swapbuffers();
            paintMainMenuButton(top[1], 1);
            return 7;
        }
        if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[2]))
        {
            paintSettingsButton(top[2], 0);
            swapbuffers();
            while ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[2]));
            swapbuffers();
            paintSettingsButton(top[2], 1);
            if (hoveringOver(top[2])) return 8;
            return -1;
        }
        if (GetKeyState(VK_OEM_3) & 128)
        {
            paintSettingsButton(top[2], 0);
            swapbuffers();
            while (GetKeyState(VK_OEM_3) & 128);
            swapbuffers();
            paintSettingsButton(top[2], 1);
            return 8;
        }
        if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[3]))
        {
            paintBackButton(top[3], 0);
            swapbuffers();
            while ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[3]));
            swapbuffers();
            paintBackButton(top[3], 1);
            if (hoveringOver(top[3])) return 9;
            return -1;
        }
        if (GetKeyState('B') & 128)
        {
            paintBackButton(top[3], 0);
            swapbuffers();
            while (GetKeyState('B') & 128);
            swapbuffers();
            paintBackButton(top[3], 1);
            return 9;
        }
    }
    return -1;
}

int selectedInLoad()
{
    if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(lb[0]) && ll.l)
    {
        paintButton(lb[0], 0);
        swapbuffers();
        while ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(lb[0]));
        swapbuffers();
        paintButton(lb[0], 1);
        if (hoveringOver(lb[0]))
        {
            pathToFile(llpos -> name);
            remove(path);
            llDelete();
            writeSaves();
            setBothPages(addLoadButtons);
            return 0;
        }
        return -1;
    }
    if (GetKeyState('D') & 128 && ll.l)
    {
        paintButton(lb[0], 0);
        swapbuffers();
        while (GetKeyState('D') & 128);
        swapbuffers();
        paintButton(lb[0], 1);
        pathToFile(llpos -> name);
        remove(path);
        llDelete();
        writeSaves();
        setBothPages(addLoadButtons);
        return 0;
    }
    if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(lb[1]) && ll.l)
    {
        paintButton(lb[1], 0);
        swapbuffers();
        while ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(lb[1]));
        swapbuffers();
        paintButton(lb[1], 1);
        if (hoveringOver(lb[1]))
        {
            initLTextBox();
            paintTextBox(ltb[0], 1);
            swapbuffers();
            paintTextBox(ltb[0], 1);
            renamingSave = true;
            return 1;
        }
        return -1;
    }
    if (GetKeyState('R') & 128 && ll.l)
    {
        paintButton(lb[1], 0);
        swapbuffers();
        while (GetKeyState('R') & 128);
        initLTextBox();
        paintTextBox(ltb[0], 1);
        swapbuffers();
        paintButton(lb[1], 1);
        paintTextBox(ltb[0], 1);
        renamingSave = true;
        return 1;
    }
    if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(lb[2]) && ll.l)
    {
        paintButton(lb[2], 0);
        swapbuffers();
        while ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(lb[2]));
        swapbuffers();
        paintButton(lb[2], 1);
        if (hoveringOver(lb[2]))
        {
            readSaveFile(llpos, 1);
            return 2;
        }
        return -1;
    }
    if (GetKeyState(VK_RETURN) & 128 && ll.l)
    {
        paintButton(lb[2], 0);
        swapbuffers();
        while (GetKeyState(VK_RETURN) & 128);
        swapbuffers();
        paintButton(lb[2], 1);
        readSaveFile(llpos, 1);
        return 2;
    }
    if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(ltb[0]) && renamingSave)
    {
        char temp[33];
        strcpy(temp, ltb[0].text);
        setTextBox(ltb[0], 0, 1);

        SAVE *LLPOS = llpos, *LLFS = llfs;
        int LLRPOS = llrpos;
        char type[4];
        strcpy(type, llpos -> type);
        bool turnLimitActive = llpos -> turnLimitActive;
        turnLimitCopy = llpos -> turnLimitCopy;
        turn = llpos -> turn;
        llDelete();
        strcpy(ltb[0].text, llAdd(ltb[0].text, type, turnLimitActive, turnLimitCopy, turn) -> name);
        writeSaves();
        if (strcmp(temp, ltb[0].text) == 0)
        {
            llpos = LLPOS;
            llfs = LLFS;
            llrpos = LLRPOS;
        }
        else
        {
            pathToFile(temp);
            char a[50];
            strcpy(a, path);
            pathToFile(ltb[0].text);
            rename(a, path);
            llpos = ll.first;
            llfs = ll.first;
            llrpos = 0;
            while (strcmp(llpos -> name, ltb[0].text) != 0)
            {
                llpos = llpos -> next;
                if (llrpos < 6)
                {
                    llrpos++;
                }
                else
                {
                    llfs = llfs -> next;
                }
            }
        }
        setBothPages(addLoadButtons);
        renamingSave = false;
        return 3;
    }
    if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[0]))
    {
        paintExitButton(top[0], 0);
        swapbuffers();
        while ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[0]));
        swapbuffers();
        paintExitButton(top[0], 1);
        if (hoveringOver(top[0])) return 4;
        return -1;
    }
    if (GetKeyState(VK_ESCAPE) & 128)
    {
        paintExitButton(top[0], 0);
        swapbuffers();
        while (GetKeyState(VK_ESCAPE) & 128);
        swapbuffers();
        paintExitButton(top[0], 1);
        return 4;
    }
    if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[1]))
    {
        paintMainMenuButton(top[1], 0);
        swapbuffers();
        while ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[1]));
        swapbuffers();
        paintMainMenuButton(top[1], 1);
        if (hoveringOver(top[1])) return 5;
        return -1;
    }
    if (GetKeyState('M') & 128)
    {
        paintMainMenuButton(top[1], 0);
        swapbuffers();
        while (GetKeyState('M') & 128);
        swapbuffers();
        paintMainMenuButton(top[1], 1);
        return 5;
    }
    if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[2]))
    {
        paintSettingsButton(top[2], 0);
        swapbuffers();
        while ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[2]));
        swapbuffers();
        paintSettingsButton(top[2], 1);
        if (hoveringOver(top[2])) return 6;
        return -1;
    }
    if (GetKeyState(VK_OEM_3) & 128)
    {
        paintSettingsButton(top[2], 0);
        swapbuffers();
        while (GetKeyState(VK_OEM_3) & 128);
        swapbuffers();
        paintSettingsButton(top[2], 1);
        return 6;
    }
    if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[3]))
    {
        paintBackButton(top[3], 0);
        swapbuffers();
        while ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[3]));
        swapbuffers();
        paintBackButton(top[3], 1);
        if (hoveringOver(top[3])) return 7;
        return -1;
    }
    if (GetKeyState('B') & 128)
    {
        paintBackButton(top[3], 0);
        swapbuffers();
        while (GetKeyState('B') & 128);
        swapbuffers();
        paintBackButton(top[3], 1);
        return 7;
    }
    if ((GetKeyState(VK_UP) & 128) && llpos -> prev != NULL)
    {
        int timer = 500;
        llpos = llpos -> prev;
        if (llrpos > 0)
        {
            llrpos--;
        }
        else
        {
            llfs = llfs -> prev;
        }
        addLoadButtons();
        swapbuffers();
        while ((GetKeyState(VK_UP) & 128) && timer)
        {
            delay(10);
            timer = timer - 10;
        }
        while ((GetKeyState(VK_UP) & 128) && llpos -> prev != NULL)
        {
            llpos = llpos -> prev;
            if (llrpos > 0)
            {
                llrpos--;
            }
            else
            {
                llfs = llfs -> prev;
            }
            addLoadButtons();
            swapbuffers();
            delay(50);
        }
        addLoadButtons();
        return 8;
    }
    if ((GetKeyState(VK_DOWN) & 128) && llpos -> next != NULL)
    {
        int timer = 500;
        llpos = llpos -> next;
        if (llrpos < 6)
        {
            llrpos++;
        }
        else
        {
            llfs = llfs -> next;
        }
        addLoadButtons();
        swapbuffers();
        while ((GetKeyState(VK_DOWN) & 128) && timer)
        {
            delay(10);
            timer = timer - 10;
        }
        while ((GetKeyState(VK_DOWN) & 128) && llpos -> next != NULL)
        {
            llpos = llpos -> next;
            if (llrpos < 6)
            {
                llrpos++;
            }
            else
            {
                llfs = llfs -> next;
            }
            addLoadButtons();
            swapbuffers();
            delay(50);
        }
        addLoadButtons();
        return 9;
    }
    return -1;
}


int getNumberOfTurnsLeft(int x)
{
    if (x < 120) return 0;
    if (x < 144) return 1;
    if (x < 168) return 2;
    if (x < 208) return 3;
    if (x < 232) return 4;
    return -1;
}

bool isLogicalNextMove(int i, bool t, bool _case)
{
    int a = 0, b = 0, c = 0;
    for (int j = 0; j < 4; j++)
    {
        for (int k = 0; k < 4; k++)
        {
            if (es[i][j][k] == 'b' && game[_case + 1][j][k] != 'e') return 0;
            if (game[_case][j][k] == (t ? 'r' : 'b') && es[i][j][k] == 'r') a++;
            if (game[_case][j][k] == (t ? 'b' : 'r') && es[i][j][k] == 'b') b++;
            if (game[_case][j][k] == 'n' && es[i][j][k] == 'n') c++;
        }
    }
    if (a == 4 && b != 4 && c)
    {
        return 1;
    }
    return 0;
}

int AIMove(char s[])
{
    if (getPM(0) && !(st[6].v && turnLimitCopy == 1) && (st[3].v || (!st[3].v && cgp == gp.last)))
    {
        initPM();
        setPM();
        if (!d[turn].v)
        {
            erasePiece(0);
            int a = rand() % pml;
            placeAIPiece(pm[a], game[0], 1, 0);
            a = rand() % 3;
            int b = rand() % 6;
            if (a)
            {
                setEC(0);
                setNP();
                a--;
                replaceAINeutral(np[a], ec[b], game[0], 1);
            }
            turn = !turn;
            if (st[6].v) turnLimitCopy--;
            addGPToStack();
            makeMessage(s);
            if (st[6].v) paintNumberOfTurns();
            swapbuffers();
            paintBoard();
            bar(0, 100, 1200, 200);
            outtextxy(600, 160, message);
            if (st[6].v) paintNumberOfTurns();
            return a ? -3 : -2;
        }
        else
        {
            setNextGame(0);
            ///the pretties but better
            {
/*
            setNP();
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < pml; j++)
                {
                    placeAIPiece(pm[j], game[1], 0, 0);
                    for (int k = 0; k < 4; k++) cout << game[1][k][i] << ' ';
                    cout << "              ";
                    placeAIPiece(pm[j], game[1], 0, 1);
                }
                cout << endl;
            }
            cout << endl;
            for (int j = 0; j < pml; j++)
            {
                cout << "The EC vector:        ";
            }
            cout << endl;
            for (int j = 0; j < pml; j++)
            {
                placeAIPiece(pm[j], game[1], 0, 0);
                setEC(1);
                for (int i = 0; i < 6; i++)
                {
                    cout << ec[i].x << ' ';
                }
                cout << "          ";
                placeAIPiece(pm[j], game[1], 0, 1);
            }
            cout << endl;
            for (int j = 0; j < pml; j++)
            {
                placeAIPiece(pm[j], game[1], 0, 0);
                setEC(1);
                for (int i = 0; i < 6; i++)
                {
                    cout << ec[i].y << ' ';
                }
                cout << "          ";
                placeAIPiece(pm[j], game[1], 0, 1);
            }
            cout << endl;
            for (int i = 0; i <= 12; i++)
            {
                for (int j = 0; j < pml; j++)
                {
                    placeAIPiece(pm[j], game[1], 0, 0);
                    setEC(1);
                    cout << "NP: " << np[0].x << ' ' << np[0].y << ' ' << game[0][np[0].x][np[0].y] << ' ';
                    cout << np[1].x << ' ' << np[1].y << ' '<< game[0][np[1].x][np[1].y] << "       ";
                    placeAIPiece(pm[j], game[1], 0, 1);
                }
                cout << endl;
                for (int j = 0; j < pml; j++)
                {
                    placeAIPiece(pm[j], game[1], 0, 0);
                    setEC(1);
                    if (i)
                    {
                        cout << (i - 1) / 6 << ' ' << np[(i - 1) / 6].x << ' ' << np[(i - 1) / 6].y << ' ';
                        cout << game[1][np[(i - 1) / 6].x][np[(i - 1) / 6].y] << " -> ";
                        cout << (i - 1) % 6 << ' ' << ec[(i - 1) % 6].x << ' ' << ec[(i - 1) % 6].y << ' ';
                        cout << game[1][ec[(i - 1) % 6].x][ec[(i - 1) % 6].y] << "    ";
                    }
                    placeAIPiece(pm[j], game[1], 0, 1);
                }
                cout << endl;
                for (int j = 0; j < pml; j++)
                {
                    placeAIPiece(pm[j], game[1], 0, 0);
                    setEC(1);
                    if (i) replaceAINeutral(np[(i - 1) / 6], ec[(i - 1) % 6], game[1], 0);
                    endingScenario = -1;
                    bool ok;
                    for (int k = 0; k < 232 && endingScenario == -1; k++)
                    {
                        ok = true;
                        for (int l = 0; l < 4; l++)
                        {
                            for (int m = 0; m < 4; m++)
                            {
                                if (!turn)
                                {
                                    if (game[1][l][m] == 'b') game[1][l][m] = 'r';
                                    else if (game[1][l][m] == 'r') game[1][l][m] = 'b';
                                }
                                if (game[1][l][m] != es[k][l][m]) ok = false;
                                if (!turn)
                                {
                                    if (game[1][l][m] == 'b') game[1][l][m] = 'r';
                                    else if (game[1][l][m] == 'r') game[1][l][m] = 'b';
                                }
                            }
                        }
                        if (ok) endingScenario = k;
                    }
                    char s[40];
                    if (endingScenario == -1)
                    {
                        turn = !turn;
                        setNextGame(1);
                        for (int k = 0; k < 232 && endingScenario == -1; k++)
                        {
                            if (isLogicalNextMove(k, turn, 1))
                            {
                                endingScenario = k;
                            }
                        }
                        if (endingScenario == -1)
                        {
                            strcpy(s, "N m:");
                            convToChar(getPM(1));
                            strcat(s, number);
                            strcat(s, "            ");
                        }
                        else
                        {
                            strcpy(s, "L s:");
                            convToChar(endingScenario);
                            strcat(s, number);
                            strcat(s, " l:");
                            convToChar(endingScenario * 5 + 1);
                            strcat(s, number);
                            strcat(s, " m:");
                            convToChar(getNumberOfTurnsLeft(endingScenario));
                            strcat(s, number);
                        }
                        turn = !turn;
                    }
                    else
                    {
                        strcpy(s, "W s:");
                        convToChar(endingScenario);
                        strcat(s, number);
                        strcat(s, " l:");
                        convToChar(endingScenario * 5 + 1);
                        strcat(s, number);
                        strcat(s, " m:");
                        convToChar(getNumberOfTurnsLeft(endingScenario));
                        strcat(s, number);
                    }
                    cout << setw(18) << s << "    ";
                    if (i) replaceAINeutral(ec[(i - 1) % 6], np[(i - 1) / 6], game[1], 0);
                    placeAIPiece(pm[j], game[1], 0, 1);
                }
                cout << endl;
                for (int j = 0; j < 4; j++)
                {
                    for (int k = 0; k < pml; k++)
                    {
                        placeAIPiece(pm[k], game[1], 0, 0);
                        setEC(1);
                        if (i) replaceAINeutral(np[(i - 1) / 6], ec[(i - 1) % 6], game[1], 0);
                        endingScenario = -1;
                        bool ok;
                        for (int l = 0; l < 232 && endingScenario == -1; l++)
                        {
                            ok = true;
                            for (int m = 0; m < 4; m++)
                            {
                                for (int n = 0; n < 4; n++)
                                {
                                    if (!turn)
                                    {
                                        if (game[1][m][n] == 'b') game[1][m][n] = 'r';
                                        else if (game[1][m][n] == 'r') game[1][m][n] = 'b';
                                    }
                                    if (game[1][m][n] != es[l][m][n]) ok = false;
                                    if (!turn)
                                    {
                                        if (game[1][m][n] == 'b') game[1][m][n] = 'r';
                                        else if (game[1][m][n] == 'r') game[1][m][n] = 'b';
                                    }
                                }
                            }
                            if (ok) endingScenario = l;
                        }
                        if (endingScenario == -1)
                        {
                            turn = !turn;
                            setNextGame(1);
                            for (int l = 0; l < 232 && endingScenario == -1; l++)
                            {
                                if (isLogicalNextMove(l, turn, 1))
                                {
                                    endingScenario = l;
                                }
                            }
                            if (endingScenario == -1)
                            {
                                for (int l = 0; l < 4; l++)
                                {
                                    cout << game[1][l][j] << ' ';
                                }
                                cout << "   ";
                                for (int l = 0; l < 4; l++)
                                {
                                    cout << "  ";
                                }
                                cout << "   ";
                            }
                            else
                            {
                                for (int l = 0; l < 4; l++)
                                {
                                    cout << game[1][l][j] << ' ';
                                }
                                cout << "   ";
                                for (int l = 0; l < 4; l++)
                                {
                                    cout << es[endingScenario][l][j] << ' ';
                                }
                                cout << "   ";
                            }
                            turn = !turn;
                        }
                        else
                        {
                            for (int l = 0; l < 4; l++)
                            {
                                cout << game[1][l][j] << ' ';
                            }
                            cout << "   ";
                            for (int l = 0; l < 4; l++)
                            {
                                cout << es[endingScenario][l][j] << ' ';
                            }
                            cout << "    ";
                        }
                        if (i) replaceAINeutral(ec[(i - 1) % 6], np[(i - 1) / 6], game[1], 0);
                        placeAIPiece(pm[k], game[1], 0, 1);
                    }
                    cout << endl;
                }
                cout << endl;
            }
/**/
            }
            endingScenario = -1;
            initMP(lmp, lmpl);
            initMP(wnmp, wnmpl);
            for (int i = 0; i < 232 && (endingScenario == getNumberOfTurnsLeft(i) || endingScenario == -1); i++)
            {
                if (isLogicalNextMove(i, turn, 0) && (endingScenario == getNumberOfTurnsLeft(i) || endingScenario == -1))
                {
                    endingScenario = getNumberOfTurnsLeft(i);
                    mpAdd(wnmp, wnmpl, es[i], turn);
                }
            }   ///gets the least amount of moves needed to achieve victory in endingScenario and all the compatible ending scenarios idk how to explain
            if (endingScenario == -1)
            {
                setNP();
                int neutral = -1, losing = -1;
                for (int i = 0; i < pml; i++)
                {
                    placeAIPiece(pm[i], game[1], 0, 0);
                    setEC(1);
                    turn = !turn;
                    for (int j = 0; j <= 12; j++)
                    {
                        if (j) replaceAINeutral(np[(j - 1) / 6], ec[(j - 1) % 6], game[1], 0);
                        setNextGame(1);
                        endingScenario = -1;
                        for (int k = 0; k < 232 && endingScenario == -1; k++)
                        {
                            if (isLogicalNextMove(k, turn, 1))
                            {
                                endingScenario = getNumberOfTurnsLeft(k);
                            }
                        }   ///ending Scenario should get the max of moves left against a perfect opponent
                            ///between 2 and 4 the maximum again said perfect opponent is 2; that's the reason k goes from 0 to 232
                        if (endingScenario == -1)
                        {
                            if ((neutral > getPM(1) && !st[5].v) || neutral == -1)
                            {
                                neutral = getPM(1);
                                initMP(wnmp, wnmpl);
                                mpAdd(wnmp, wnmpl, game[1], 1);
                            }
                            else if (neutral == getPM(1) || st[5].v)
                            {
                                mpAdd(wnmp, wnmpl, game[1], 1);
                            }
                        }
                        else
                        {
                            if (endingScenario > losing || losing == -1)
                            {
                                losing = endingScenario;
                                initMP(lmp, lmpl);
                                mpAdd(lmp, lmpl, game[1], 1);
                            }
                            else if (endingScenario == losing)
                            {
                                mpAdd(lmp, lmpl, game[1], 1);
                            }
                        }
                        if (j) replaceAINeutral(ec[(j - 1) % 6], np[(j - 1) / 6], game[1], 0);
                    }
                    turn = !turn;
                    placeAIPiece(pm[i], game[1], 0, 1);
                }
                ///pretties yet again
                {
/*
                if (neutral != -1)
                {
                    cout << "Neutral Moves for " << (turn ? "blue" : "red") << " (" << (st[5].v ? wnmpl : neutral) << "):" << endl;
                    for (int j = 0; j < 4; j++)
                    {
                        for (int k = 0; k < wnmpl; k++)
                        {
                            for (int l = 0; l < 4; l++)
                            {
                                cout << wnmp[k][l][j] << ' ';
                            }
                            cout << "   ";
                        }
                        cout << endl;
                    }
                    cout << endl;
                }
                if (losing != -1)
                {
                    cout << "Losing Moves for " << (turn ? "blue" : "red") << " (" << losing << "):" << endl;
                    for (int j = 0; j < 4; j++)
                    {
                        for (int k = 0; k < lmpl; k++)
                        {
                            for (int l = 0; l < 4; l++)
                            {
                                cout << lmp[k][l][j] << ' ';
                            }
                            cout << "   ";
                        }
                        cout << endl;
                    }
                    cout << endl;
                }
/**/
                }
                if (neutral != -1) return mpPick(wnmp, wnmpl, s) ? -3 : -2;
                return mpPick(lmp, lmpl, s) ? -3 : -2;
            }
            else
            {
                ///guess what? more pretties
                {
/*
                cout << turn << " has the following winning moves (" << endingScenario << "):" << endl;
                for (int j = 0; j < 4; j++)
                {
                    for (int k = 0; k < wnmpl; k++)
                    {
                        for (int l = 0; l < 4; l++)
                        {
                            cout << wnmp[k][l][j] << ' ';
                        }
                        cout << "    ";
                    }
                    cout << endl;
                }
                cout << endl;
/**/
                }
                return mpPick(wnmp, wnmpl, s) ? -3 : -2;
            }
        }
    }
    return selectedInPlayArea(s);
}


void placeRandomPiece()
{
    initPM();
    setPM();
    placeAIPiece(pm[rand() % pml], game[0], 0, 0);
    turn = !turn;
}

void initRandomStart(char s[])
{
    bool ok = true;
    while (ok)
    {
        ok = false;
        initEmptyStart();
        placeRandomPiece();
        placeRandomPiece();
        setEC(0);
        int a = rand() % 8;
        paintCell(sc[ec[a].x][ec[a].y], 'n', 0);
        game[0][ec[a].x][ec[a].y] = 'n';
        setEC(0);
        a = rand() % 7;
        paintCell(sc[ec[a].x][ec[a].y], 'n', 0);
        game[0][ec[a].x][ec[a].y] = 'n';

        initPM();
        setPM();
        setNextGame(0);
        ///the pretties are back yaaay
        {
/*
        setNP();
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < pml; j++)
            {
                placeAIPiece(pm[j], game[1], 0, 0);
                for (int k = 0; k < 4; k++) cout << game[1][k][i] << ' ';
                cout << "              ";
                placeAIPiece(pm[j], game[1], 0, 1);
            }
            cout << endl;
        }
        cout << endl;
        for (int j = 0; j < pml; j++)
        {
            cout << "The EC vector:        ";
        }
        cout << endl;
        for (int j = 0; j < pml; j++)
        {
            placeAIPiece(pm[j], game[1], 0, 0);
            setEC(1);
            for (int i = 0; i < 6; i++)
            {
                cout << ec[i].x << ' ';
            }
            cout << "          ";
            placeAIPiece(pm[j], game[1], 0, 1);
        }
        cout << endl;
        for (int j = 0; j < pml; j++)
        {
            placeAIPiece(pm[j], game[1], 0, 0);
            setEC(1);
            for (int i = 0; i < 6; i++)
            {
                cout << ec[i].y << ' ';
            }
            cout << "          ";
            placeAIPiece(pm[j], game[1], 0, 1);
        }
        cout << endl;
        for (int i = 0; i <= 12; i++)
        {
            for (int j = 0; j < pml; j++)
            {
                placeAIPiece(pm[j], game[1], 0, 0);
                setEC(1);
                cout << "NP: " << np[0].x << ' ' << np[0].y << ' ' << game[0][np[0].x][np[0].y] << ' ';
                cout << np[1].x << ' ' << np[1].y << ' '<< game[0][np[1].x][np[1].y] << "       ";
                placeAIPiece(pm[j], game[1], 0, 1);
            }
            cout << endl;
            for (int j = 0; j < pml; j++)
            {
                placeAIPiece(pm[j], game[1], 0, 0);
                setEC(1);
                if (i)
                {
                    cout << (i - 1) / 6 << ' ' << np[(i - 1) / 6].x << ' ' << np[(i - 1) / 6].y << ' ';
                    cout << game[1][np[(i - 1) / 6].x][np[(i - 1) / 6].y] << " -> ";
                    cout << (i - 1) % 6 << ' ' << ec[(i - 1) % 6].x << ' ' << ec[(i - 1) % 6].y << ' ';
                    cout << game[1][ec[(i - 1) % 6].x][ec[(i - 1) % 6].y] << "    ";
                }
                placeAIPiece(pm[j], game[1], 0, 1);
            }
            cout << endl;
            for (int j = 0; j < pml; j++)
            {
                placeAIPiece(pm[j], game[1], 0, 0);
                setEC(1);
                if (i) replaceAINeutral(np[(i - 1) / 6], ec[(i - 1) % 6], game[1], 0);
                endingScenario = -1;
                bool ok;
                for (int k = 0; k < 232 && endingScenario == -1; k++)
                {
                    ok = true;
                    for (int l = 0; l < 4; l++)
                    {
                        for (int m = 0; m < 4; m++)
                        {
                            if (!turn)
                            {
                                if (game[1][l][m] == 'b') game[1][l][m] = 'r';
                                else if (game[1][l][m] == 'r') game[1][l][m] = 'b';
                            }
                            if (game[1][l][m] != es[k][l][m]) ok = false;
                            if (!turn)
                            {
                                if (game[1][l][m] == 'b') game[1][l][m] = 'r';
                                else if (game[1][l][m] == 'r') game[1][l][m] = 'b';
                            }
                        }
                    }
                    if (ok) endingScenario = k;
                }
                char s[40];
                if (endingScenario == -1)
                {
                    turn = !turn;
                    setNextGame(1);
                    for (int k = 0; k < 232 && endingScenario == -1; k++)
                    {
                        if (isLogicalNextMove(k, turn, 1))
                        {
                            endingScenario = k;
                        }
                    }
                    if (endingScenario == -1)
                    {
                        strcpy(s, "N m:");
                        convToChar(getPM(1));
                        strcat(s, number);
                        strcat(s, "            ");
                    }
                    else
                    {
                        strcpy(s, "L s:");
                        convToChar(endingScenario);
                        strcat(s, number);
                        strcat(s, " l:");
                        convToChar(endingScenario * 5 + 1);
                        strcat(s, number);
                        strcat(s, " m:");
                        convToChar(getNumberOfTurnsLeft(endingScenario));
                        strcat(s, number);
                    }
                    turn = !turn;
                }
                else
                {
                    strcpy(s, "W s:");
                    convToChar(endingScenario);
                    strcat(s, number);
                    strcat(s, " l:");
                    convToChar(endingScenario * 5 + 1);
                    strcat(s, number);
                    strcat(s, " m:");
                    convToChar(getNumberOfTurnsLeft(endingScenario));
                    strcat(s, number);
                }
                cout << setw(18) << s << "    ";
                if (i) replaceAINeutral(ec[(i - 1) % 6], np[(i - 1) / 6], game[1], 0);
                placeAIPiece(pm[j], game[1], 0, 1);
            }
            cout << endl;
            for (int j = 0; j < 4; j++)
            {
                for (int k = 0; k < pml; k++)
                {
                    placeAIPiece(pm[k], game[1], 0, 0);
                    setEC(1);
                    if (i) replaceAINeutral(np[(i - 1) / 6], ec[(i - 1) % 6], game[1], 0);
                    endingScenario = -1;
                    bool ok;
                    for (int l = 0; l < 232 && endingScenario == -1; l++)
                    {
                        ok = true;
                        for (int m = 0; m < 4; m++)
                        {
                            for (int n = 0; n < 4; n++)
                            {
                                if (!turn)
                                {
                                    if (game[1][m][n] == 'b') game[1][m][n] = 'r';
                                    else if (game[1][m][n] == 'r') game[1][m][n] = 'b';
                                }
                                if (game[1][m][n] != es[l][m][n]) ok = false;
                                if (!turn)
                                {
                                    if (game[1][m][n] == 'b') game[1][m][n] = 'r';
                                    else if (game[1][m][n] == 'r') game[1][m][n] = 'b';
                                }
                            }
                        }
                        if (ok) endingScenario = l;
                    }
                    if (endingScenario == -1)
                    {
                        turn = !turn;
                        setNextGame(1);
                        for (int l = 0; l < 232 && endingScenario == -1; l++)
                        {
                            if (isLogicalNextMove(l, turn, 1))
                            {
                                endingScenario = l;
                            }
                        }
                        if (endingScenario == -1)
                        {
                            for (int l = 0; l < 4; l++)
                            {
                                cout << game[1][l][j] << ' ';
                            }
                            cout << "   ";
                            for (int l = 0; l < 4; l++)
                            {
                                cout << "  ";
                            }
                            cout << "   ";
                        }
                        else
                        {
                            for (int l = 0; l < 4; l++)
                            {
                                cout << game[1][l][j] << ' ';
                            }
                            cout << "   ";
                            for (int l = 0; l < 4; l++)
                            {
                                cout << es[endingScenario][l][j] << ' ';
                            }
                            cout << "   ";
                        }
                        turn = !turn;
                    }
                    else
                    {
                        for (int l = 0; l < 4; l++)
                        {
                            cout << game[1][l][j] << ' ';
                        }
                        cout << "   ";
                        for (int l = 0; l < 4; l++)
                        {
                            cout << es[endingScenario][l][j] << ' ';
                        }
                        cout << "    ";
                    }
                    if (i) replaceAINeutral(ec[(i - 1) % 6], np[(i - 1) / 6], game[1], 0);
                    placeAIPiece(pm[k], game[1], 0, 1);
                }
                cout << endl;
            }
            cout << endl;
        }
/**/
        }
        initMP(wnmp, wnmpl);
        for (int i = 0; i < 232 && !ok; i++)
        {
            if (isLogicalNextMove(i, turn, 0))
            {
                ok = !ok;
            }
        }
        if (!ok)
        {
            setNP();
            for (int i = 0; i < pml; i++)
            {
                placeAIPiece(pm[i], game[1], 0, 0);
                setEC(1);
                turn = !turn;
                for (int j = 0; j <= 12; j++)
                {
                    if (j) replaceAINeutral(np[(j - 1) / 6], ec[(j - 1) % 6], game[1], 0);
                    setNextGame(1);
                    ok = false;
                    for (int k = 0; k < 232 && !ok; k++)
                    {
                        if (isLogicalNextMove(k, turn, 1))
                        {
                            ok = !ok;
                        }
                    }
                    if (!ok)
                    {
                        mpAdd(wnmp, wnmpl, game[1], 1);
                    }
                    if (j) replaceAINeutral(ec[(j - 1) % 6], np[(j - 1) / 6], game[1], 0);
                }
                turn = !turn;
                placeAIPiece(pm[i], game[1], 0, 1);
            }
            ///need I introduce them again?
            {
/*
            if (wnmpl)
            {
                cout << "Neutral Moves for " << (turn ? "blue" : "red") << " (" << wnmpl << "):" << endl;
                for (int j = 0; j < 4; j++)
                {
                    for (int k = 0; k < wnmpl; k++)
                    {
                        for (int l = 0; l < 4; l++)
                        {
                            cout << wnmp[k][l][j] << ' ';
                        }
                        cout << "   ";
                    }
                    cout << endl;
                }
                cout << endl;
            }
            else cout << endl << "Will lose again a perfect opponent" << endl << endl << endl;
/**/
            }
            if (wnmpl) ok = false;
        }
        else
        {
            ///cout << endl << endl << "Will win somehow" << endl << endl << endl;
            ok = true;
        }
    }
}

#endif
