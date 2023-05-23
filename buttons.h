#ifndef buttons
#define buttons

void convToChar(unsigned long long n)
{
    if (n == 0) strcpy(number, "0");
    else
    {
        int i = 0;
        while (n)
        {
            number[i] = n % 10 + 48;
            n = n / 10;
            i++;
        }
        number[i] = NULL;
        strrev(number);
    }
}

unsigned long long convToInt(char s[])
{
    unsigned long long nr = 0;
    for (int i = 0; i < strlen(s); i++)
    {
        nr = nr * 10 + (s[i] - 48);
    }
    return nr;
}

void readBit(unsigned char c, bool &v, int pos)
{
/*
    cout << "the byte:\t\t\t";
    for (int i = 7; i >= 0; i--)
    {
        cout << ((c >> i) & 1);
    }
    cout << endl << "the bit that'll be read:\t";
    for (int i = 7; i >= 0; i--)
    {
        if (i == 7 - pos) cout << (((v << (7 - pos)) >> i) & 1);
        else cout << ' ';
    }
/**/
    v = ((c >> (7 - pos)) & 1);
/*
    cout << endl << "the bit after it has been read:\t";
    for (int i = 7; i >= 0; i--)
    {
        if (i == 7 - pos) cout << (((v << (7 - pos)) >> i) & 1);
        else cout << ' ';
    }
    cout << endl << endl;
/**/
}

void writeBit(unsigned char &c, bool v, int pos)
{
/*
    cout << "byte before any modification:\t";
    for (int i = 7; i >= 0; i--)
    {
        cout << ((c >> i) & 1);
    }
    cout << endl << "byte without the " << pos + 1;
    if (pos == 0) cout << "st";
    else if (pos == 1) cout << "nd";
    else if (pos == 2) cout << "rd";
    else cout << "th";
    cout << " bit:\t";
    for (int i = 7; i >= 0; i--)
    {
        if (i == 7 - pos) cout << ' ';
        else cout << (((c & (~(1 << (7 - pos)))) >> i) & 1);
    }
    cout << endl << "the bit that'll be added:\t";
    for (int i = 7; i >= 0; i--)
    {
        if (i == 7 - pos) cout << (((v << (7 - pos)) >> i) & 1);
        else cout << ' ';
    }
    cout << endl << "the final byte product:\t\t";
/**/
    c = (c & (~(1 << (7 - pos)))) + (v << (7 - pos));
/*
    for (int i = 7; i >= 0; i--)
    {
        cout << ((c >> i) & 1);
    }
    cout << endl;
    cout << endl;
/**/
}

void readSettings()
{
    unsigned char c;
    FILE *settings = fopen("settings", "r");

    fseek(settings, 44, SEEK_SET);
    fscanf(settings, "%c", &c);
    for (int i = 2; i < 8; i++)
    {
        readBit(c, st[i - 2].v, i);
    }

    fseek(settings, 45, SEEK_SET);
    fscanf(settings, "%c", &c);
    for (int i = 0; i < 1; i++)
    {
        readBit(c, st[i + 6].v, i);
    }

    fclose(settings);
}

void writeSettings()
{
    unsigned char c;
    FILE *settings = fopen("settings", "r+");

    fseek(settings, 44, SEEK_SET);
    fscanf(settings, "%c", &c);
    fseek(settings, 44, SEEK_SET);
    for (int i = 2; i < 8; i++)
    {
        writeBit(c, st[i - 2].v, i);
    }
    fprintf(settings, "%c", c);

    fseek(settings, 45, SEEK_SET);
    fscanf(settings, "%c", &c);
    fseek(settings, 45, SEEK_SET);
    for (int i = 0; i < 1; i++)
    {
        writeBit(c, st[i + 6].v, i);
    }
    fprintf(settings, "%c", c);

    fclose(settings);
}

void pathToFile(char name[])
{
    strcpy(path, "saved games/");
    strcat(path, name);
    strcat(path, ".txt");
}

void readSaveFile(SAVE *&S, bool loading)
{///a save file: 8 - counter, 4 x counter - gp stack, 4 - type, 8 - turn limit copy, 1/8 turn and 1/8 turn limit active
    unsigned long long counter;
    bool b1, b2;
    unsigned char byte;
    pathToFile(S -> name);
    FILE *saveFile = fopen(path, "rb");
    fread(&counter, 8, 1, saveFile);
    if (loading)
    {
        for (int j = 0; j < 4; j++)
        {
            fread(&byte, 1, 1, saveFile);
            for (int k = 0; k < 4; k++)
            {
                readBit(byte, b1, k * 2);
                readBit(byte, b2, k * 2 + 1);
                if (!b1 && !b2) game[0][j][k] = 'e';
                else if (b1 && !b2) game[0][j][k] = 'r';
                else if (!b1 && b2) game[0][j][k] = 'b';
                else game[0][j][k] = 'n';
            }
        }
        initGP();
        for (unsigned long long i = 1; i < counter; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                fread(&byte, 1, 1, saveFile);
                for (int k = 0; k < 4; k++)
                {
                    readBit(byte, b1, k * 2);
                    readBit(byte, b2, k * 2 + 1);
                    if (!b1 && !b2) game[0][j][k] = 'e';
                    else if (b1 && !b2) game[0][j][k] = 'r';
                    else if (!b1 && b2) game[0][j][k] = 'b';
                    else game[0][j][k] = 'n';
                }
            }
            addGPToStack();
        }
    }
    fseek(saveFile, counter * 4 + 8, SEEK_SET);
    fread(&S -> type, 1, 4, saveFile);
    fread(&S -> turnLimitCopy, 8, 1, saveFile);
    fread(&byte, 1, 1, saveFile);
    readBit(byte, S -> turn, 0);
    readBit(byte, S -> turnLimitActive, 1);
    fclose(saveFile);
}

