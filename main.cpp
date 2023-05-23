#include <iostream>
#include <cmath>
#include <graphics.h>
#include <winbgim.h>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <fstream>

using namespace std;

#include "important_variables_and_structures.h"
#include "arena.h"
#include "buttons.h"
#include "node_functions.h"
#include "possible_moves_and_move_functions.h"

///0 - up, -128 - down, 1 -toggled off, -127 - toggled on
///(GetKeyState(VK_LBUTTON) & 128) - 0 when not held down, 128 when held down
///(GetKeyState(VK_LBUTTON) & 1) - 0 when toggled off, 1 when toggled on

void mainMenu(); void play(); void exit(char []); void settings(char []); void addPlayButtons();
void addMainMenuButtons(); void addPvPSettingsButtons(); void addPvESettingsButtons(); void addEvESettingsButtons();
void addLoadButtons();

void initCommonButtons()
{
    initButton(cb[0], 320, 820, 580, 880, "Previous", "Anterior");
    initButton(cb[1], 620, 820, 880, 880, "Next", "Urmator");
    initButton(cb[2], 20, 420, 280, 480, "Save", "Salveaza");
    initButton(cb[3], 20, 320, 280, 380, "Skip", "Sari");
}

void addCommonButtons(char s[])
{
    clearviewport();
    paintButtons(cb, ((strcmp(s, "EvE") && st[2].v) ? 4 : 3));
    paintTopButtons();

    paintBoardLines();
    paintBoard();
    makeMessage(s);
    if (st[6].v) paintNumberOfTurns();
}

void PvP(char prev[])
{
    bool oldToggle = st[6].v;
    if (strcmp(prev, "l") == 0)
    {
        st[6].v = llpos -> turnLimitActive;
        if (st[6].v) turnLimitCopy = llpos -> turnLimitCopy;
        turn = llpos -> turn;
    }
    if (strcmp(prev, "l"))
    {
        if (st[6].v) turnLimitCopy = turnLimit * 2 + 1;
        turn = 0;
        if (st[4].v) initRandomStart("PvP");
        else initNormalStart();
        initGP();
    }
    turnEnded = 1;
    setBothPages(addCommonButtons, "PvP");
    while (exitCalled == 0 && mainMenuCalled == 0)
    {
        selected = selectedInPlayArea("PvP");
        if (selected == -1)
        {
            ///cout << "PvP: Nothing" << endl;
        }
        else if (selected == 0)
        {
            selected = turnEnded ? pieceMove() : neutralMove("PvP");
            if (selected == -1)
            {
                ///cout << "PvP: Nothing" << endl;
            }
            else if (selected == 0)
            {
                cout << "PvP: The " << (turn ? "Second Player" : "First Player") << " Failed To Move Its Piece" << endl;
            }
            else if (selected == 1)
            {
                cout << "PvP: The " << (turn ? "Second Player" : "First Player") << " Moved Its Piece" << endl;
            }
            else if (selected == 2)
            {
                cout << "PvP: The " << (turn ? "Second Player" : "First Player") << " Failed To Move A Neutral Piece" << endl;
            }
            else if (selected == 3)
            {
                cout << "PvP: The " << (turn ? "First Player" : "Second Player") << " Moved A Neutral Piece" << endl;
            }
        }
        else if (selected == 1)
        {
            cout << "PvP: Previous" << endl;
            if (turnEnded) setGame(-1, "PvP");
            else setGame(0, "PvP");
        }
        else if (selected == 2)
        {
            cout << "PvP: Next" << endl;
            setGame(1, "PvP");
        }
        else if (selected == 3)
        {
            cout << "PvP: Save" << endl;

        }
        else if (selected == 4)
        {
            cout << "PvP: The " << (turn ? "First Player" : "Second Player") << " Skipped Moving A Neutral Piece" << endl;
            turnEnded = 1;
            turn = !turn;
            addGPToStack();
            makeMessage("PvP");
            swapbuffers();
            bar(0, 100, 1200, 200);
            outtextxy(600, 160, message);
        }
        else if (selected == 5)
        {
            cout << "PvP: Successfully Saved The Game" << endl;
        }
        else if (selected == 6)
        {
            cout << "PvP: Exit" << endl;
            exit("PvP");
        }
        else if (selected == 7)
        {
            cout << "PvP: Main Menu" << endl;
            setBothPages(addMainMenuButtons);
            mainMenuCalled = 1;
        }
        else if (selected == 8)
        {
            cout << "PvP: Settings" << endl;
            settings("PvP");
        }
        else if (selected == 9)
        {
            if (strcmp(prev, "PvPS") == 0)
            {
                cout << "PvP: Back -> PvPSettings" << endl;
                setBothPages(addPvPSettingsButtons);
            }
            else if (strcmp(prev, "l") == 0)
            {
                cout << "PvP: Back -> Load" << endl;
                setBothPages(addLoadButtons);
            }
            break;
        }
    }
    if (strcmp(prev, "l") == 0) st[6].v = oldToggle;
}

