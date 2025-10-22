#include "BrailleResolver.h"

#include <iostream>
#include <chrono>

char CurrentLetter = '0';

bool ProcessKeyDown(WPARAM key)
{
    return key == CurrentLetter;
}

void GenerateItem(BrailleGrid& GridOne, BrailleGrid& GridTwo)
{
    BrailleDot*& One = GridOne.One;
    BrailleDot*& Two = GridOne.Two;
    BrailleDot*& Three = GridOne.Three;
    BrailleDot*& Four = GridOne.Four;
    BrailleDot*& Five = GridOne.Five;
    BrailleDot*& Six = GridOne.Six;

    srand(static_cast<unsigned int>(time(nullptr)));

    Retry:
    char Letter = static_cast<char>(65 + (rand() % 26));
    char Numeric = static_cast<char>(48 + (rand() % 10));

    char This = '\0';
    int Prob = static_cast<int>(rand() % 5);
    if (Prob)
        This = Letter;
    else
        This = Numeric;

    if (This == CurrentLetter)
        goto Retry;
    else
        CurrentLetter = This;

    FillKey(CurrentLetter, GridOne, GridTwo);
}
void FillKey(char Key, BrailleGrid& GridOne, BrailleGrid& GridTwo)
{
    GridOne.One->State(false);
    GridOne.Two->State(false);
    GridOne.Three->State(false);
    GridOne.Four->State(false);
    GridOne.Five->State(false);
    GridOne.Six->State(false);

    GridTwo.One->State(false);
    GridTwo.Two->State(false);
    GridTwo.Three->State(false);
    GridTwo.Four->State(false);
    GridTwo.Five->State(false);
    GridTwo.Six->State(false);

    bool Numeric = Key >= 0x30 && Key <= 0x39;
    if (Numeric)
    {
        GridOne.One->State(false);
        GridOne.Two->State(false);
        GridOne.Three->State(true);
        GridOne.Four->State(true);
        GridOne.Five->State(true);
        GridOne.Six->State(true);
    }
    else
    {
        GridOne.One->State(false);
        GridOne.Two->State(false);
        GridOne.Three->State(false);
        GridOne.Four->State(false);
        GridOne.Five->State(false);
        GridOne.Six->State(true);
    }

    BrailleDot*& One = GridTwo.One;
    BrailleDot*& Two = GridTwo.Two;
    BrailleDot*& Three = GridTwo.Three;
    BrailleDot*& Four = GridTwo.Four;
    BrailleDot*& Five = GridTwo.Five;
    BrailleDot*& Six = GridTwo.Six;

    switch (Key)
    {
    case '1':
    case 'A':
        One->State(true);
        Two->State(false);
        Three->State(false);
        Four->State(false);
        Five->State(false);
        Six->State(false);
        break;
    case '2':
    case 'B':
        One->State(true);
        Two->State(true);
        Three->State(false);
        Four->State(false);
        Five->State(false);
        Six->State(false);
        break;
    case '3':
    case 'C':
        One->State(true);
        Two->State(false);
        Three->State(false);
        Four->State(true);
        Five->State(false);
        Six->State(false);
        break;
    case '4':
    case 'D':
        One->State(true);
        Two->State(false);
        Three->State(false);
        Four->State(true);
        Five->State(true);
        Six->State(false);
        break;
    case '5':
    case 'E':
        One->State(true);
        Two->State(false);
        Three->State(false);
        Four->State(false);
        Five->State(true);
        Six->State(false);
        break;
    case '6':
    case 'F':
        One->State(true);
        Two->State(true);
        Three->State(false);
        Four->State(true);
        Five->State(false);
        Six->State(false);
        break;
    case '7':
    case 'G':
        One->State(true);
        Two->State(true);
        Three->State(false);
        Four->State(true);
        Five->State(true);
        Six->State(false);
        break;
    case '8':
    case 'H':
        One->State(true);
        Two->State(true);
        Three->State(false);
        Four->State(false);
        Five->State(true);
        Six->State(false);
        break;
    case '9':
    case 'I':
        One->State(false);
        Two->State(true);
        Three->State(false);
        Four->State(true);
        Five->State(false);
        Six->State(false);
        break;
    case '0':
    case 'J':
        One->State(false);
        Two->State(true);
        Three->State(false);
        Four->State(true);
        Five->State(true);
        Six->State(false);
        break;
    case 'K':
        One->State(true);
        Two->State(false);
        Three->State(true);
        Four->State(false);
        Five->State(false);
        Six->State(false);
        break;
    case 'L':
        One->State(true);
        Two->State(true);
        Three->State(true);
        Four->State(false);
        Five->State(false);
        Six->State(false);
        break;
    case 'M':
        One->State(true);
        Two->State(false);
        Three->State(true);
        Four->State(true);
        Five->State(false);
        Six->State(false);
        break;
    case 'N':
        One->State(true);
        Two->State(false);
        Three->State(true);
        Four->State(true);
        Five->State(true);
        Six->State(false);
        break;
    case 'O':
        One->State(true);
        Two->State(false);
        Three->State(true);
        Four->State(false);
        Five->State(true);
        Six->State(false);
        break;
    case 'P':
        One->State(true);
        Two->State(true);
        Three->State(true);
        Four->State(true);
        Five->State(false);
        Six->State(false);
        break;
    case 'Q':
        One->State(true);
        Two->State(true);
        Three->State(true);
        Four->State(true);
        Five->State(true);
        Six->State(false);
        break;
    case 'R':
        One->State(true);
        Two->State(true);
        Three->State(true);
        Four->State(false);
        Five->State(true);
        Six->State(false);
        break;
    case 'S':
        One->State(false);
        Two->State(true);
        Three->State(true);
        Four->State(true);
        Five->State(false);
        Six->State(false);
        break;
    case 'T':
        One->State(false);
        Two->State(true);
        Three->State(true);
        Four->State(true);
        Five->State(true);
        Six->State(false);
        break;
    case 'U':
        One->State(true);
        Two->State(false);
        Three->State(true);
        Four->State(false);
        Five->State(false);
        Six->State(true);
        break;
    case 'V':
        One->State(true);
        Two->State(true);
        Three->State(true);
        Four->State(false);
        Five->State(false);
        Six->State(true);
        break;
    case 'W':
        One->State(false);
        Two->State(true);
        Three->State(false);
        Four->State(true);
        Five->State(true);
        Six->State(true);
        break;
    case 'X':
        One->State(true);
        Two->State(false);
        Three->State(true);
        Four->State(true);
        Five->State(false);
        Six->State(true);
        break;
    case 'Y':
        One->State(true);
        Two->State(false);
        Three->State(true);
        Four->State(true);
        Five->State(true);
        Six->State(true);
        break;
    case 'Z':
        One->State(true);
        Two->State(false);
        Three->State(true);
        Four->State(false);
        Five->State(true);
        Six->State(true);
        break;
    }
}