void writeSaveFile(SAVE *S, bool saving)
{
    pathToFile(S -> name);
    unsigned long long counter;
    bool b1, b2;
    unsigned char byte;
    FILE *saveFile = fopen(path, saving ? "wb" : "rb+");
    if (saving)
    {
        counter = 0;
        fseek(saveFile, 8, SEEK_SET);
        for (BOARD *B = gp.first; B != NULL; B = B -> next)
        {
            for (int i = 0; i < 4; i++)
            {
                byte = 0;
                for (int j = 0; j < 4; j++)
                {
                    if (B -> g[i][j] == 'r' || B -> g[i][j] == 'n')
                    {
                        writeBit(byte, 1, j * 2);
                    }
                    if (B -> g[i][j] == 'b' || B -> g[i][j] == 'n')
                    {
                        writeBit(byte, 1, j * 2 + 1);
                    }
                }
                fwrite(&byte, 1, 1, saveFile);
            }
            counter++;
        }
        fseek(saveFile, 0, SEEK_SET);
        fwrite(&counter, 8, 1, saveFile);
    }
    else
    {
        fread(&counter, 8, 1, saveFile);
    }
    fseek(saveFile, counter * 4 + 8, SEEK_SET);
    fwrite(&S -> type, 1, 4, saveFile);
    fwrite(&S -> turnLimitCopy, 8, 1, saveFile);
    byte = 0;
    writeBit(byte, S -> turn, 0);
    writeBit(byte, S -> turnLimitActive, 1);
    fwrite(&byte, 1, 1, saveFile);
    fclose(saveFile);
}

void readSaves()
{
    unsigned long long counter;
    SAVE *S;
    char name[33];
    FILE *saves = fopen("saves", "rb");
    fread(&counter, 8, 1, saves);
    fclose(saves);
    saves = fopen("saves", "r");
    fseek(saves, 8, SEEK_SET);
    for (unsigned long long i = 0; i < counter; i++)
    {
        fgets(name, 34, saves);
        name[strlen(name) - 1] = NULL;
        S = llAdd(name, "", 0, 0, 0);
        readSaveFile(S, 1);
    }
    fclose(saves);
}

void writeSaves()
{
    unsigned long long counter = 0;
    FILE *saves = fopen("saves", "w");
    fseek(saves, 8, SEEK_SET);
    for (SAVE *S = ll.first; S != NULL; S = S -> next)
    {
        fprintf(saves, "%s\n", S -> name);
        counter++;
        cout << "name: <" << S -> name << ">" << endl;
    }
    fclose(saves);
    saves = fopen("saves", "rb+");
    fwrite(&counter, 8, 1, saves);
    fclose(saves);

}

///Buttons

void initButton(BUTTON &B, int x1, int y1, int x2, int y2, char ENG[], char ROM[])
{
    B.x1 = x1;
    B.y1 = y1;
    B.x2 = x2;
    B.y2 = y2;
    strcpy(B.eng, ENG);
    strcpy(B.rom, ROM);
}

bool hoveringOver(BUTTON B)
{
    return mousex() > B.x1 && mousex() < B.x2 && mousey() > B.y1 && mousey() < B.y2;
}

void paintButton(BUTTON B, bool c)
{
    setfillstyle(1, c ? backgroundColor : outlineColor);
    setcolor(c ? outlineColor : backgroundColor);
    setbkcolor(c ? backgroundColor : outlineColor);
    bar(B.x1 + 1, B.y1 + 1, B.x2, B.y2);
    rectangle(B.x1, B.y1, B.x2, B.y2);
    outtextxy((B.x1 + B.x2) / 2, (B.y1 + B.y2) / 2 + 10, st[0].v ? B.eng : B.rom);
    setfillstyle(1, backgroundColor);
    setcolor(outlineColor);
    setbkcolor(backgroundColor);
}

void paintButtons(BUTTON B[], int nr)
{
    for (int i = 0; i < nr; i++)
    {
        paintButton(B[i], 1);
    }
}

void initTopButtons()
{
    for (unsigned int i = 0; i < 4; i++)
    {
        initButton(top[i], 820 + i * 100, 20, 880 + i * 100, 80, "", "");
    }
}

void paintExitButton(BUTTON B, bool c)
{
    int x = (B.x1 + B.x2) / 2, y = (B.y1 + B.y2) / 2, p[32] = {x - 15, y - 20, x + 10, y - 20, x + 10, y - 6, x + 7, y - 6, x + 7, y - 17,
                                                                x - 9, y - 17, x + 2, y - 12, x + 2, y + 13, x + 7, y + 13, x + 7, y + 2,
                                                                x + 10, y + 2, x + 10, y + 16, x + 2, y + 16, x + 2, y + 23, x - 15, y + 15,
                                                                x - 15, y - 20};
    setfillstyle(1, c ? backgroundColor : outlineColor);
    setcolor(c ? outlineColor : backgroundColor);
    bar(B.x1, B.y1, B.x2, B.y2);
    setfillstyle(1, c ? outlineColor : backgroundColor);
    rectangle(B.x1, B.y1, B.x2, B.y2);
    drawpoly(16, p);
    floodfill(x, y, c ? outlineColor : backgroundColor);
    int q[16] = {x + 4, y - 4, x + 13, y - 4, x + 13, y - 6, x + 21, y - 2, x + 13, y + 2, x + 13, y, x + 4, y, x + 4, y - 4};
    drawpoly(8, q);
    floodfill(x + 14, y - 2, c ? outlineColor : backgroundColor);
}

void paintMainMenuButton(BUTTON B, bool c)
{
    int x = (B.x1 + B.x2) / 2, y = (B.y1 + B.y2) / 2, p[28] = {x - 20, y, x, y - 20, x + 20, y, x + 20, y + 1, x + 15, y + 1,
                                                                x + 15, y + 20, x + 10, y + 20, x + 10, y + 5, x + 2, y + 5, x + 2, y + 20,
                                                                x - 15, y + 20, x - 15, y + 1, x - 20, y + 1, x - 20, y};
    setfillstyle(1, c ? backgroundColor : outlineColor);
    setcolor(c ? outlineColor : backgroundColor);
    bar(B.x1, B.y1, B.x2, B.y2);
    setfillstyle(1, c ? outlineColor : backgroundColor);
    rectangle(B.x1, B.y1, B.x2, B.y2);
    drawpoly(14, p);
    rectangle(x - 10, y + 5, x - 3, y + 12);
    floodfill(x, y, c ? outlineColor : backgroundColor);
    setfillstyle(1, backgroundColor);
    setcolor(outlineColor);
}