void PvE(char prev[])
{
    bool oldToggle = st[6].v;
    if (strcmp(prev, "l") == 0)
    {
        st[6].v = llpos -> turnLimitActive;
        if (st[6].v) turnLimitCopy = llpos -> turnLimitCopy;
        turn = llpos -> turn;
    }
    else
    {
        if (st[6].v) turnLimitCopy = turnLimit * 2 + 1;
        turn = 0;
        if (st[4].v) initRandomStart("PvE");
        else initNormalStart();
        initGP();
    }
    turnEnded = 1;
    setBothPages(addCommonButtons, "PvE");
    while (exitCalled == 0 && mainMenuCalled == 0)
    {
        selected = turn ? AIMove("PvE") : selectedInPlayArea("PvE");
        if (selected == -3)
        {
            cout << "PvE: The AI Moved A Neutral Piece" << endl;
        }
        else if (selected == -2)
        {
            cout << "PvE: The AI Skipped Moving A Neutral Piece" << endl;
        }
        else if (selected == -1)
        {
            ///cout << "PvE: Nothing" << endl;
        }
        else if (selected == 0 && turn == 0)
        {
            selected = turnEnded ? pieceMove() : neutralMove("PvE");
            if (selected == -1)
            {
                ///cout << "PvE: Nothing" << endl;
            }
            else if (selected == 0)
            {
                cout << "PvE: You Failed To Move Your Piece" << endl;
            }
            else if (selected == 1)
            {
                cout << "PvE: You Moved Your Piece" << endl;
            }
            else if (selected == 2)
            {
                cout << "PvE: You Failed To Move A Neutral Piece" << endl;
            }
            else if (selected == 3)
            {
                cout << "PvE: You Moved A Neutral Piece" << endl;
            }
        }
        else if (selected == 1)
        {
            cout << "PvE: Previous" << endl;
            if (turnEnded)
            {
                if (!getPM(0) && turn) setGame(-1, "PvE");
                else setGame(-2, "PvE");
            }
            else setGame(0, "PvE");
        }
        else if (selected == 2)
        {
            cout << "PvE: Next" << endl;
            if (cgp -> next -> next) setGame(2, "PvE");
            else setGame(1, "PvE");
        }
        else if (selected == 3)
        {
            cout << "PvE: Save" << endl;
        }
        else if (selected == 4)
        {
            cout << "PvE: You Skipped Moving A Neutral Piece" << endl;
            turnEnded = 1;
            turn = !turn;
            addGPToStack();
            makeMessage("PvE");
            swapbuffers();
            bar(0, 100, 1200, 200);
            outtextxy(600, 160, message);
        }
        else if (selected == 5)
        {
            cout << "PvE: Successfully Saved The Game" << endl;
        }
        else if (selected == 6)
        {
            cout << "PvE: Exit" << endl;
            exit("PvE");
        }
        else if (selected == 7)
        {
            cout << "PvE: Main Menu" << endl;
            setBothPages(addMainMenuButtons);
            mainMenuCalled = 1;
        }
        else if (selected == 8)
        {
            cout << "PvE: Settings" << endl;
            settings("PvE");
        }
        else if (selected == 9)
        {
            if (strcmp(prev, "PvES") == 0)
            {
                cout << "PvE: Back -> PvESettings" << endl;
                setBothPages(addPvESettingsButtons);
            }
            else if (strcmp(prev, "l") == 0)
            {
                cout << "PvE: Back -> Load" << endl;
                setBothPages(addLoadButtons);
            }
            break;
        }
    }
    if (strcmp(prev, "l") == 0) st[6].v = oldToggle;
}

void EvE(char prev[])
{
    bool oldToggle = st[6].v;
    if (strcmp(prev, "l") == 0)
    {
        st[6].v = llpos -> turnLimitActive;
        if (st[6].v) turnLimitCopy = llpos -> turnLimitCopy;
        turn = llpos -> turn;
    }
    else
    {
        if (st[6].v) turnLimitCopy = turnLimit * 2 + 1;
        turn = 0;
        if (st[4].v) initRandomStart("EvE");
        else initNormalStart();
        initGP();
    }
    turnEnded = 1;
    setBothPages(addCommonButtons, "EvE");
    while (exitCalled == 0 && mainMenuCalled == 0)
    {
        selected = selectedInPlayArea("EvE");
        if (selected == -1)
        {
            ///cout << "EvE: Nothing" << endl;
        }
        else if (selected == 0)
        {
            ///cout << "EvE: Even More Nothing" << endl;
        }
        else if (selected == 1)
        {
            cout << "EvE: Previous" << endl;
            setGame(-1, "EvE");
        }
        else if (selected == 2)
        {
            cout << "EvE: Next" << endl;
            if (cgp != gp.last && !st[3].v) setGame(1, "EvE");
            else AIMove("EvE");
        }
        else if (selected == 3)
        {
            cout << "EvE: Save" << endl;
        }
        else if (selected == 4)
        {
            cout << "EvE: Skip" << endl;    ///double redundant
        }
        else if (selected == 5)
        {
            cout << "EvE: Successfully Saved The Game" << endl;
        }
        else if (selected == 6)
        {
            cout << "EvE: Exit" << endl;
            exit("EvE");
        }
        else if (selected == 7)
        {
            cout << "EvE: Main Menu" << endl;
            setBothPages(addMainMenuButtons);
            mainMenuCalled = 1;
        }
        else if (selected == 8)
        {
            cout << "EvE: Settings" << endl;
            settings("EvE");
        }
        else if (selected == 9)
        {
            if (strcmp(prev, "EvES") == 0)
            {
                cout << "EvE: Back -> EvESettings" << endl;
                setBothPages(addEvESettingsButtons);
            }
            else if (strcmp(prev, "l") == 0)
            {
                cout << "EvE: Back -> Load" << endl;
                setBothPages(addLoadButtons);
            }
            break;
        }
    }
    if (strcmp(prev, "l") == 0) st[6].v = oldToggle;
}


