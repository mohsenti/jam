//
// Created by mohsen on 18/05/16.
//

#include <stdlib.h>
#include "Cursor.h"


int DBA::Cursor::asInteger(unsigned long col) {
    return atoi(getData(col));
}

double DBA::Cursor::asDouble(unsigned long col) {
    return atof(getData(col));
}

string DBA::Cursor::asString(unsigned long col) {
    return getData(col);
}

bool DBA::Cursor::isNull(unsigned long col) {
    return getData(col) == NULL;
}