void paintSettingsButton(BUTTON B, bool c)
{
    int x = (B.x1 + B.x2) / 2, y = (B.y1 + B.y2) / 2;
    setfillstyle(1, c ? backgroundColor : outlineColor);
    setcolor(c ? outlineColor : backgroundColor);
    bar(B.x1, B.y1, B.x2, B.y2);
    setfillstyle(1, c ? outlineColor : backgroundColor);
    rectangle(B.x1, B.y1, B.x2, B.y2);
    circle(x, y, 8);
    circle(x, y, 17);
    floodfill(x + 12, y, c ? outlineColor : backgroundColor);
    rectangle(x - 4, y + 16, x + 4, y + 24);
    floodfill(x, y + 20, c ? outlineColor : backgroundColor);
    rectangle(x - 4, y - 24, x + 4, y - 16);
    floodfill(x, y - 20, c ? outlineColor : backgroundColor);
    rectangle(x + 16, y - 4, x + 24, y + 4);
    floodfill(x + 20, y, c ? outlineColor : backgroundColor);
    rectangle(x - 24, y - 4, x - 16, y + 4);
    floodfill(x - 20, y, c ? outlineColor : backgroundColor);
    line(x + 8, y + 14, x + 14, y + 20);
    line(x + 14, y + 8, x + 20, y + 14);
    line(x + 14, y + 20, x + 20, y + 14);
    floodfill(x + 14, y + 14, c ? outlineColor : backgroundColor);
    line(x + 8, y - 14, x + 14, y - 20);
    line(x + 14, y - 8, x + 20, y - 14);
    line(x + 14, y - 20, x + 20, y - 14);
    floodfill(x + 14, y - 14, c ? outlineColor : backgroundColor);
    line(x - 8, y - 14, x - 14, y - 20);
    line(x - 14, y - 8, x - 20, y - 14);
    line(x - 14, y - 20, x - 20, y - 14);
    floodfill(x - 14, y - 14, c ? outlineColor : backgroundColor);
    line(x - 8, y + 14, x - 14, y + 20);
    line(x - 14, y + 8, x - 20, y + 14);
    line(x - 14, y + 20, x - 20, y + 14);
    floodfill(x - 14, y + 14, c ? outlineColor : backgroundColor);
    setfillstyle(1, backgroundColor);
    setcolor(outlineColor);
}

void paintBackButton(BUTTON B, bool c)
{
    int x = (B.x1 + B.x2) / 2, y = (B.y1 + B.y2) / 2, p[8] = {x - 7, y - 14, x - 7, y + 14, x - 21, y, x - 7, y - 14};
    setfillstyle(1, c ? backgroundColor : outlineColor);
    setcolor(c ? outlineColor : backgroundColor);
    bar(B.x1, B.y1, B.x2, B.y2);
    setfillstyle(1, c ? outlineColor : backgroundColor);
    rectangle(B.x1, B.y1, B.x2, B.y2);
    rectangle(x - 7, y - 7, x + 21, y + 7);
    floodfill(x, y, c ? outlineColor : backgroundColor);
    drawpoly(4, p);
    floodfill(x - 18, y, c ? outlineColor : backgroundColor);
    setfillstyle(1, backgroundColor);
    setcolor(outlineColor);
}

void paintTopButtons()
{
    paintExitButton(top[0], 1);
    paintMainMenuButton(top[1], 1);
    paintSettingsButton(top[2], 1);
    paintBackButton(top[3], 1);
}

///Sliders

void readSliders(bool c)
{
    FILE *colors = fopen("settings", "r");
    fseek(colors, (c ? 0 : 6), SEEK_SET);
    for (int i = 0; i < 6; i++)
    {
        fscanf(colors, "%c", c ? &bgol[i].n : &p12[i].n);
    }
    fclose(colors);
}

void writeSliders(bool c)
{
    FILE *colors = fopen("settings", "r+");
    fseek(colors, (c ? 0 : 6), SEEK_SET);
    for (int i = 0; i < 6; i++)
    {
        fprintf(colors, "%c", c ? bgol[i].n : p12[i].n);
    }
    fclose(colors);
}

void initSlider(SLIDER &A, int x1, int y1, int x2, int y2)
{
    A.x1 = x1;
    A.y1 = y1;
    A.x2 = x2;
    A.y2 = y2;
}

void initSliders()
{
    initSlider(p12[0], 20, 500, 1, 20);
    initSlider(p12[1], 20, 550, 1, 20);
    initSlider(p12[2], 20, 600, 1, 20);
    initSlider(p12[3], 925, 500, 1, 20);
    initSlider(p12[4], 925, 550, 1, 20);
    initSlider(p12[5], 925, 600, 1, 20);
    readSliders(0);

    initSlider(bgol[0], 20, 760, 1, 20);
    initSlider(bgol[1], 20, 810, 1, 20);
    initSlider(bgol[2], 20, 860, 1, 20);
    initSlider(bgol[3], 325, 760, 1, 20);
    initSlider(bgol[4], 325, 810, 1, 20);
    initSlider(bgol[5], 325, 860, 1, 20);
    readSliders(1);
}

bool hoveringOver(SLIDER S)
{
    return mousex() > S.x1 && mousex() < S.x1 + S.x2 * 256 && mousey() > S.y1 && mousey() < S.y1 + S.y2;
}

void paintArrow(int x, int y, bool o)
{
    setfillstyle(1, outlineColor);
    int p[8] = {x, y, x - 5, o ? y + 8 : y - 8 , x + 5, o ? y + 8 : y - 8, x, y};
    drawpoly(4, p);
    floodfill(x, o ? y + 1 : y - 1, outlineColor);
    setfillstyle(1, backgroundColor);
}

void paintSlider(SLIDER S, int c)
{

    setfillstyle(1, c);
    bar(S.x1, S.y1, S.x1 + S.x2 * 256, S.y1 + S.y2); ///maybe we could put an img here with nuances of red/green/blue
    paintArrow(S.x1 + S.x2 * S.n + S.x2 / 2, S.y1 + S.y2, 1);
    paintArrow(S.x1 + S.x2 * S.n + S.x2 / 2, S.y1 - 1, 0);
}

void paintSliders(bool c)
{
    if (c)
    {
        paintSlider(bgol[0], COLOR(255, 0, 0));
        paintSlider(bgol[1], COLOR(0, 255, 0));
        paintSlider(bgol[2], COLOR(0, 0, 255));
        paintSlider(bgol[3], COLOR(255, 0, 0));
        paintSlider(bgol[4], COLOR(0, 255, 0));
        paintSlider(bgol[5], COLOR(0, 0, 255));
    }
    else
    {
        paintSlider(p12[0], COLOR(255, 0, 0));
        paintSlider(p12[1], COLOR(0, 255, 0));
        paintSlider(p12[2], COLOR(0, 0, 255));
        paintSlider(p12[3], COLOR(255, 0, 0));
        paintSlider(p12[4], COLOR(0, 255, 0));
        paintSlider(p12[5], COLOR(0, 0, 255));
    }
}

