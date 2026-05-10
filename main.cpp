#include "chess.h"

int main()
{
    // Enable ANSI colors for Windows
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(h, &mode);
    SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    board b;
    setupboard(b);
    movevalidator v;

    char turn = 'W';
    string msg = "";

    while (true)
    {
        showboard(b);

        if (msg != "")
        {
            cout << "\n  >> " << msg << "\n";
            msg = "";
        }

        // Check game over
        if (!v.haslegal(turn, b))
        {
            if (v.isincheck(turn, b))
            {
                cout << "\n  ========================================\n";
                cout << "        CHECKMATE! ";
                cout << (turn == 'W' ? "Black" : "White") << " WINS!\n";
                cout << "  ========================================\n";
            }
            else
            {
                cout << "\n  ========================================\n";
                cout << "           STALEMATE! DRAW!\n";
                cout << "  ========================================\n";
            }
            break;
        }

        // Check warning
        if (v.isincheck(turn, b))
            cout << "\n  *** " << (turn == 'W' ? "White" : "Black") << " is in CHECK! ***\n";

        cout << "\n  " << (turn == 'W' ? "White" : "Black") << "'s move (e.g., e2 e4): ";

        string from, to;
        cin >> from >> to;

        if (from.length() < 2 || to.length() < 2)
        {
            msg = "Invalid format! Use: e2 e4";
            continue;
        }

        // Convert notation to coordinates
        int fc = from[0] - 'a';
        int fr = 8 - (from[1] - '0');
        int tc = to[0] - 'a';
        int tr = 8 - (to[1] - '0');

        position pf(fr, fc);
        position pt(tr, tc);

        if (!pf.isvalid() || !pt.isvalid())
        {
            msg = "Out of board! Use a1 to h8";
            continue;
        }

        piece* p = b.getpiece(pf);

        if (p == NULL)
        {
            msg = "No piece there!";
            continue;
        }

        if (p->color != turn)
        {
            msg = "Not your piece!";
            continue;
        }

        chessmove m(pf, pt);

        if (!v.islegalmove(m, b))
        {
            msg = "Illegal move!";
            continue;
        }

        v.applymove(m, b);
        turn = (turn == 'W') ? 'B' : 'W';
    }

    cout << "\n  Press any key to exit...";
    system("pause");
    return 0;
}