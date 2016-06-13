//
// Created by mohsen on 6/11/16.
//

#ifndef JAM_DATABASEIO_H
#define JAM_DATABASEIO_H


#include <SQLiteProvider/SQLite.h>
#include "DatabaseModels.h"

using namespace DBA;
using namespace SQLite;

class DatabaseIO {
public:
    void open();

    void close();

    vector<PCategory> *getCategories(int parentId = 0);

    vector<PPoem> *getPoems(int categoryId);

    vector<PVerse> *getVerses(int poemId);

    string getBiography(int biographyId);

    static DatabaseIO *getInstance();

private:
    SQLiteProvider *dbo;
};


#endif //JAM_DATABASEIO_H