void paintSelectedInSlider(SLIDER A, unsigned char next)
{
    bar(A.x1 - 5, A.y1 - 9, A.x1 + A.x2 * 256 + 5, A.y1);
    bar(A.x1 - 5, A.y1 + A.y2, A.x1 + A.x2 * 256 + 5, A.y1 + A.y2 + 9);
    paintArrow(A.x1 + A.x2 * next + A.x2 / 2, A.y1 + A.y2, 1);
    paintArrow(A.x1 + A.x2 * next + A.x2 / 2, A.y1 - 1, 0);
}

unsigned char getSliderValue(SLIDER &S)
{
    if (mousex() > S.x1 && mousex() < S.x1 + S.x2 * 255)
    {
        return (mousex() - S.x1) / S.x2;
    }
    else if (mousex() <= S.x1)
    {
        return 0;
    }
    else
    {
        return 255;
    }
}

void setSliderValue(SLIDER &A, bool sk, SLIDER &B, unsigned char v, char s[])
{
    A.n = v;
    if (sk)
    {
        if (st[1].v == 0)
        {
            B.n = 255 - v;
        }
        setBothColors();
        addSettingsButtons(s);
    }
    else
    {
        paintSelectedInSlider(A, v);
        paintSettingsPieces();
    }
    swapbuffers();
}

void setSlider(SLIDER &A, bool sk, SLIDER &B, char s[])
{
    while (GetKeyState(VK_LBUTTON) & 128)
    {
        if (A.n != getSliderValue(A))
        {
            setSliderValue(A, sk, B, getSliderValue(A), s);
        }
    }
    if (sk)
    {
        addSettingsButtons(s);
    }
    else
    {
        paintSelectedInSlider(A, getSliderValue(A));
        paintSettingsPieces();
    }
}

///Toggles

void readSkill(bool m)
{
    unsigned char c;
    FILE *skill = fopen("settings", "r");
    fseek(skill, 44, SEEK_SET);
    fscanf(skill, "%c", &c);
    if (m)
    {
        for (int i = 0; i < 2; i++)
        {
            readBit(c, d[i].v, i);
        }
    }
    else readBit(c, d[1].v, 1);
    fclose(skill);
}

void writeSkill(bool m)
{
    unsigned char c;
    FILE *skill = fopen("settings", "r+");
    fseek(skill, 44, SEEK_SET);
    fscanf(skill, "%c", &c);
    fseek(skill, 44, SEEK_SET);
    if (m)
    {
        for (int i = 0; i < 2; i++)
        {
            writeBit(c, d[i].v, i);
        }
    }
    else writeBit(c, d[1].v, 1);
    fprintf(skill, "%c", c);
    fclose(skill);
}

void initToggle(TOGGLE &T, int x1, int x2, int x3, int y1, int y2, char ENG1[], char ROM1[], char ENG2[], char ROM2[])
{
    initButton(T.B[0], x1, y1, x2, y2, ENG1, ROM1);
    initButton(T.B[1], x2, y1, x3, y2, ENG2, ROM2);
}

void initDToggles()
{
    initToggle(d[0], 20, 150, 280, 320, 420, "Easy", "Usor", "Hard", "Greu");
    initToggle(d[1], 920, 1050, 1180, 320, 420, "Easy", "Usor", "Hard", "Greu");
    readSkill(1);
}

void initSToggles()
{
    initToggle(st[0], 820, 1000, 1180, 20, 80, "Romanian", "Romana", "English", "Engleza");
    initToggle(st[1], 820, 1000, 1180, 100, 160, "On", "Activ", "Off", "Oprit");
    initToggle(st[2], 820, 1000, 1180, 180, 240, "On", "Activ", "Off", "Oprit");
    initToggle(st[3], 820, 1000, 1180, 260, 320, "On", "Activ", "Off", "Oprit");
    initToggle(st[4], 820, 1000, 1180, 340, 400, "Yes", "Da", "No", "Nu");
    initToggle(st[5], 820, 1000, 1180, 420, 480, "On", "Activ", "Off", "Oprit");
    initToggle(st[6], 820, 1000, 1180, 500, 560, "On", "Activ", "Off", "Oprit");
    readSettings();
}

bool hoveringOver(TOGGLE T)
{
    return mousex() > T.B[0].x1 && mousex() < T.B[1].x2 && mousey() > T.B[0].y1 && mousey() < T.B[1].y2;
}

void paintToggle(TOGGLE T)
{
    paintButton(T.B[0], T.v);
    paintButton(T.B[1], !T.v);
}

void paintToggles(TOGGLE T[], int nr)
{
    for (int i = 0; i < nr; i++)
    {
        paintToggle(T[i]);
    }
}

///Textboxes

void readNames(bool m)
{
    FILE *names = fopen("settings", "r");
    fseek(names, 12, SEEK_SET);
    for (int i = 0; i < 12; i++)
    {
        fscanf(names, "%c", &n[0].text[i]);
    }
    if (m)
    {
        for (int i = 0; i < 12; i++)
        {
            fscanf(names, "%c", &n[1].text[i]);
        }
    }
    fclose(names);
}

void writeNames(bool m)
{
    FILE *names = fopen("settings", "r+");
    fseek(names, 12, SEEK_SET);
    for (int i = 0; i < 12; i++)
    {
        fprintf(names, "%c", n[0].text[i]);
    }
    if (m)
    {
        for (int i = 0; i < 12; i++)
        {
            fprintf(names, "%c", n[1].text[i]);
        }
    }
    fclose(names);
}

void readNumbers()
{
    FILE *numbers = fopen("settings", "rb");
    fseek(numbers, 36, SEEK_SET);
    fread(&turnLimit, 8, 1, numbers);
    if (turnLimit)
    {
        convToChar(turnLimit);
        strcpy(stb[0].text, number);
    }
    else strcpy(stb[0].text, "");
    fclose(numbers);
}
    ///had to use binary stuff otherwise it would save the numbers like in a file(readable) and use 9
    ///bytes(999999999 -> 39 39 39 39 39 39 39 39 39) instead of 4(999999999 -> FF C9 9A 3B); minor detail overall