void initEvESettingsButtons()
{
    initButton(EvESb[0], 20, 820, 1180, 880, "Start", "Incepe");
}

void addEvESettingsButtons()
{
    clearviewport();
    paintBoardLines();
    paintSettingsPieces();
    if (st[0].v)
    {
        outtextxy(150, 140, "First");
        outtextxy(150, 180, "Computer");
        outtextxy(150, 280, "Difficulty:");
        outtextxy(150, 480, "Color:");

        outtextxy(1050, 140, "Second");
        outtextxy(1050, 180, "Computer");
        outtextxy(1050, 280, "Difficulty:");
        outtextxy(1050, 480, "Color:");
    }
    else
    {
        outtextxy(150, 140, "Primul");
        outtextxy(150, 180, "Calculator");
        outtextxy(150, 280, "Dificultate:");
        outtextxy(150, 480, "Culoare:");

        outtextxy(1050, 140, "Al Doilea");
        outtextxy(1050, 180, "Calculator");
        outtextxy(1050, 280, "Dificultate:");
        outtextxy(1050, 480, "Culoare:");
    }
    paintSliders(0);
    paintToggles(d, 2);
    paintButtons(EvESb, 1);
    paintTopButtons();
}

void EvESettings()
{
    setBothPages(addEvESettingsButtons);
    while (exitCalled == 0 && mainMenuCalled == 0)
    {
        selected = selectedInMenus(EvESb, 1, p12, d, 2, NULL, 0, 1, "EvE");
        if (selected == -1)
        {
            ///cout << "EvESettings: Nothing" << endl;
        }
        else if (selected == 0)
        {
            cout << "EvESettings: Start" << endl;
            writeSkill(1);
            writeSliders(0);
            EvE("EvES");
        }
        else if (selected == 1)
        {
            cout << "EvESettings: Changed The First Computer's Red Nuance" << endl;
        }
        else if (selected == 2)
        {
            cout << "EvESettings: Changed The First Computer's Green Nuance" << endl;
        }
        else if (selected == 3)
        {
            cout << "EvESettings: Changed The First Computer's Blue Nuance" << endl;
        }
        else if (selected == 4)
        {
            cout << "EvESettings: Changed The Second Computer's Red Nuance" << endl;
        }
        else if (selected == 5)
        {
            cout << "EvESettings: Changed The Second Computer's Green Nuance" << endl;
        }
        else if (selected == 6)
        {
            cout << "EvESettings: Changed The Second Computer's Blue Nuance" << endl;
        }
        else if (selected == 7)
        {
            cout << "EvESettings: Changed The First Computer's Difficulty To Easy" << endl;
        }
        else if (selected == 8)
        {
            cout << "EvESettings: Changed The First Computer's Difficulty To Hard" << endl;
        }
        else if (selected == 9)
        {
            cout << "EvESettings: Changed The Second Computer's Difficulty To Easy" << endl;
        }
        else if (selected == 10)
        {
            cout << "EvESettings: Changed The Second Computer's Difficulty To Hard" << endl;
        }
        else if (selected == 11)
        {
            cout << "EvESettings: Exit" << endl;
            exit("EvES");
        }
        else if (selected == 12)
        {
            cout << "EvESettings: Main Menu" << endl;
            readSkill(1);
            setBothPages(addMainMenuButtons);
            mainMenuCalled = 1;
        }
        else if (selected == 13)
        {
            cout << "EvESettings: Settings" << endl;
            settings("EvES");
        }
        else if (selected == 14)
        {
            cout << "EvESettings: Back -> Play" << endl;
            readSkill(1);
            setBothPages(addPlayButtons);
            break;
        }
    }
}


void initPvESettingsButtons()
{
    initButton(PvESb[0], 20, 820, 1180, 880, "Start", "Incepe");
}

void addPvESettingsButtons()
{
    clearviewport();
    paintBoardLines();
    paintSettingsPieces();
    if (st[0].v)
    {
        outtextxy(150, 180, "You");
        outtextxy(150, 280, "Name:");
        outtextxy(150, 480, "Color:");

        outtextxy(1050, 180, "The Computer");
        outtextxy(1050, 280, "Difficulty:");
        outtextxy(1050, 480, "Color:");
    }
    else
    {
        outtextxy(150, 180, "Tu");
        outtextxy(150, 280, "Nume:");
        outtextxy(150, 480, "Culoare:");

        outtextxy(1050, 180, "Calculatorul");
        outtextxy(1050, 280, "Dificultate:");
        outtextxy(1050, 480, "Culoare:");
    }
    paintSliders(0);
    paintTextBox(n[0], 0);
    paintToggle(d[1]);
    paintButtons(PvESb, 1);
    paintTopButtons();
}

