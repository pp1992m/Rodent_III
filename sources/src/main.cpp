/*
Rodent, a UCI chess playing engine derived from Sungorus 1.4
Copyright (C) 2009-2011 Pablo Vazquez (Sungorus author)
Copyright (C) 2011-2017 Pawel Koziol

Rodent is free software: you can redistribute it and/or modify it under the terms of the GNU
General Public License as published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

Rodent is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.
If not, see <http://www.gnu.org/licenses/>.
*/

#include "rodent.h"
#include "book.h"

cGlobals Glob;
#ifndef USE_THREADS
    cEngine EngineSingle;
#else
    #include <list>
    std::list<cEngine> enginesArray (1);
#endif
cBitBoard BB;
cParam Par;
cMask Mask;
cDistance Dist;
sBook GuideBook;
sBook MainBook;

#ifndef USEGEN
    sInternalBook InternalBook;
#else
    #include "book_gen.h"
#endif


int main() {

    POS p;

    BB.Init();
    InitSearch();
    Init();
    Glob.Init();
    Par.DefaultWeights();
    Par.InitTables();
    Glob.is_tuning = false;

    Mask.Init();
    Dist.Init();

#if defined(_WIN32) || defined(_WIN64)
    // if we are on Windows search for books and settings in same directory as rodentII.exe
    MainBook.SetBookName("books/rodent.bin");
    GuideBook.SetBookName("books/guide.bin");
    ReadPersonality("basic.ini");
#elif __linux || __unix
    // if we are on Linux
    // first check, if compiler got told where books and settings are stored
#ifdef BOOKPATH
    #define MAKESTR(x) #x
    // process Mainbook
    MainBook.SetBookName(MAKESTR(BOOKPATH) "/rodent.bin");   // store it
    // process Guidebook
    GuideBook.SetBookName(MAKESTR(BOOKPATH) "/guide.bin");
    // process Personality file
    ReadPersonality(MAKESTR(BOOKPATH) "/basic.ini");
    #undef MAKESTR
#else // if no path was given than we assume that files are stored at /usr/share/rodentII
    MainBook.SetBookName("/usr/share/rodentII/rodent.bin");
    GuideBook.SetBookName("/usr/share/rodentII/guide.bin");
    ReadPersonality("/usr/share/rodentII/basic.ini");
#endif

#else
    // a platform we have not tested yet. We assume that opening books and
    // settings are stored within the same directory. Similiar to Windows.
    printf("Platform unknown. We assume that opening books and settings are stored within RodentII path");
    MainBook.SetBookName("books/rodent.bin");
    GuideBook.SetBookName("books/guide.bin");
    ReadPersonality("basic.ini");
#endif

    InternalBook.Init(&p);
    UciLoop();

    return 0;
}

void cGlobals::Init() {

    reading_personality = false;
    use_personality_files = false;
    separate_books = false;
    thread_no = 1;
    should_clear = false;
    is_console = true;
    elo_slider = true;
}
