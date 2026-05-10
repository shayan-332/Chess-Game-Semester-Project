#include "chess.h"

// cursor ko x,y pe le jao
void gotoxy(int x, int y)
{
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// board pe saare pieces starting position pe rakh do
void setupboard(board& b)
{
    // Clear board first
    for (int r = 0; r < SIZE; r++)
        for (int c = 0; c < SIZE; c++)
            b.grid[r][c] = NULL;

    // Black pieces - row 0
    b.grid[0][0] = new rook('B');
    b.grid[0][1] = new knight('B');
    b.grid[0][2] = new bishop('B');
    b.grid[0][3] = new queen('B');
    b.grid[0][4] = new king('B');
    b.grid[0][5] = new bishop('B');
    b.grid[0][6] = new knight('B');
    b.grid[0][7] = new rook('B');

    for (int c = 0; c < 8; c++)
        b.grid[1][c] = new pawn('B');

    // White pieces - row 7
    b.grid[7][0] = new rook('W');
    b.grid[7][1] = new knight('W');
    b.grid[7][2] = new bishop('W');
    b.grid[7][3] = new queen('W');
    b.grid[7][4] = new king('W');
    b.grid[7][5] = new bishop('W');
    b.grid[7][6] = new knight('W');
    b.grid[7][7] = new rook('W');

    for (int c = 0; c < 8; c++)
        b.grid[6][c] = new pawn('W');
}

// colored board display
void showboard(board& b)
{
    system("cls");

    int bx = 2;
    int by = 3;

    gotoxy(8, 0);
    cout << "======= CHESS GAME =======";
    gotoxy(6, 1);
    cout << "White = UPPER  |  Black = lower";

    // Column labels
    gotoxy(bx, by);
    for (int c = 0; c < 8; c++)
        cout << " " << (char)('a' + c) << " ";

    for (int r = 0; r < 8; r++)
    {
        gotoxy(0, by + 1 + r);
        cout << (8 - r);
        gotoxy(bx, by + 1 + r);

        for (int c = 0; c < 8; c++)
        {
            if ((r + c) % 2 == 0)
                cout << "\033[47m\033[30m";
            else
                cout << "\033[40m\033[37m";

            piece* p = b.getpiece(r, c);
            if (p == NULL)
            {
                cout << "   ";
            }
            else
            {
                char sym = p->type;
                if (p->color == 'B')
                    sym = tolower(sym);
                cout << " " << sym << " ";
            }
        }
        cout << "\033[0m";
        gotoxy(bx + 24, by + 1 + r);
        cout << " " << (8 - r);
    }

    gotoxy(bx, by + 9);
    for (int c = 0; c < 8; c++)
        cout << " " << (char)('a' + c) << " ";

    gotoxy(0, by + 11);
}

// ========== POSITION CLASS ==========
position::position()
{
    row = 0;
    col = 0;
}

position::position(int r, int c)
{
    row = r;
    col = c;
}

bool position::isvalid()
{
    return (row >= 0 && row < SIZE && col >= 0 && col < SIZE);
}

bool position::issame(position p)
{
    return (row == p.row && col == p.col);
}

// ========== CHESS MOVE CLASS ==========
chessmove::chessmove()
{
}

chessmove::chessmove(position f, position t)
{
    from = f;
    to = t;
}

// ========== PIECE CLASS ==========
piece::piece(char c, char t)
{
    color = c;
    type = t;
    hasmoved = false;
    movecount = 0;
}

// ========== BOARD CLASS ==========
board::board()
{
    for (int r = 0; r < SIZE; r++)
        for (int c = 0; c < SIZE; c++)
            grid[r][c] = NULL;
}

// board destructor - must be defined after the full definition of piece
board::~board()
{
    for (int r = 0; r < SIZE; r++)
        for (int c = 0; c < SIZE; c++)
            if (grid[r][c] != NULL)
                delete grid[r][c];
}

piece* board::getpiece(int r, int c)
{
    if (r >= 0 && r < SIZE && c >= 0 && c < SIZE)
        return grid[r][c];
    return NULL;
}

piece* board::getpiece(position p)
{
    return getpiece(p.row, p.col);
}

// ========== KNIGHT ==========
knight::knight(char c) : piece(c, 'N') {}

void knight::getpossiblemoves(position pos, board& b)
{
    movecount = 0;
    int dr[] = { -2, -2, -1, -1, 1, 1, 2, 2 };
    int dc[] = { -1, 1, -2, 2, -2, 2, -1, 1 };

    for (int i = 0; i < 8; i++)
    {
        position to(pos.row + dr[i], pos.col + dc[i]);
        if (!to.isvalid()) continue;

        piece* target = b.getpiece(to);
        if (target != NULL && target->color == color) continue;

        moves[movecount] = chessmove(pos, to);
        movecount++;
    }
}

// ========== ROOK ==========
rook::rook(char c) : piece(c, 'R') {}

void rook::getpossiblemoves(position pos, board& b)
{
    movecount = 0;
    int dr[] = { -1, 1, 0, 0 };
    int dc[] = { 0, 0, -1, 1 };

    for (int d = 0; d < 4; d++)
    {
        int r = pos.row + dr[d];
        int c = pos.col + dc[d];

        while (r >= 0 && r < SIZE && c >= 0 && c < SIZE)
        {
            piece* target = b.getpiece(r, c);
            if (target != NULL && target->color == color) break;

            moves[movecount] = chessmove(pos, position(r, c));
            movecount++;

            if (target != NULL && target->color != color) break;

            r += dr[d];
            c += dc[d];
        }
    }
}

// ========== BISHOP ==========
bishop::bishop(char c) : piece(c, 'B') {}

void bishop::getpossiblemoves(position pos, board& b)
{
    movecount = 0;
    int dr[] = { -1, -1, 1, 1 };
    int dc[] = { -1, 1, -1, 1 };

    for (int d = 0; d < 4; d++)
    {
        int r = pos.row + dr[d];
        int c = pos.col + dc[d];

        while (r >= 0 && r < SIZE && c >= 0 && c < SIZE)
        {
            piece* target = b.getpiece(r, c);
            if (target != NULL && target->color == color) break;

            moves[movecount] = chessmove(pos, position(r, c));
            movecount++;

            if (target != NULL && target->color != color) break;

            r += dr[d];
            c += dc[d];
        }
    }
}

// ========== QUEEN ==========
queen::queen(char c) : piece(c, 'Q') {}

void queen::getpossiblemoves(position pos, board& b)
{
    movecount = 0;
    int dr[] = { -1, 1, 0, 0, -1, -1, 1, 1 };
    int dc[] = { 0, 0, -1, 1, -1, 1, -1, 1 };

    for (int d = 0; d < 8; d++)
    {
        int r = pos.row + dr[d];
        int c = pos.col + dc[d];

        while (r >= 0 && r < SIZE && c >= 0 && c < SIZE)
        {
            piece* target = b.getpiece(r, c);
            if (target != NULL && target->color == color) break;

            moves[movecount] = chessmove(pos, position(r, c));
            movecount++;

            if (target != NULL && target->color != color) break;

            r += dr[d];
            c += dc[d];
        }
    }
}

// ========== KING ==========
king::king(char c) : piece(c, 'K') {}

void king::getpossiblemoves(position pos, board& b)
{
    movecount = 0;
    int dr[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int dc[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

    for (int i = 0; i < 8; i++)
    {
        position to(pos.row + dr[i], pos.col + dc[i]);
        if (!to.isvalid()) continue;

        piece* target = b.getpiece(to);
        if (target != NULL && target->color == color) continue;

        moves[movecount] = chessmove(pos, to);
        movecount++;
    }
}

// ========== PAWN ==========
pawn::pawn(char c) : piece(c, 'P') {}

void pawn::getpossiblemoves(position pos, board& b)
{
    movecount = 0;
    int dir = (color == 'W') ? -1 : 1;

    // One step forward
    position onestep(pos.row + dir, pos.col);
    if (onestep.isvalid() && b.getpiece(onestep) == NULL)
    {
        moves[movecount] = chessmove(pos, onestep);
        movecount++;

        // Two steps forward (first move only)
        if (!hasmoved)
        {
            position twostep(pos.row + 2 * dir, pos.col);
            if (b.getpiece(twostep) == NULL)
            {
                moves[movecount] = chessmove(pos, twostep);
                movecount++;
            }
        }
    }

    // Diagonal captures
    position leftcap(pos.row + dir, pos.col - 1);
    if (leftcap.isvalid())
    {
        piece* target = b.getpiece(leftcap);
        if (target != NULL && target->color != color)
        {
            moves[movecount] = chessmove(pos, leftcap);
            movecount++;
        }
    }

    position rightcap(pos.row + dir, pos.col + 1);
    if (rightcap.isvalid())
    {
        piece* target = b.getpiece(rightcap);
        if (target != NULL && target->color != color)
        {
            moves[movecount] = chessmove(pos, rightcap);
            movecount++;
        }
    }
}

// ========== MOVE VALIDATOR ==========
position movevalidator::findking(char color, board& b)
{
    for (int r = 0; r < SIZE; r++)
        for (int c = 0; c < SIZE; c++)
        {
            piece* p = b.getpiece(r, c);
            if (p != NULL && p->type == 'K' && p->color == color)
                return position(r, c);
        }
    return position(0, 0);
}

bool movevalidator::isincheck(char color, board& b)
{
    position kingpos = findking(color, b);

    for (int r = 0; r < SIZE; r++)
        for (int c = 0; c < SIZE; c++)
        {
            piece* p = b.getpiece(r, c);
            if (p == NULL || p->color == color) continue;

            p->getpossiblemoves(position(r, c), b);
            for (int i = 0; i < p->movecount; i++)
                if (p->moves[i].to.issame(kingpos))
                    return true;
        }
    return false;
}

bool movevalidator::issafeaftermove(chessmove m, board& b)
{
    piece* moving = b.grid[m.from.row][m.from.col];
    piece* captured = b.grid[m.to.row][m.to.col];

    // Make temporary move
    b.grid[m.to.row][m.to.col] = moving;
    b.grid[m.from.row][m.from.col] = NULL;

    bool safe = !isincheck(moving->color, b);

    // Undo move
    b.grid[m.from.row][m.from.col] = moving;
    b.grid[m.to.row][m.to.col] = captured;

    return safe;
}

bool movevalidator::islegalmove(chessmove m, board& b)
{
    piece* p = b.getpiece(m.from);
    if (p == NULL) return false;

    p->getpossiblemoves(m.from, b);

    for (int i = 0; i < p->movecount; i++)
        if (p->moves[i].to.issame(m.to))
            return issafeaftermove(m, b);

    return false;
}

void movevalidator::applymove(chessmove m, board& b)
{
    piece* p = b.grid[m.from.row][m.from.col];

    // Capture piece
    if (b.grid[m.to.row][m.to.col] != NULL)
        delete b.grid[m.to.row][m.to.col];

    // Move piece
    b.grid[m.to.row][m.to.col] = p;
    b.grid[m.from.row][m.from.col] = NULL;

    if (p != NULL)
        p->hasmoved = true;

    // Pawn promotion
    if (p != NULL && p->type == 'P')
    {
        if ((p->color == 'W' && m.to.row == 0) || (p->color == 'B' && m.to.row == 7))
        {
            char col = p->color;
            delete b.grid[m.to.row][m.to.col];
            b.grid[m.to.row][m.to.col] = new queen(col);
        }
    }
}

bool movevalidator::haslegal(char color, board& b)
{
    for (int r = 0; r < SIZE; r++)
        for (int c = 0; c < SIZE; c++)
        {
            piece* p = b.getpiece(r, c);
            if (p == NULL || p->color != color) continue;

            p->getpossiblemoves(position(r, c), b);
            for (int i = 0; i < p->movecount; i++)
                if (issafeaftermove(p->moves[i], b))
                    return true;
        }
    return false;
}

bool movevalidator::ischeckmate(char color, board& b)
{
    return isincheck(color, b) && !haslegal(color, b);
}