void PvESettings()
{
    setBothPages(addPvESettingsButtons);
    while (exitCalled == 0 && mainMenuCalled == 0)
    {
        selected = selectedInMenus(PvESb, 1, p12, d + 1, 1, n, 1, 1, "PvE");
        if (selected == -1)
        {
            ///cout << "PvESettings: Nothing" << endl;
        }
        else if (selected == 0)
        {
            cout << "PvESettings: Start" << endl;
            writeNames(0);
            writeSkill(0);
            writeSliders(0);
            PvE("PvES");
        }
        else if (selected == 1)
        {
            cout << "PvESettings: Changed The Player's Red Nuance" << endl;
        }
        else if (selected == 2)
        {
            cout << "PvESettings: Changed The Player's Green Nuance" << endl;
        }
        else if (selected == 3)
        {
            cout << "PvESettings: Changed The Player's Blue Nuance" << endl;
        }
        else if (selected == 4)
        {
            cout << "PvESettings: Changed The Computer's Red Nuance" << endl;
        }
        else if (selected == 5)
        {
            cout << "PvESettings: Changed The Computer's Green Nuance" << endl;
        }
        else if (selected == 6)
        {
            cout << "PvESettings: Changed The Computer's Blue Nuance" << endl;
        }
        else if (selected == 7)
        {
            cout << "EvESettings: Changed The Computer's Difficulty To Easy" << endl;
        }
        else if (selected == 8)
        {
            cout << "EvESettings: Changed The Computer's Difficulty To Hard" << endl;
        }
        else if (selected == 9)
        {
            cout << "EvESettings: Changed The Player's Name" << endl;
        }
        else if (selected == 10)
        {
            cout << "EvESettings: Failed To Change The Player's Name" << endl;
        }
        else if (selected == 11)
        {
            cout << "PvESettings: Exit" << endl;
            exit("PvES");
        }
        else if (selected == 12)
        {
            cout << "PvESettings: Main Menu" << endl;
            readNames(0);
            readSkill(0);
            setBothPages(addMainMenuButtons);
            mainMenuCalled = 1;
        }
        else if (selected == 13)
        {
            cout << "PvESettings: Settings" << endl;
            settings("PvES");
        }
        else if (selected == 14)
        {
            cout << "PvESettings: Back -> Play" << endl;
            readNames(0);
            readSkill(0);
            setBothPages(addPlayButtons);
            break;
        }
    }
}


void initPvPSettingsButtons()
{
    initButton(PvPSb[0], 20, 820, 1180, 880, "Start", "Incepe");
}

void addPvPSettingsButtons()
{
    clearviewport();
    paintBoardLines();
    paintSettingsPieces();
    if (st[0].v)
    {
        outtextxy(150, 140, "First");
        outtextxy(150, 180, "Player");
        outtextxy(150, 280, "Name:");
        outtextxy(150, 480, "Color:");

        outtextxy(1050, 140, "Second");
        outtextxy(1050, 180, "Player");
        outtextxy(1050, 280, "Name:");
        outtextxy(1050, 480, "Color:");
    }
    else
    {
        outtextxy(150, 140, "Primul");
        outtextxy(150, 180, "Jucator");
        outtextxy(150, 280, "Nume:");
        outtextxy(150, 480, "Culoare:");

        outtextxy(1050, 140, "Al Doilea");
        outtextxy(1050, 180, "Jucator");
        outtextxy(1050, 280, "Nume:");
        outtextxy(1050, 480, "Culoare:");
    }
    paintSliders(0);
    paintTextBoxes(n, 2, 0);
    paintButtons(PvPSb, 1);
    paintTopButtons();
}

void PvPSettings()
{
    setBothPages(addPvPSettingsButtons);
    while (exitCalled == 0 && mainMenuCalled == 0)
    {
        selected = selectedInMenus(PvPSb, 1, p12, NULL, 0, n, 2, 1, "PvP");
        if (selected == -1)
        {
            ///cout << "PvPSettings: Nothing" << endl;
        }
        else if (selected == 0)
        {
            cout << "PvPSettings: Start" << endl;
            writeNames(1);
            writeSliders(0);
            PvP("PvPS");
        }
        else if (selected == 1)
        {
            cout << "PvPSettings: Changed The First Player's Red Nuance" << endl;
        }
        else if (selected == 2)
        {
            cout << "PvPSettings: Changed The First Player's Green Nuance" << endl;
        }
        else if (selected == 3)
        {
            cout << "PvPSettings: Changed The First Player's Blue Nuance" << endl;
        }
        else if (selected == 4)
        {
            cout << "PvPSettings: Changed The Second Player's Red Nuance" << endl;
        }
        else if (selected == 5)
        {
            cout << "PvPSettings: Changed The Second Player's Green Nuance" << endl;
        }
        else if (selected == 6)
        {
            cout << "PvPSettings: Changed The Second Player's Blue Nuance" << endl;
        }
        else if (selected == 7)
        {
            cout << "PvPSettings: Changed The First Player's Name" << endl;
        }
        else if (selected == 8)
        {
            cout << "PvPSettings: Failed To Change The First Player's Name" << endl;
        }
        else if (selected == 9)
        {
            cout << "PvPSettings: Changed The Second Player's Name" << endl;
        }
        else if (selected == 10)
        {
            cout << "PvPSettings: Failed To Change The Second Player's Name" << endl;
        }
        else if (selected == 11)
        {
            cout << "PvPSettings: Exit" << endl;
            exit("PvPS");
        }
        else if (selected == 12)
        {
            cout << "PvPSettings: Main Menu" << endl;
            readNames(1);
            setBothPages(addMainMenuButtons);
            mainMenuCalled = 1;
        }
        else if (selected == 13)
        {
            cout << "PvPSettings: Settings" << endl;
            settings("PvPS");
        }
        else if (selected == 14)
        {
            cout << "PvPSettings: Back -> Play" << endl;
            readNames(1);
            setBothPages(addPlayButtons);
            break;
        }
    }
}