void writeNumbers()
{
    FILE *numbers = fopen("settings", "rb+");
    fseek(numbers, 36, SEEK_SET);
    fwrite(&turnLimit, 8, 1, numbers);
    fclose(numbers);
}

void initTextBox(TEXTBOX &TB, int x1, int x2, int y1, int y2, char ENGP[], char ROMP[])
{
    TB.x1 = x1;
    TB.x2 = x2;
    TB.y1 = y1;
    TB.y2 = y2;
    strcpy(TB.engp, ENGP);
    strcpy(TB.romp, ROMP);
}

void initNTextBoxes()
{
    initTextBox(n[0], 20, 280, 320, 420, "PLAYER1", "JUCATOR1");
    initTextBox(n[1], 920, 1180, 320, 420, "PLAYER2", "JUCATOR2");
    readNames(1);
}

void initSTextBoxes()
{
    initTextBox(stb[0], 820, 1180, 580, 640, "turn number", "numar de ture");
    readNumbers();
}

void initLTextBox()
{
    initTextBox(ltb[0], 20, 300, 120 + llrpos * 100, 180 + llrpos * 100, "save name", "nume joc");
    strcpy(ltb[0].text, llpos -> name);
}

void initCTextBox()
{
    initTextBox(ltb[0], 20, 280, 520, 620, "save name", "nume joc");
    strcpy(ltb[0].text, "");
}

bool hoveringOver(TEXTBOX TB)
{
    return mousex() > TB.x1 && mousex() < TB.x2 && mousey() > TB.y1 && mousey() < TB.y2;
}

void paintTextBox(TEXTBOX TB, bool l)
{
    setfillstyle(1, l ? outlineColor : backgroundColor);
    setcolor(l ? backgroundColor : outlineColor);
    setbkcolor(l ? outlineColor : backgroundColor);
    if (l) settextjustify(0, 1);
    bar(TB.x1 + 1, TB.y1 + 1, TB.x2, TB.y2);
    rectangle(TB.x1, TB.y1, TB.x2, TB.y2);
    outtextxy((l ? TB.x1 + 20 : (TB.x1 + TB.x2) / 2), (TB.y1 + TB.y2) / 2 + 10, strlen(TB.text) ? TB.text : st[0].v ? TB.engp : TB.romp);
    setfillstyle(1, backgroundColor);
    setcolor(outlineColor);
    setbkcolor(backgroundColor);
    settextjustify(1, 1);
}

void paintTextBoxes(TEXTBOX TB[], int nr, bool l)
{
    for (int i = 0; i < nr; i++)
    {
        paintTextBox(TB[i], l);
    }
}

void addToMessage(TEXTBOX &TB, int key, char s, bool c, bool l)
{
    int timer = 500;
    TB.text[strlen(TB.text) + 1] = NULL;
    TB.text[strlen(TB.text)] = s;
    paintTextBox(TB, l);
    swapbuffers();
    while ((GetKeyState(key) & 128) && timer)
    {
        delay(10);
        timer = timer - 10;
    }
    while ((GetKeyState(key) & 128) && strlen(TB.text) < (c ? 9 : 12))
    {
        TB.text[strlen(TB.text) + 1] = NULL;
        TB.text[strlen(TB.text)] = s;
        paintTextBox(TB, l);
        swapbuffers();
        delay(50);
    }
}

