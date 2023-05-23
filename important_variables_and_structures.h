#ifndef important_variables_and_structures
#define important_variables_and_structures

///settings will have   6 (2 colors x 3 nuances) background and outline color bytes
///                     6 (2 colors x 3 nuances) player color bytes
///                     24 (2 names x 12 chars) name bytes
///                     8 (1 unsigned long long x 4 bytes) move limit bytes
///                     2 skill bits
///                     7 settings bits


char number[10], message[100], path[50];
int selected, outlineColor, backgroundColor;
unsigned long long turnLimit, turnLimitCopy;
int ax[4] = {0, 1, 0, -1};
int ay[4] = {-1, 0, 1, 0};
///char pattern[8] = {-121, -61, -31, -16, 120, 60, 30, 15};    ///diagonal
///char pattern[8] = {-52, -52, 51, 51, -52, -52, 51, 51};      ///2 x 2 squares alternating colors
char pattern[8] = {-86, 85, -86, 85, -86, 85, -86, 85};         ///1 x 1 squares alternating colors
bool exitCalled;                    ///1 - user pressed the exit confirmation button
bool mainMenuCalled;                ///1 - the main menu button is pressed
bool turnEnded;                     ///1 - the turn has ended
bool renamingSave;                  ///1 - renaming a save
bool saving;                        ///1 - save has been pressed

char game[3][4][4];
bool turn;                          ///0 when it's red's turn to move, 1 when it's blue's

struct BOARD
{
    char g[4][4];
    BOARD *prev, *next;
};

struct STACK
{
    int l;
    BOARD *first, *last;
}gp;    ///the game progression from the start all the way to the end
BOARD *cgp; ///current game progression (what's shown on the screen)

struct SAVE
{
    char name[33];  ///12 chars for the save name, 2 - " (", 17 - copy number, 1 - ")", 1 - NULL
    char type[4];
    bool turnLimitActive;
    unsigned long long turnLimitCopy;
    bool turn;
    SAVE *prev, *next;
};

struct LIST
{
    int l;
    SAVE *first, *last;
}ll;        ///basically a list of save file names

struct LOADRECT
{
    int x1, x2, y1, y2;
}llr[7];

SAVE *llpos;    ///current position in ll
SAVE *llfs;     ///first save present on the screen
int llrpos;     ///position relative to the screen

struct BUTTON
{
    int x1, y1, x2, y2;
    char eng[100];
    char rom[100];
}top[4], mmb[4], eb[2], sb[2], htb[2], pb[4], lb[3], EvESb[1], PvPSb[1], PvESb[1];
BUTTON cb[4];   ///common buttons shared between the non-mainmenu screens: skip, save, prev, next

struct SLIDER
{
    int x1, y1, x2, y2; ///the slider will occupy the space between (x1, y1) and (x2 * 256, y2)
    unsigned char n;
}bgol[6], p12[6];

struct TOGGLE
{
    BUTTON B[2];
    bool v;
}d[2];          ///computer difficulty, 0 - easy, 1 - hard
                ///     [0] - AI used exclusively for EvE
                ///     [1] - AI used in PvE and EvE
TOGGLE st[7];   ///settings toggles, 0 - off, 1 - on or whatev
                ///     [0] - language, 0 - romanian, 1 - english
                ///     [1] - outline color dependency to the background color, 0 - ol is the negative of bg
                ///     [2] - mandatory circle moves, 0 - no skip button, 1 - skip button
                ///     [3] - can modify boards that're previous to the current game board, 0 - nope, 1 - yea
                ///     [4] - basic or randomized starts, 0 - basic, 1 - randomized
                ///     [5] - more lenient hard computer, if it doesn't have a winning move but has neutral moves
                ///                         0 - will pick a neutral move that leaves the opponent with the least number of moves
                ///                         1 - will pick a neutral move at random
                ///     [6] - sudden death, 0 - unlimited games, 1 - turn limit

struct TEXTBOX
{
    int x1, y1, x2, y2;
    char text[33];
    char engp[20];  ///placeholder
    char romp[20];  ///same
}n[2];          ///player names
                ///     [0] - used in PvP and PvE
                ///     [1] - used only in PvP
TEXTBOX stb[1]; ///settings textboxes, should only have numbers in them
                ///     [0] - turn limit
TEXTBOX ltb[1]; ///load textbox, used for naming and renaming files, help me god


struct COORDINATES
{
    int x, y;
};
COORDINATES sc[4][4];   ///the screen coordinates of every square/cell of the 4 x 4 board
COORDINATES t[4];   ///trail made by one of the players when it moves its piece
int tl; ///the length of the trail
COORDINATES m[16][3];   ///the 16 possible ways to form a piece starting from a point
COORDINATES ec[8];  ///all the empty cells on the board, last 2 positions're used in initRandomStart
COORDINATES np[2];  ///all the neutral pieces on the board

struct MOVE
{
    int x, y, m;
};   ///the best piece the ai can move, usually coupled with a moved neutral piece
MOVE pm[48];    ///all the potential moves the current AI has to take into account
int pml;    ///how many potential moves the current AI has to take into account

char es[232][4][4]; ///the 15 x 8 scenarios where red loses and 14 x 8 in which red loses in at most 4 moves against a perfect ai

char wnmp[672][4][4];    ///24 x 28 place to dump similar scenarios (either a player is losing in 1 2 3 or 4 moves in all of them or it has the same number of moves avaliable)
int wnmpl;   ///length of the random move picker vector
int endingScenario;   ///if checking for ending scenarios, will be the specific ending scenario compatible with the current game or the moves required to get there
char lmp[672][4][4]; ///picker for the losing moves, as a last resort
int lmpl;









void initGP();
SAVE *llAdd(char[], char[], bool, unsigned long long, bool);
void addGPToStack();
void convToChar(unsigned long long);
int getPM(bool);
void makeMessage(char []);
void addSettingsButtons(char []);
void addLoadButtons();
void addCommonButtons(char []);
#endif