void initLoadButtons()
{
    initButton(lb[0], 220, 820, 480, 880, "Delete Game", "Sterge Joc");
    initButton(lb[1], 520, 820, 880, 880, "Rename Game", "Redenumeste Joc");
    initButton(lb[2], 920, 820, 1180, 880, "Load Game", "Incarca Joc");
}

void addLoadButtons()
{
    clearviewport();
    paintButtons(lb, ll.l ? 3 : 0);
    paintTopButtons();
    paintLoadRects(llfs);
}

void load()
{
    llpos = ll.first;
    llfs = ll.first;
    llrpos = 0;

    setBothPages(addLoadButtons);
    while (exitCalled == 0 && mainMenuCalled == 0)
    {
        selected = selectedInLoad();
        if (selected == -1)
        {
            ///cout << "Load: Nothing" << endl;
        }
        else if (selected == 0)
        {
            cout << "Load: Deleted A Game" << endl;
        }
        else if (selected == 1)
        {
            cout << "Load: Begun Renaming A Game" << endl;
        }
        else if (selected == 2)
        {
            cout << "Load: Loaded A Game" << endl;
            if (strcmp(llpos -> type, "PvP") == 0)
            {
                PvP("l");
            }
            else if (strcmp(llpos -> type, "PvE") == 0)
            {
                PvE("l");
            }
            else if (strcmp(llpos -> type, "EvE") == 0)
            {
                EvE("l");
            }
        }
        else if (selected == 3)
        {
            cout << "Load: Renamed A Game" << endl;
        }
        else if (selected == 4)
        {
            cout << "Load: Exit" << endl;
            exit("l");
        }
        else if (selected == 5)
        {
            cout << "Load: Main Menu" << endl;
            setBothPages(addMainMenuButtons);
            mainMenuCalled = 1;
        }
        else if (selected == 6)
        {
            cout << "Load: Settings" << endl;
            settings("l");
        }
        else if (selected == 7)
        {
            cout << "Load: Back -> Play" << endl;
            setBothPages(addPlayButtons);
            break;
        }
    }
}


void initPlayButtons()
{
    initButton(pb[0], 20, 520, 1180, 580, "PvE", "Jucator vs Calculator");
    initButton(pb[1], 20, 620, 1180, 680, "PvP", "Jucator vs Jucator");
    initButton(pb[2], 20, 720, 1180, 780, "EvE", "Calculator vs Calculator");
    initButton(pb[3], 20, 820, 1180, 880, "Load", "Incarca");
}

void addPlayButtons()
{
    clearviewport();
    paintButtons(pb, 4);
    paintTopButtons();
}

void play()
{
    setBothPages(addPlayButtons);
    while (exitCalled == 0 && mainMenuCalled == 0)
    {
        selected = selectedInMenus(pb, 4, NULL, NULL, 0, NULL, 0, 1, "p");
        if (selected == -1)
        {
            ///cout << "Play: Nothing" << endl;
        }
        else if (selected == 0)
        {
            cout << "Play: PvESettings" << endl;
            PvESettings();
        }
        else if (selected == 1)
        {
            cout << "Play: PvPSettings" << endl;
            PvPSettings();
        }
        else if (selected == 2)
        {
            cout << "Play: EvESettings" << endl;
            EvESettings();
        }
        else if (selected == 3)
        {
            cout << "Play: Load" << endl;
            load();
        }
        else if (selected == 4)
        {
            cout << "Play: Exit" << endl;
            exit("p");
        }
        else if (selected == 5)
        {
            cout << "Play: Main Menu" << endl;
            setBothPages(addMainMenuButtons);
            mainMenuCalled = 1;
        }
        else if (selected == 6)
        {
            cout << "Play: Settings" << endl;
            settings("p");
        }
        else if (selected == 7)
        {
            cout << "Play: Back -> Main Menu" << endl;
            setBothPages(addMainMenuButtons);
            break;
        }
    }
}


void initHowToButtons()
{
    initButton(htb[0], 320, 820, 480, 880, "Back", "Inapoi");
    initButton(htb[1], 520, 820, 1180, 880, "Appreciate Tudor's Work", "Apreciaza Munca Lui Tudor");
}

