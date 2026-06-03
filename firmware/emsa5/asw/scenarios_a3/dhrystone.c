/**
  * Based on famous Dhrystone benchmark.
  *
  * (c) 1984-1988 Reinhold P. Weicker and Rick Richardson
  *
  * See also: https://en.wikipedia.org/wiki/Dhrystone
  * License: not explicitly defined.
  */

#include "defs.h"
#include "dhrystone.h"

int counter;

int strcmp_x(char *s, char *t)
{
    for ( ; *s == *t; s++, t++ , counter++)
        if ( *s == '\0' )
            return( 0 ) ;
    return( *s - *t ) ;
}

void strcpy_x (char s[], char t[])
{
    int i = 0 ;
    while (( s[i] = t[i] ) != '\0'){
        counter++;
        i++ ;}
}

void memcpy_x (char * d, char * s, int l)
{
    while (l--) *d++ = *s++;

    counter ++;
}

typedef enum    {Ident1, Ident2, Ident3, Ident4, Ident5} Enumeration;

typedef int     OneToThirty;
typedef int     OneToFifty;
typedef char    CapitalLetter;
typedef char    String30[31];
typedef int     Array1Dim[51];
typedef int     Array2Dim[51][51];

typedef int             boolean;

struct  Record
{
    struct Record           *PtrComp;
    Enumeration             Discr;
    Enumeration             EnumComp;
    OneToFifty              IntComp;
    String30                StringComp;
};

typedef struct Record   RecordType;
typedef RecordType *    RecordPtr;

static RecordType feld[5];
static int zz=0;

RecordPtr malloc_x (void)
{
    zz+=1;
    counter+=101;
    return(&feld[zz-1]);
}

#define LOOPS   1

void Proc0 (void);
void Proc1 (RecordPtr);
void Proc2 (OneToFifty *);
void Proc3 (RecordPtr *);
void Proc4 (void);
void Proc5 (void);
void Proc6 (Enumeration, Enumeration *);
void Proc7 (OneToFifty, OneToFifty, OneToFifty *);
void Proc8 (Array1Dim, Array2Dim, OneToFifty, OneToFifty);
Enumeration Func1 (CapitalLetter, CapitalLetter);
boolean Func2 (String30, String30);
boolean Func3 (Enumeration);

int dhry(void)
{
    counter=7;
    zz = 0;
    Proc0();
    return counter;
}

int             IntGlob;
boolean         BoolGlob;
char            Char1Glob;
char            Char2Glob;
Array1Dim       Array1Glob;
Array2Dim       Array2Glob;
RecordPtr       PtrGlb;
RecordPtr       PtrGlbNext;

