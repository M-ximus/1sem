//! @file main.cpp
//----------------------------------------------------------------------------------------------------------------------
//!
//!
//----------------------------------------------------------------------------------------------------------------------
//! @mainpage
//! - main.cpp
//----------------------------------------------------------------------------------------------------------------------


#include "enc.h"


//----------------------------------------------------------------------------------------------------------------------
//! int main()
//!
//! @param[in] inFile - file with the best Pushkin's poem
//! @param[out] outFile - program create this file with sorted poem
//!
//----------------------------------------------------------------------------------------------------------------------

int main() {
    PRINTF("#Hello, I can rewrite Onegin to russian life encyclopedia\n");
    PRINTF("#The dream of Belinskyi and death for literature teacher\n");


    int inFile = open(way_to_in, O_RDONLY|O_BINARY);

    assert(inFile != -1);

    mystring* addressesStrings = create_array_of_strings_fromFile(inFile);
    assert(addressesStrings != nullptr);
    close(inFile);

    char* buff = addressesStrings[0].String;

    addressesStrings = encyclopediaSorter_from_start(addressesStrings);

    FILE* outFile_Encyclopedia = fopen(way_to_Encyclopedia, "w");
    assert(outFile_Encyclopedia != nullptr);

    printText(outFile_Encyclopedia, addressesStrings);
    fclose(outFile_Encyclopedia);

    addressesStrings = encyclopediaSorter_from_end(addressesStrings);

    FILE* outFile_Rhyme = fopen(way_to_rhyme, "w");
    assert(outFile_Rhyme != nullptr);

    printText(outFile_Rhyme, addressesStrings);
    fclose(outFile_Rhyme);

    destroyer(buff, addressesStrings);

    return 0;
}