void setTextBoxMessage(TEXTBOX &TB, bool c, bool l)
{
    if ((GetKeyState(VK_BACK) & 128) && strlen(TB.text))
    {
        int timer = 500;
        TB.text[strlen(TB.text) - 1] = NULL;
        paintTextBox(TB, l);
        swapbuffers();
        while ((GetKeyState(VK_BACK) & 128) && timer)
        {
            delay(10);
            timer = timer - 10;
        }
        while ((GetKeyState(VK_BACK) & 128) && strlen(TB.text))
        {
            TB.text[strlen(TB.text) - 1] = NULL;
            paintTextBox(TB, l);
            swapbuffers();
            delay(50);
        }
        return;
    }
    if (c)
    {
        if (strlen(TB.text) < 9)
        {
            if (strlen(TB.text))
            {
                if (GetKeyState('0') & 128)
                {
                    addToMessage(TB, '0', '0', c, l);
                    return;
                }
                if (GetKeyState(VK_NUMPAD0) & 128)
                {
                    addToMessage(TB, VK_NUMPAD0, '0', c, l);
                    return;
                }
            }
            if (GetKeyState('1') & 128)
            {
                addToMessage(TB, '1', '1', c, l);
                return;
            }
            if (GetKeyState(VK_NUMPAD1) & 128)
            {
                addToMessage(TB, VK_NUMPAD1, '1', c, l);
                return;
            }
            if (GetKeyState('2') & 128)
            {
                addToMessage(TB, '2', '2', c, l);
                return;
            }
            if (GetKeyState(VK_NUMPAD2) & 128)
            {
                addToMessage(TB, VK_NUMPAD2, '2', c, l);
                return;
            }
            if (GetKeyState('3') & 128)
            {
                addToMessage(TB, '3', '3', c, l);
                return;
            }
            if (GetKeyState(VK_NUMPAD3) & 128)
            {
                addToMessage(TB, VK_NUMPAD3, '3', c, l);
                return;
            }
            if (GetKeyState('4') & 128)
            {
                addToMessage(TB, '4', '4', c, l);
                return;
            }
            if (GetKeyState(VK_NUMPAD4) & 128)
            {
                addToMessage(TB, VK_NUMPAD4, '4', c, l);
                return;
            }
            if (GetKeyState('5') & 128)
            {
                addToMessage(TB, '5', '5', c, l);
                return;
            }
            if (GetKeyState(VK_NUMPAD5) & 128)
            {
                addToMessage(TB, VK_NUMPAD5, '5', c, l);
                return;
            }
            if (GetKeyState('6') & 128)
            {
                addToMessage(TB, '6', '6', c, l);
                return;
            }
            if (GetKeyState(VK_NUMPAD6) & 128)
            {
                addToMessage(TB, VK_NUMPAD6, '6', c, l);
                return;
            }
            if (GetKeyState('7') & 128)
            {
                addToMessage(TB, '7', '7', c, l);
                return;
            }
            if (GetKeyState(VK_NUMPAD7) & 128)
            {
                addToMessage(TB, VK_NUMPAD7, '7', c, l);
                return;
            }
            if (GetKeyState('8') & 128)
            {
                addToMessage(TB, '8', '8', c, l);
                return;
            }
            if (GetKeyState(VK_NUMPAD8) & 128)
            {
                addToMessage(TB, VK_NUMPAD8, '8', c, l);
                return;
            }
            if (GetKeyState('9') & 128)
            {
                addToMessage(TB, '9', '9', c, l);
                return;
            }
            if (GetKeyState(VK_NUMPAD9) & 128)
            {
                addToMessage(TB, VK_NUMPAD9, '9', c, l);
                return;
            }
        }
    }
    else
    {
        if (strlen(TB.text) < 12)
        {
            for (char i = 'A'; i <= 'Z'; i++)
            {
                if (GetKeyState(i) & 128)
                {
                    addToMessage(TB, i, ((GetKeyState(VK_SHIFT) & 128) || (GetKeyState(VK_CAPITAL) & 1)) ? i : i + 32, c, l);
                    return;
                }
            }
            if (GetKeyState('0') & 128)
            {
                addToMessage(TB, '0', (GetKeyState(VK_SHIFT) & 128) ? ')' : '0', c, l);
                return;
            }
            if (GetKeyState(VK_NUMPAD0) & 128)
            {
                addToMessage(TB, VK_NUMPAD0, '0', c, l);
                return;
            }
            if (GetKeyState('1') & 128)
            {
                addToMessage(TB, '1', (GetKeyState(VK_SHIFT) & 128) ? '!' : '1', c, l);
                return;
            }
            if (GetKeyState(VK_NUMPAD1) & 128)
            {
                addToMessage(TB, VK_NUMPAD1, '1', c, l);
                return;
            }
            if (GetKeyState('2') & 128)
            {
                addToMessage(TB, '2', (GetKeyState(VK_SHIFT) & 128) ? '@' : '2', c, l);
                return;
            }
            if (GetKeyState(VK_NUMPAD2) & 128)
            {
                addToMessage(TB, VK_NUMPAD2, '2', c, l);
                return;
            }
            if (GetKeyState('3') & 128)
            {
                addToMessage(TB, '3', (GetKeyState(VK_SHIFT) & 128) ? '#' : '3', c, l);
                return;
            }
            if (GetKeyState(VK_NUMPAD3) & 128)
            {
                addToMessage(TB, VK_NUMPAD3, '3', c, l);
                return;
            }
            if (GetKeyState('4') & 128)
            {
                addToMessage(TB, '4', (GetKeyState(VK_SHIFT) & 128) ? '$' : '4', c, l);
                return;
            }
            if (GetKeyState(VK_NUMPAD4) & 128)
            {
                addToMessage(TB, VK_NUMPAD4, '4', c, l);
                return;
            }
            if (GetKeyState('5') & 128)
            {
                addToMessage(TB, '5', (GetKeyState(VK_SHIFT) & 128) ? '%' : '5', c, l);
                return;
            }
            if (GetKeyState(VK_NUMPAD5) & 128)
            {
                addToMessage(TB, VK_NUMPAD5, '5', c, l);
                return;
            }
            if (GetKeyState('6') & 128)
            {
                addToMessage(TB, '6', (GetKeyState(VK_SHIFT) & 128) ? '^' : '6', c, l);
                return;
            }
            if (GetKeyState(VK_NUMPAD6) & 128)
            {
                addToMessage(TB, VK_NUMPAD6, '6', c, l);
                return;
            }
            if (GetKeyState('7') & 128)
            {
                addToMessage(TB, '7', (GetKeyState(VK_SHIFT) & 128) ? '&' : '7', c, l);
                return;
            }
            if (GetKeyState(VK_NUMPAD7) & 128)
            {
                addToMessage(TB, VK_NUMPAD7, '7', c, l);
                return;
            }
            if (GetKeyState('8') & 128)
            {
                addToMessage(TB, '8', (GetKeyState(VK_SHIFT) & 128) ? '*' : '8', c, l);
                return;
            }
            if (GetKeyState(VK_NUMPAD8) & 128)
            {
                addToMessage(TB, VK_NUMPAD8, '8', c, l);
                return;
            }
            if (GetKeyState('9') & 128)
            {
                addToMessage(TB, '9', (GetKeyState(VK_SHIFT) & 128) ? '(' : '9', c, l);
                return;
            }
            if (GetKeyState(VK_NUMPAD9) & 128)
            {
                addToMessage(TB, VK_NUMPAD9, '9', c, l);
                return;
            }
            if (GetKeyState(VK_OEM_PLUS) & 128)
            {
                addToMessage(TB, VK_OEM_PLUS, ((GetKeyState(VK_SHIFT) & 128) || (GetKeyState(VK_CAPITAL) & 1)) ? '+' : '=', c, l);
                return;
            }
            if (GetKeyState(VK_OEM_COMMA) & 128)
            {
                addToMessage(TB, VK_OEM_COMMA, ((GetKeyState(VK_SHIFT) & 128) || (GetKeyState(VK_CAPITAL) & 1)) ? '<' : ',', c, l);
                return;
            }
            if (GetKeyState(VK_OEM_MINUS) & 128)
            {
                addToMessage(TB, VK_OEM_MINUS, ((GetKeyState(VK_SHIFT) & 128) || (GetKeyState(VK_CAPITAL) & 1)) ? '_' : '-', c, l);
                return;
            }
            if (GetKeyState(VK_OEM_PERIOD) & 128)
            {
                addToMessage(TB, VK_OEM_PERIOD, ((GetKeyState(VK_SHIFT) & 128) || (GetKeyState(VK_CAPITAL) & 1)) ? '>' : '.', c, l);
                return;
            }
            if (GetKeyState(VK_OEM_1) & 128)
            {
                addToMessage(TB, VK_OEM_1, ((GetKeyState(VK_SHIFT) & 128) || (GetKeyState(VK_CAPITAL) & 1)) ? ':' : ';', c, l);
                return;
            }
            if (GetKeyState(VK_OEM_2) & 128)
            {
                addToMessage(TB, VK_OEM_2, ((GetKeyState(VK_SHIFT) & 128) || (GetKeyState(VK_CAPITAL) & 1)) ? '\?' : '/', c, l);
                return;
            }
            if (GetKeyState(VK_OEM_3) & 128)
            {
                addToMessage(TB, VK_OEM_3, ((GetKeyState(VK_SHIFT) & 128) || (GetKeyState(VK_CAPITAL) & 1)) ? '~' : '`', c, l);
                return;
            }
            if (GetKeyState(VK_OEM_4) & 128)
            {
                addToMessage(TB, VK_OEM_4, ((GetKeyState(VK_SHIFT) & 128) || (GetKeyState(VK_CAPITAL) & 1)) ? '{' : '[', c, l);
                return;
            }
            if (GetKeyState(VK_OEM_5) & 128)
            {
                addToMessage(TB, VK_OEM_5, ((GetKeyState(VK_SHIFT) & 128) || (GetKeyState(VK_CAPITAL) & 1)) ? '|' : '\\', c, l);
                return;
            }
            if (GetKeyState(VK_OEM_6) & 128)
            {
                addToMessage(TB, VK_OEM_6, ((GetKeyState(VK_SHIFT) & 128) || (GetKeyState(VK_CAPITAL) & 1)) ? '}' : ']', c, l);
                return;
            }
            if (GetKeyState(VK_OEM_7) & 128)
            {
                addToMessage(TB, VK_OEM_7, ((GetKeyState(VK_SHIFT) & 128) || (GetKeyState(VK_CAPITAL) & 1)) ? '\"' : '\'', c, l);
                return;
            }
            if (GetKeyState(VK_SPACE) & 128)
            {
                addToMessage(TB, VK_SPACE, ' ', c, l);
                return;
            }
        }
    }
}