void addHowToButtons()
{
    clearviewport();
    if (st[0].v)
    {
        outtextxy(600, 40, "L Game is a two player abstract strategy");
        outtextxy(600, 80, "turn-based board game. It is played on a 4x4");
        outtextxy(600, 120, "board on which four pieces are placed: two");
        outtextxy(600, 160, "3x2 L-shaped tetrominos, each corresponding");
        outtextxy(600, 200, "to one of the players, and two 1x1 neutral");
        outtextxy(600, 240, "circular pieces. Every turn, the active player");
        outtextxy(600, 280, "mandatorily moves their respective tetromino");
        outtextxy(600, 320, "in such a way that it occupies a position");
        outtextxy(600, 360, "distinct from the starting one and fills");
        outtextxy(600, 400, "previously unoccupied by any other piece.");
        outtextxy(600, 440, "Translations, rotations and mirrors are all");
        outtextxy(600, 480, "allowed. Then, the active player optionally");
        outtextxy(600, 520, "moves either circular piece to an unoccupied");
        outtextxy(600, 560, "square. Priority passes thereby. Turns");
        outtextxy(600, 600, "alternate until a player is no longer able to");
        outtextxy(600, 640, "move their tetromino, wherefore their opponent");
        outtextxy(600, 680, "wins.");
    }
    else
    {
        outtextxy(600, 40, "Jocul L este un joc de strategie abstracta");
        outtextxy(600, 80, "intre doi jucatori care muta pe rand. Se");
        outtextxy(600, 120, "desfasoara pe o tabla de dimensiune 4x4 pe");
        outtextxy(600, 160, "care sunt amplasate patru piese: doua");
        outtextxy(600, 200, "tetrominouri de dimensiune 3x2 in forma");
        outtextxy(600, 240, "de L, fiecare corespunzand unuia dintre");
        outtextxy(600, 280, "jucatori si doua piese circulare de");
        outtextxy(600, 320, "dimensiune 1x1. In fiecare tura, jucatorul");
        outtextxy(600, 360, "activ trebuie sa isi mute tetrominoul personal");
        outtextxy(600, 400, "in asa fel incat acesta sa ocupe o pozitie");
        outtextxy(600, 440, "distincta de pozitia initiala, fara a se");
        outtextxy(600, 480, "juxtapune pe celelalte piese de pe tabla.");
        outtextxy(600, 520, "Apoi, jucatorul activ poate muta oricare");
        outtextxy(600, 560, "dintre piesele circulare pe o pozitie");
        outtextxy(600, 600, "libera. Dupa, randul vine urmatorului");
        outtextxy(600, 640, "jucator. Turele alterneaza pana cand un");
        outtextxy(600, 680, "jucator nu mai poate muta, situatie in");
        outtextxy(600, 720, "care oponentul castiga jocul.");
    }
    paintButtons(htb, 2);
}

void howTo()
{
    setBothPages(addHowToButtons);
    while (exitCalled == 0)
    {
        selected = selectedInMenus(htb, 2, NULL, NULL, 0, NULL, 0, 0, "ht");
        if (selected == -1)
        {
            ///cout << "How To: Nothing" << endl;
        }
        else if (selected == 0)
        {
            cout << "How To: Back -> Main Menu" << endl;
            setBothPages(addMainMenuButtons);
            break;
        }
        else if (selected == 1)
        {
            cout << "How To: Appreciate Tudor's Work" << endl;
            exitCalled = 1;
        }
    }
}


void initSettingsButtons()
{
    initButton(sb[0], 820, 820, 980, 880, "Back", "Inapoi");
    initButton(sb[1], 1020, 820, 1180, 880, "Apply", "Aplica");
}

void addSettingsButtons(char prev[])
{
    clearviewport();
    if (st[0].v)
    {
        outtextxy(650, 60, "Language");
        outtextxy(650, 140, "Contrast lock");
        outtextxy(650, 220, "No skip circle");
        outtextxy(650, 300, "No takebacks");
        outtextxy(650, 380, "Classic start");
        if (strcmp(prev, "PvP")) outtextxy(650, 460, "Merciless AI");
        if (strcmp(prev, "PvE") && strcmp(prev, "EvE") && strcmp(prev, "PvP")) outtextxy(650, 540, "Unlimited Games");
    }
    else
    {
        outtextxy(650, 60, "Limba");
        outtextxy(650, 140, "Contrast fix");
        outtextxy(650, 220, "Cerc fortat");
        outtextxy(650, 300, "Fara ia inapoi");
        outtextxy(650, 380, "Start clasic");
        if (strcmp(prev, "PvP")) outtextxy(650, 460, "IA fara mila");
        if (strcmp(prev, "PvE") && strcmp(prev, "EvE") && strcmp(prev, "PvP")) outtextxy(650, 540, "Jocuri Nelimitate");
    }
    paintSliders(1);
    if (st[6].v && strcmp(prev, "PvP") && strcmp(prev, "PvE") && strcmp(prev, "EvE")) paintTextBoxes(stb, 1, 0);
    if (strcmp(prev, "PvP") == 0) paintToggles(st, 5);
    else if (strcmp(prev, "PvE") == 0 || strcmp(prev, "EvE") == 0) paintToggles(st, 6);
    else paintToggles(st, 7);
    paintButtons(sb, 2);
}

