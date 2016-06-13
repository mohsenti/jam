//
// Created by mohsen on 6/11/16.
//

#include <QDebug>
#include <QtCore/QString>
#include <sstream>
#include "DatabaseIO.h"
#include "DatabaseModels.h"

static DatabaseIO *databaseIO = NULL;

void DatabaseIO::open() {
    dbo = new SQLiteProvider("./ganjoor.s3db");
    dbo->connect();
}

void DatabaseIO::close() {
    dbo->disConnect();
    delete dbo;
}


vector<PCategory> *DatabaseIO::getCategories(int parentId) {
    vector<PCategory> *aVector = new vector<PCategory>();
    Cursor *cursor = NULL;
    stringstream query;
    query << "SELECT * FROM cat WHERE parent_id = " << parentId;
    if (dbo->query(query.str(), cursor)) {
        if (cursor == NULL)
            return aVector;
        if (cursor->first()) {
            do {
                PCategory category = new Category;
                category->name = cursor->asString(2);
                category->Id = cursor->asInteger(0);
                category->parentId = cursor->asInteger(3);
                category->biographyId = cursor->asInteger(1);
                aVector->push_back(category);
            } while (cursor->next());
        }
        delete cursor;
    } else {
        qDebug() << QString::fromStdString(dbo->getError());
    }
    return aVector;
}

vector<PPoem> *DatabaseIO::getPoems(int categoryId) {
    vector<PPoem> *aVector = new vector<PPoem>();
    Cursor *cursor = NULL;
    stringstream query;
    query << "SELECT * FROM poem WHERE cat_id = " << categoryId;
    if (dbo->query(query.str(), cursor)) {
        if (cursor == NULL)
            return aVector;
        if (cursor->first()) {
            do {
                PPoem poem = new Poem;
                poem->Id = cursor->asInteger(0);
                poem->title = cursor->asString(2);
                aVector->push_back(poem);
            } while (cursor->next());
        }
        delete cursor;
    } else {
        qDebug() << QString::fromStdString(dbo->getError());
    }
    return aVector;
}

vector<PVerse> *DatabaseIO::getVerses(int poemId) {
    vector<PVerse> *aVector = new vector<PVerse>();
    return aVector;
}

string DatabaseIO::getBiography(int biographyId) {
    return "";
}

DatabaseIO *DatabaseIO::getInstance() {
    if (databaseIO == NULL) {
        databaseIO = new DatabaseIO();
        databaseIO->open();
    }
    return databaseIO;
}