void setTextBox(TEXTBOX &TB, bool c, bool l)
{
    while (hoveringOver(TB))
    {
        setTextBoxMessage(TB, c, l);
    }
    paintTextBox(TB, l);
}

///LoadRect

void initLoadRect(LOADRECT &LR, int x1, int x2, int y1, int y2)
{
    LR.x1 = x1;
    LR.x2 = x2;
    LR.y1 = y1;
    LR.y2 = y2;
}

void initLoadRects()
{
    for (int i = 0; i < 7; i++)
    {
        initLoadRect(llr[i], 20, 1180, 120 + i * 100, 180 + i * 100);
    }
}

void paintLoadRect(LOADRECT LR, SAVE *S, bool c)
{
    setfillstyle(1, c ? backgroundColor : outlineColor);
    setcolor(c ? outlineColor : backgroundColor);
    setbkcolor(c ? backgroundColor : outlineColor);
    bar(LR.x1 + 1, LR.y1 + 1, LR.x2, LR.y2);
    rectangle(LR.x1, LR.y1, LR.x2, LR.y2);
    settextjustify(0, 1);
    outtextxy(LR.x1 + 20, (LR.y1 + LR.y2) / 2 + 10, S -> name);
    settextjustify(1, 1);
    outtextxy((LR.x1 + LR.x2) / 2, (LR.y1 + LR.y2) / 2 + 10, S -> type);
    settextjustify(2, 1);
    if (st[0].v)
    {
        if (S -> turnLimitActive == 0)
        {
            outtextxy(LR.x2 - 20, (LR.y1 + LR.y2) / 2 + 10, "No turn limit");
        }
        else
        {
            convToChar(S -> turnLimitCopy / 2);
            strcpy(message, number);
            strcat(message, " turns left");
            outtextxy(LR.x2 - 20, (LR.y1 + LR.y2) / 2 + 10, message);
        }
    }
    else
    {
        if (S -> turnLimitActive == 0)
        {
            outtextxy(LR.x2 - 20, (LR.y1 + LR.y2) / 2 + 10, "Fara limita de ture");
        }
        else
        {
            convToChar(S -> turnLimitCopy / 2);
            strcpy(message, number);
            strcat(message, " ture ramase");
            outtextxy(LR.x2 - 20, (LR.y1 + LR.y2) / 2 + 10, message);
        }
    }
    settextjustify(1, 1);
    setfillstyle(1, backgroundColor);
    setcolor(outlineColor);
    setbkcolor(backgroundColor);
}

void paintLoadRects(SAVE *S)
{
    for (int i = 0; i < ((ll.l < 7) ? ll.l : 7); i++)
    {
        paintLoadRect(llr[i], S, (i != llrpos));
        S = S -> next;
    }
}