void settings(char prev[])
{
    setBothPages(addSettingsButtons, prev);
    while (exitCalled == 0)
    {
        if (strcmp(prev, "PvP") == 0)
        {
            selected = selectedInMenus(sb, 2, bgol, st, 5, NULL, 0, 0, "s");
        }
        else if (strcmp(prev, "PvE") == 0 || strcmp(prev, "EvE") == 0)
        {
            selected = selectedInMenus(sb, 2, bgol, st, 6, NULL, 0, 0, "s");
        }
        else
        {
            selected = selectedInMenus(sb, 2, bgol, st, 7, (st[6].v ? stb : NULL), (st[6].v ? 1 : 0), 0, "s");
        }
        if (selected == -1)
        {
            ///cout << "Settings: Nothing" << endl;
        }
        else if (selected == 0)
        {
            readSettings();
            readSliders(1);
            readNumbers();
            setBothColors();
            if (strcmp(prev, "mm") == 0)
            {
                cout << "Settings: Back -> Main Menu" << endl;
                setBothPages(addMainMenuButtons);
            }
            else if (strcmp(prev, "p") == 0)
            {
                cout << "Settings: Back -> Play" << endl;
                setBothPages(addPlayButtons);
            }
            else if (strcmp(prev, "PvES") == 0)
            {
                cout << "Settings: Back -> PvESettings" << endl;
                setBothPages(addPvESettingsButtons);
            }
            else if (strcmp(prev, "PvPS") == 0)
            {
                cout << "Settings: Back -> PvPSettings" << endl;
                setBothPages(addPvPSettingsButtons);
            }
            else if (strcmp(prev, "EvES") == 0)
            {
                cout << "Settings: Back -> EvESettings" << endl;
                setBothPages(addEvESettingsButtons);
            }
            else if (strcmp(prev, "PvE") == 0)
            {
                cout << "Settings: Back -> PvE" << endl;
                setBothPages(addCommonButtons, "PvE");
            }
            else if (strcmp(prev, "PvP") == 0)
            {
                cout << "Settings: Back -> PvP" << endl;
                setBothPages(addCommonButtons, "PvP");
            }
            else if (strcmp(prev, "EvE") == 0)
            {
                cout << "Settings: Back -> EvE" << endl;
                setBothPages(addCommonButtons, "EvE");
            }
            else if (strcmp(prev, "l") == 0)
            {
                cout << "Settings: Back -> Load" << endl;
                setBothPages(addLoadButtons);
            }
            break;
        }
        else if (selected == 1)
        {
            cout << "Settings: Apply" << endl;
            writeSettings();
            writeSliders(1);
            writeNumbers();
        }
        else if (selected == 2)
        {
            cout << "Settings: Changed The Background Color's Red Nuance" << endl;
        }
        else if (selected == 3)
        {
            cout << "Settings: Changed The Background Color's Green Nuance" << endl;
        }
        else if (selected == 4)
        {
            cout << "Settings: Changed The Background Color's Blue Nuance" << endl;
        }
        else if (selected == 5)
        {
            cout << "Settings: Changed The Outline Color's Red Nuance" << endl;
        }
        else if (selected == 6)
        {
            cout << "Settings: Changed The Outline Color's Green Nuance" << endl;
        }
        else if (selected == 7)
        {
            cout << "Settings: Changed The Outline Color's Blue Nuance" << endl;
        }
        else if (selected == 8)
        {
            cout << "Settings: Language Switched To Romanian" << endl;
        }
        else if (selected == 9)
        {
            cout << "Settings: Language Switched To English" << endl;
        }
        else if (selected == 10)
        {
            cout << "Settings: Made The Outline Color Dependent On The Background Color" << endl;
        }
        else if (selected == 11)
        {
            cout << "Settings: Freed The Outline Color" << endl;
        }
        else if (selected == 12)
        {
            cout << "Settings: Eliminated The Skip Button" << endl;
        }
        else if (selected == 13)
        {
            cout << "Settings: Added The Skip Button" << endl;
        }
        else if (selected == 14)
        {
            cout << "Settings: Can't Modify The Past Anymore" << endl;
        }
        else if (selected == 15)
        {
            cout << "Settings: Can Now Modify The Past" << endl;
        }
        else if (selected == 16)
        {
            cout << "Settings: Games Will Now Start In The Usual Position" << endl;
        }
        else if (selected == 17)
        {
            cout << "Settings: Games Will Start In A Random Position" << endl;
        }
        else if (selected == 18)
        {
            cout << "Settings: A Hard Computer Will Pick The Best Move Everytime" << endl;
        }
        else if (selected == 19)
        {
            cout << "Settings: A Hard Computer Will Pick A Random Neutral Move If There Are No Winning Moves Avaliable" << endl;
        }
        else if (selected == 20)
        {
            cout << "Settings: Unlimited Games" << endl;
        }
        else if (selected == 21)
        {
            cout << "Settings: Added Turn Limit" << endl;
        }
        else if (selected == 22)
        {
            cout << "Settings: Changed The Turn Limit" << endl;
        }
        else if (selected == 23)
        {
            cout << "Settings: Failed To Change The Turn Limit" << endl;
        }
    }
}


