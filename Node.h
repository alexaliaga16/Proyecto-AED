#include <string>

struct Node
{

    int row;
    char col;
    std::string value;
    Node *nextInRow;
    Node *nextInCol;

    Node()
    {
        row = 1;
        col = 'A';
        value = "";
        nextInRow = NULL;
        nextInCol = NULL;
    }

    Node(int r, char c, std::string v)
    {
        row = r;
        col = c;
        value = v;
        nextInRow = NULL;
        nextInCol = NULL;
    }
};