int selectedInMenus(BUTTON B[], int nrb, SLIDER S[], TOGGLE T[], int nrt, TEXTBOX TB[], int nrtb, bool hasTopButtons, char s[])
{
    ///0 - up, -128 - down, 1 -toggled off, -127 - toggled on
    ///(GetKeyState(VK_LBUTTON) & 128) - 0 when not held down, 128 when held down
    ///(GetKeyState(VK_LBUTTON) & 1) - 0 when toggled off, 1 when toggled on
    for (int i = 0; i < nrb; i++)
    {
        if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(B[i]))
        {
            paintButton(B[i], 0);
            swapbuffers();
            while ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(B[i]));
            swapbuffers();
            paintButton(B[i], 1);
            if (hoveringOver(B[i])) return i;
            return -1;
        }
    }
    if (S)
    {
        for (int i = 0; i < 6; i++)
        {
            if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(S[i]))
            {
                if (strcmp(s, "s") == 0)
                {
                    if (nrt == 5) setSlider(S[i], 1, S[(i + 3) % 6], "PvP");
                    else if (nrt == 6) setSlider(S[i], 1, S[(i + 3) % 6], "EvE");
                    else setSlider(S[i], 1, S[(i + 3) % 6], "mm");
                }
                else setSlider(S[i], 0, S[(i + 3) % 6], NULL);
                return i + nrb;
            }
        }
    }
    for (int i = 0; i < nrt; i++)
    {
        if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(T[i]))
        {
            while ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(T[i]))
            {
                if (hoveringOver(T[i].B[!T[i].v]))
                {
                    T[i].v = !T[i].v;
                    if (strcmp(s, "s") == 0)
                    {
                        if (nrt == 5) addSettingsButtons("PvP");
                        else if (nrt == 6) addSettingsButtons("EvE");
                        else addSettingsButtons("mm");
                    }
                    else paintToggle(T[i]);
                    swapbuffers();
                }
            }
            if (strcmp(s, "s") == 0)
            {
                if (nrt == 5) addSettingsButtons("PvP");
                else if (nrt == 6) addSettingsButtons("EvE");
                else addSettingsButtons("mm");
            }
            else paintToggle(T[i]);
            return i * 2 + T[i].v + nrb + (S ? 6 : 0);
        }
    }
    for (int i = 0; i < nrtb; i++)
    {
        if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(TB[i]))
        {
            char temp[100];
            strcpy(temp, TB[i].text);
            setTextBox(TB[i], strcmp(s, "s") == 0, 0);
            if (strcmp(s, "s") == 0)
            {
                if (strcmp(TB[i].text, "") == 0) turnLimit = 0;
                else turnLimit = convToInt(stb[0].text);
            }
            if (strcmp(s, "PvE") == 0 && strcmp(n[0].text, n[1].text) == 0)
            {
                char aux[13];
                strcpy(aux, n[1].text);
                strcpy(n[1].text, temp);
                strcpy(temp, aux);
            }
            bool ok = true;
            if (strcmp(TB[i].text, ""))
            {
                for (int j = 0; j < nrtb; j++)
                {
                    if (i != j && strcmp(TB[i].text, TB[j].text) == 0) ok = false;
                }
            }
            if (ok) return i * 2 + nrb + (S ? 6 : 0) + nrt * 2;
            strcpy(TB[i].text, temp);
            paintTextBox(TB[i], 0);
            swapbuffers();
            paintTextBox(TB[i], 0);
            return i * 2 + 1 + nrb + (S ? 6 : 0) + nrt * 2;
        }
    }
    if (hasTopButtons)
    {
        if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[0]))
        {
            paintExitButton(top[0], 0);
            swapbuffers();
            while ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[0]));
            swapbuffers();
            paintExitButton(top[0], 1);
            if (hoveringOver(top[0])) return 0 + nrb + (S ? 6 : 0) + nrt * 2 + nrtb * 2;
            return -1;
        }
        if (GetKeyState('B') & 128)
        {
            paintExitButton(top[0], 0);
            swapbuffers();
            while (GetKeyState('B') & 128);
            swapbuffers();
            paintExitButton(top[0], 1);
            return 0 + nrb + (S ? 6 : 0) + nrt * 2 + nrtb * 2;
        }
        if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[1]))
        {
            paintMainMenuButton(top[1], 0);
            swapbuffers();
            while ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[1]));
            swapbuffers();
            paintMainMenuButton(top[1], 1);
            if (hoveringOver(top[1])) return 1 + nrb + (S ? 6 : 0) + nrt * 2 + nrtb * 2;
            return -1;
        }
        if (GetKeyState('M') & 128)
        {
            paintMainMenuButton(top[1], 0);
            swapbuffers();
            while (GetKeyState('M') & 128);
            swapbuffers();
            paintMainMenuButton(top[1], 1);
            return 1 + nrb + (S ? 6 : 0) + nrt * 2 + nrtb * 2;
        }
        if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[2]))
        {
            paintSettingsButton(top[2], 0);
            swapbuffers();
            while ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[2]));
            swapbuffers();
            paintSettingsButton(top[2], 1);
            if (hoveringOver(top[2])) return 2 + nrb + (S ? 6 : 0) + nrt * 2 + nrtb * 2;
            return -1;
        }
        if (GetKeyState(VK_OEM_3) & 128)
        {
            paintSettingsButton(top[2], 0);
            swapbuffers();
            while (GetKeyState(VK_OEM_3) & 128);
            swapbuffers();
            paintSettingsButton(top[2], 1);
            return 2 + nrb + (S ? 6 : 0) + nrt * 2 + nrtb * 2;
        }
        if ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[3]))
        {
            paintBackButton(top[3], 0);
            swapbuffers();
            while ((GetKeyState(VK_LBUTTON) & 128) && hoveringOver(top[3]));
            swapbuffers();
            paintBackButton(top[3], 1);
            if (hoveringOver(top[3])) return 3 + nrb + (S ? 6 : 0) + nrt * 2 + nrtb * 2;
            return -1;
        }
        if (GetKeyState('B') & 128)
        {
            paintBackButton(top[3], 0);
            swapbuffers();
            while (GetKeyState('B') & 128);
            swapbuffers();
            paintBackButton(top[3], 1);
            return 3 + nrb + (S ? 6 : 0) + nrt * 2 + nrtb * 2;
        }
    }
    return -1;
}

void makeMessage(char s[])
{
    if (st[6].v && turnLimitCopy == 1)
    {
        if (st[0].v)
        {
            strcpy(message, "The Game ended in a draw.");
        }
        else
        {
            strcpy(message, "Jocul a fost incheiat printr-o remiza.");
        }
    }
    else
    {
        if (st[0].v)
        {
            if (strcmp(s, "PvP") == 0)
            {
                strcpy(message, n[(turn ^ (getPM(0) == 0))].text);
            }
            else if (strcmp(s, "PvE") == 0)
            {
                strcpy(message, (turn ^ (getPM(0) == 0)) ? "The Computer" : n[0].text);
            }
            else if (strcmp(s, "EvE") == 0)
            {
                strcpy(message, "The ");
                strcat(message, (turn ^ (getPM(0) == 0)) ? "Second" : "First");
                strcat(message, " Computer");
            }
            strcat(message, " has ");
            if (getPM(0) == 0)
            {
                strcat(message, "won.");
            }
            else
            {
                convToChar(getPM(0));
                strcat(message, number);
                strcat(message, " way");
                if (getPM(0) != 1)
                {
                    strcat(message, "s");
                }
                strcat(message, " to move its piece.");
            }
        }
        else
        {
            if (strcmp(s, "PvP") == 0)
            {
                strcpy(message, n[(turn ^ (getPM(0) == 0))].text);
            }
            else if (strcmp(s, "PvE") == 0)
            {
                strcpy(message, (turn ^ (getPM(0) == 0)) ? "Calculatorul" : n[0].text);
            }
            else if (strcmp(s, "EvE") == 0)
            {
                strcpy(message, (turn ^ (getPM(0) == 0)) ? "Al Doilea" : "Primul");
                strcat(message, " Calculator");
            }
            if (getPM(0) == 0)
            {
                strcat(message, " a castigat.");
            }
            else
            {
                strcat(message, " isi poate muta piesa in ");
                convToChar(getPM(0));
                strcat(message, number);
                if (getPM(0) == 1)
                {
                    strcat(message, " mod.");
                }
                else
                {
                    strcat(message, " moduri.");
                }
            }
        }
    }
    bar(0, 100, 1200, 200);
    outtextxy(600, 160, message);
}

#endif
