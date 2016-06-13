//
// Created by mohsen on 19/05/16.
//

#include <iostream>
#include <cstring>
#include "SQLite.h"

DBA::SQLite::SQLiteCursor::SQLiteCursor(int fieldCount, char **fields) {
    for (int i = 0; i < fieldCount; i++)
        this->fields.push_back(fields[i]);
}

DBA::SQLite::SQLiteCursor::~SQLiteCursor() {
    vector<char **>::const_iterator it = rows.begin();
    for (int i = 0; it != rows.end(); it++, i++) {
        char **row = *it;
        for (int j = 0; j < fieldCount(); j++) {
            if (row[j] != NULL)
                delete[] row[j];
        }
        delete[] row;
    }
    rows.clear();
    fields.clear();
}

DBA::FieldType DBA::SQLite::SQLiteCursor::getFieldType(unsigned long col) {
    return ftString;
}

string DBA::SQLite::SQLiteCursor::fieldName(unsigned long col) {
    return fields.at(col);
}

int DBA::SQLite::SQLiteCursor::indexOfField(string fieldName) {
    vector<char *>::const_iterator it = fields.begin();
    for (int i = 0; it < fields.end(); i++, it++) {
        if (strcmp((*it), fieldName.c_str()) == 0)
            return i;
    }
    return -1;
}

const char *DBA::SQLite::SQLiteCursor::getData(unsigned long col) {
    char **row = rows.at(currentRow - 1);
    return row[col];
}

bool DBA::SQLite::SQLiteCursor::next() {
    if (currentRow < recordCount()) {
        currentRow++;
        return true;
    }
    return false;
}

bool DBA::SQLite::SQLiteCursor::prev() {
    if (currentRow > 1) {
        currentRow--;
        return true;
    }
    return false;
}

bool DBA::SQLite::SQLiteCursor::first() {
    if (recordCount() == 0)
        return false;
    currentRow = 1;
    return currentRow > 0;
}

bool DBA::SQLite::SQLiteCursor::last() {
    currentRow = recordCount();
    return currentRow > 0;
}

unsigned long DBA::SQLite::SQLiteCursor::fieldCount() {
    return fields.size();
}

unsigned long DBA::SQLite::SQLiteCursor::recordCount() {
    return rows.size();
}

void DBA::SQLite::SQLiteCursor::AddRow(char **&row) {
    rows.push_back(row);
}

string DBA::SQLite::SQLiteProvider::getClientVersion() {
    return sqlite3_version;
}

bool DBA::SQLite::SQLiteProvider::connect() {
    return sqlite3_open(fileName.c_str(), &db) == 0;
}

void DBA::SQLite::SQLiteProvider::disConnect() {
    sqlite3_close(db);
}

bool DBA::SQLite::SQLiteProvider::query(string query, DBA::Cursor *&cursor) {
    char *errorMsg = 0;
    if (cursor != NULL)
        delete cursor;
    if (sqlite3_exec(db, query.c_str(), fetchRows, &cursor, &errorMsg) != SQLITE_OK) {
        sqlite3_free(errorMsg);
        return false;
    }
    sqlite3_db_release_memory(db);
    return true;
}

string DBA::SQLite::SQLiteProvider::getError() {
    return sqlite3_errmsg(db);
}

int DBA::SQLite::SQLiteProvider::getErrorNumber() {
    return sqlite3_errcode(db);
}

int DBA::SQLite::SQLiteProvider::fetchRows(void *cursor, int argc, char **argv, char **azColName) {
    Cursor **c = (Cursor **) cursor;
    if (*c == NULL) {
        char **fields;
        fields = new char *[argc];
        for (int i = 0; i < argc; ++i) {
            fields[i] = new char[strlen(azColName[i]) + 1];
            strcpy(fields[i], azColName[i]);
        }
        *c = new SQLiteCursor(argc, fields);
        delete[] fields;
    }
    char **row = new char *[argc];
    for (int i = 0; i < argc; ++i) {
        if (argv[i] == NULL) {
            row[i] = NULL;
            break;
        }
        row[i] = new char[strlen(argv[i])+1];
        row[i][strlen(argv[i])]=0;
        memcpy(row[i], argv[i], sizeof(char) * (strlen(argv[i])));
    }
    ((SQLiteCursor *) *c)->AddRow(row);
    return 0;
}

bool DBA::SQLite::SQLiteProvider::tableExists(string tableName) {
    //TODO:need more test
    Cursor *cursor = NULL;
    if (query("SELECT name FROM sqlite_master WHERE type='table' AND name='" + tableName + "';", cursor)) {
        if (cursor == NULL)
            return false;
        delete cursor;
        return true;
    }
    return false;
}

vector<string> DBA::SQLite::SQLiteProvider::listOfDatabases() {
    vector<string> databases;
    Cursor *cursor=NULL;
    if (query("PRAGMA database_list;",cursor)){
        if (cursor!=NULL) {
            cursor->first();
            int nameIndex=cursor->indexOfField("name");
            do {
                databases.push_back(cursor->asString(nameIndex));
            } while(cursor->next());
            delete cursor;
        }
    }
    return databases;
}

vector<string> DBA::SQLite::SQLiteProvider::listOfTables() {
    vector<string> tables;
    Cursor *cursor=NULL;
    if (query("SELECT name FROM sqlite_master WHERE type='table';",cursor)){
        if (cursor!=NULL) {
            cursor->first();
            int nameIndex=cursor->indexOfField("name");
            do {
                tables.push_back(cursor->asString(nameIndex));
            } while(cursor->next());
            delete cursor;
        }
    }
    return tables;
}


string DBA::SQLite::SQLiteProvider::quotedString(string str) {
    return "'" + str + "'";
}

string DBA::SQLite::SQLiteProvider::escapeString(string str) {
    return sqlite3_mprintf("%q",str.c_str());;
}

string DBA::SQLite::SQLiteProvider::quotedNames(string name) {
    return "\"" + name +"\"";
}