void Proc0 (void)
{
    OneToFifty              IntLoc1;
    OneToFifty          IntLoc2;
    OneToFifty              IntLoc3;
    char                CharIndex;
    Enumeration             EnumLoc;
    String30                String1Loc;
    String30                String2Loc;

    unsigned int    i;

    PtrGlbNext = (RecordPtr) malloc_x();
    PtrGlb = (RecordPtr) malloc_x();
    PtrGlb->PtrComp = PtrGlbNext;
    PtrGlb->Discr = Ident1;
    PtrGlb->EnumComp = Ident3;
    PtrGlb->IntComp = 40;
    strcpy_x(PtrGlb->StringComp, "DHRYSTONE PROGRAM, SOME STRING");
    strcpy_x(String1Loc, "DHRYSTONE PROGRAM, 1'ST STRING");
    Array2Glob[8][7] = 10;

    for (i = 0; i < LOOPS; ++i)
    {

        counter+=3;
        Proc5();
        Proc4();
        IntLoc1 = 2;
        IntLoc2 = 3;
        strcpy_x(String2Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
        EnumLoc = Ident2;
        BoolGlob = ! Func2(String1Loc, String2Loc);
        while (IntLoc1 < IntLoc2)
        {
            IntLoc3 = 5 * IntLoc1 - IntLoc2;
            Proc7(IntLoc1, IntLoc2, &IntLoc3);
            counter+=9;
            ++IntLoc1;
        }
        Proc8(Array1Glob, Array2Glob, IntLoc1, IntLoc3);
        Proc1(PtrGlb);
        for (CharIndex = 'A'; CharIndex <= Char2Glob; ++CharIndex,++counter)
            if (EnumLoc == Func1(CharIndex, 'C'))
                Proc6(Ident1, &EnumLoc);
        IntLoc3 = IntLoc2 * IntLoc1;
        IntLoc2 = IntLoc3 / IntLoc1;
        IntLoc2 = 7 * (IntLoc3 - IntLoc2) - IntLoc1;
        Proc2(&IntLoc1);
        counter+=11;
    }
}

void Proc1 (RecordPtr PtrParIn)
{
#define NextRecord      (*(PtrParIn->PtrComp))
#define structassign(d, s)      memcpy_x((void *)&(d), (void *)&(s), sizeof(d))

    structassign(NextRecord, *PtrGlb);
    PtrParIn->IntComp = 5;
    NextRecord.IntComp = PtrParIn->IntComp;
    NextRecord.PtrComp = PtrParIn->PtrComp;
    Proc3(&(NextRecord.PtrComp));
    if (NextRecord.Discr == Ident1)
    {
        NextRecord.IntComp = 6;
        Proc6(PtrParIn->EnumComp, &NextRecord.EnumComp);
        NextRecord.PtrComp = PtrGlb->PtrComp;
        counter+=27;
        Proc7(NextRecord.IntComp, 10, &NextRecord.IntComp);
    }
    else
        structassign(*PtrParIn, NextRecord);

    counter+=23;

#undef structassign
#undef NextRecord
}

void Proc2 (OneToFifty * IntParIO)
{
    OneToFifty          IntLoc;
    Enumeration         EnumLoc;

    IntLoc = *IntParIO + 10;
    for(;;)
    {
        if (Char1Glob == 'A')
        {
            --IntLoc;
            *IntParIO = IntLoc - IntGlob;
            counter+=13;
            EnumLoc = Ident1;
        }
        counter+=17;
        if (EnumLoc == Ident1)
            break;
    }
}

void Proc3 (RecordPtr * PtrParOut)
{
    if (PtrGlb != NULL){
        counter+=31;
        *PtrParOut = PtrGlb->PtrComp;
    }
    else{
        counter+=35;
        IntGlob = 100;
    }
    Proc7(10, IntGlob, &PtrGlb->IntComp);
}

void Proc4 (void)
{
    boolean     BoolLoc;

    BoolLoc = Char1Glob == 'A';
    BoolLoc |= BoolGlob;
    counter+=37;
    Char2Glob = 'B';
}

void Proc5 (void)
{
    Char1Glob = 'A';
    counter+=41;
    BoolGlob = FALSE;
}

void Proc6 (Enumeration EnumParIn, Enumeration * EnumParOut)
{
    *EnumParOut = EnumParIn;
    if (! Func3(EnumParIn) ){
        *EnumParOut = Ident4;
        counter+=43;
    }
    switch (EnumParIn)
    {
        case Ident1:    counter+=5; *EnumParOut = Ident1; break;
        case Ident2:    counter+=6; if (IntGlob > 100) *EnumParOut = Ident1;
                        else *EnumParOut = Ident4;
                        break;
        case Ident3:    counter+=7; *EnumParOut = Ident2; break;
        case Ident4:    counter+=8; break;
        case Ident5:    counter+=9; *EnumParOut = Ident3;
    }
}

void Proc7 (OneToFifty IntParI1, OneToFifty IntParI2, OneToFifty * IntParOut)
{
    OneToFifty  IntLoc;

    IntLoc = IntParI1 + 2;
    counter+=IntLoc;
    *IntParOut = IntParI2 + IntLoc;
}

void Proc8 (Array1Dim Array1Par, Array2Dim Array2Par, OneToFifty IntParI1, OneToFifty IntParI2)
{
    OneToFifty  IntLoc;
    OneToFifty  IntIndex;

    IntLoc = IntParI1 + 5;
    Array1Par[IntLoc] = IntParI2;
    Array1Par[IntLoc+1] = Array1Par[IntLoc];
    Array1Par[IntLoc+30] = IntLoc;
    for (IntIndex = IntLoc; IntIndex <= (IntLoc+1); ++IntIndex){
        counter+=IntIndex;
        Array2Par[IntLoc][IntIndex] = IntLoc;
    }
    ++Array2Par[IntLoc][IntLoc-1];
    Array2Par[IntLoc+20][IntLoc] = Array1Par[IntLoc];
    IntGlob = 5;
}

Enumeration Func1 (CapitalLetter CharPar1, CapitalLetter CharPar2)
{
    CapitalLetter       CharLoc1;
    CapitalLetter       CharLoc2;

    CharLoc1 = CharPar1;
    CharLoc2 = CharLoc1;
    if (CharLoc2 != CharPar2){
        counter+=91;
        return Ident1;
    }
    else {
        counter+=93;
        return Ident2;
    }
}

boolean Func2 (String30 StrParI1, String30 StrParI2)
{
    OneToThirty         IntLoc;
    CapitalLetter       CharLoc;

    IntLoc = 1;
    while (IntLoc <= 1)
        if (Func1(StrParI1[IntLoc], StrParI2[IntLoc+1]) == Ident1)
        {
            CharLoc = 'A';
            counter+=IntLoc;
            ++IntLoc;
        }
    if (CharLoc >= 'W' && CharLoc <= 'Z')
        IntLoc = 7;
    if (CharLoc == 'X'){
        counter+=41;
        return TRUE;
    }
    else
    {
        if (strcmp_x(StrParI1, StrParI2) > 0)
        {
            IntLoc += 7;
            counter+=47;
            return TRUE;
        }
        else
            return FALSE;
    }
}

boolean Func3 (Enumeration EnumParIn)
{
    Enumeration EnumLoc;

    EnumLoc = EnumParIn;
    if (EnumLoc == Ident3) {return TRUE; counter++;}
    return FALSE;
}