void initExitButtons()
{
    initButton(eb[0], 20, 720, 1180, 780, "No, please take me back!", "Nu, du-ma inapoi!");
    initButton(eb[1], 20, 820, 1180, 880, "Yea, get me outta here!", "Da, scoate-ma de aici!");
}

void addExitButtons()
{
    clearviewport();
    if (st[0].v)
    {
        outtextxy(600, 550, "Are you sure you want to");
        outtextxy(600, 650, "quit this wonderful game?");
    }
    else
    {
        outtextxy(600, 550, "Esti sigur ca vrei sa iesi");
        outtextxy(600, 650, "din acest joc minunat?");
    }
    paintButtons(eb, 2);
}

void exit(char prev[])
{
    setBothPages(addExitButtons);
    while (exitCalled == 0)
    {
        selected = selectedInMenus(eb, 2, NULL, NULL, 0, NULL, 0, 0, "e");
        if (selected == -1)
        {
            ///cout << "Exit: Nothing" << endl;
        }
        else if (selected == 0)
        {
            if (strcmp(prev, "mm") == 0)
            {
                cout << "Exit: Back -> Main Menu" << endl;
                setBothPages(addMainMenuButtons);
            }
            else if (strcmp(prev, "p") == 0)
            {
                cout << "Exit: Back -> Play" << endl;
                setBothPages(addPlayButtons);
            }
            else if (strcmp(prev, "PvES") == 0)
            {
                cout << "Exit: Back -> PvESettings" << endl;
                setBothPages(addPvESettingsButtons);
            }
            else if (strcmp(prev, "PvPS") == 0)
            {
                cout << "Exit: Back -> PvPSettings" << endl;
                setBothPages(addPvPSettingsButtons);
            }
            else if (strcmp(prev, "EvES") == 0)
            {
                cout << "Exit: Back -> EvESettings" << endl;
                setBothPages(addEvESettingsButtons);
            }
            else if (strcmp(prev, "PvE") == 0)
            {
                cout << "Exit: Back -> PvE" << endl;
                setBothPages(addCommonButtons, "PvE");
            }
            else if (strcmp(prev, "PvP") == 0)
            {
                cout << "Exit: Back -> PvP" << endl;
                setBothPages(addCommonButtons, "PvP");
            }
            else if (strcmp(prev, "EvE") == 0)
            {
                cout << "Exit: Back -> EvE" << endl;
                setBothPages(addCommonButtons, "EvE");
            }
            else if (strcmp(prev, "l") == 0)
            {
                cout << "Exit: Back -> Load" << endl;
                setBothPages(addLoadButtons);
            }
            break;
        }
        else if (selected == 1)
        {
            cout << "Exit: Confirmed" << endl;
            exitCalled = 1;
        }
    }
}


void initMainMenuButtons()
{
    initButton(mmb[0], 20, 520, 1180, 580, "Play", "Joaca");
    initButton(mmb[1], 20, 620, 1180, 680, "How To", "Cum");
    initButton(mmb[2], 20, 720, 1180, 780, "Settings", "Setari");
    initButton(mmb[3], 20, 820, 1180, 880, "Exit", "Iesire");
}

void addMainMenuButtons()
{
    clearviewport();
    paintButtons(mmb, 4);
}

void mainMenu()
{
    setBothPages(addMainMenuButtons);
    while (exitCalled == 0)
    {
        mainMenuCalled = 0;
        selected = selectedInMenus(mmb, 4, NULL, NULL, 0, NULL, 0, 0, "mm");
        if (selected == -1)
        {
            ///cout << "Main Menu: Nothing" << endl;
        }
        else if (selected == 0)
        {
            cout << "Main Menu: Play" << endl;
            play();
        }
        else if (selected == 1)
        {
            cout << "Main Menu: How To" << endl;
            howTo();
        }
        else if (selected == 2)
        {
            cout << "Main Menu: Settings" << endl;
            settings("mm");
        }
        else if (selected == 3)
        {
            cout << "Main Menu: Exit" << endl;
            exit("mm");
        }
    }
}


void start()
{
    readSaves();
    srand(time(0));
    initCommonButtons();
    initTopButtons();
    initEvESettingsButtons();
    initPvPSettingsButtons();
    initPvESettingsButtons();
    initLoadButtons();
    initPlayButtons();
    initHowToButtons();
    initSettingsButtons();
    initExitButtons();
    initMainMenuButtons();
    initSliders();
    initDToggles();
    initSToggles();
    initNTextBoxes();
    initSTextBoxes();
    initLoadRects();
    initSC();
    initM();
    initES();

    initwindow(1200, 900,"L Game");

    setBothColors();
    settextjustify(1, 1);
    settextstyle(8, 0, 4);

    setactivepage(0);
    setvisualpage(1);

    mainMenu();
    closegraph();
}

int main()
{
    start();
    return 0;
}
