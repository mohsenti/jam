//
// Created by mohsen on 6/11/16.
//

#ifndef JAM_DATABASEMODELS_H
#define JAM_DATABASEMODELS_H

#include <string>

using namespace std;

typedef struct {
    long Id;
    long parentId;
    long biographyId;
    string name;
} Category, *PCategory;

typedef struct {
    long Id;
    string title;
} Poem, *PPoem;

typedef struct {
    string text;
    int position;
    int vorder;
} Verse, *PVerse;

#endif //JAM_DATABASEMODELS_H
