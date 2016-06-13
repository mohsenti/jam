/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Provider.h
 * Author: mohsen
 *
 * Created on 16 May 2016, 18:55
 */

#ifndef PROVIDER_H
#define PROVIDER_H

#include <string>
#include <vector>

#include "Cursor.h"

using namespace std;

namespace DBA {

    class Provider {
    public:
        Provider();

        virtual ~Provider();

        virtual string getClientVersion() = 0;

        virtual bool connect() = 0;

        virtual void disConnect() = 0;

        virtual bool query(string query, Cursor *&cursor) = 0;

        virtual string getError() = 0;

        virtual int getErrorNumber() = 0;

        virtual vector<string> listOfDatabases() = 0;

        virtual vector<string> listOfTables() = 0;

        virtual bool tableExists(string tableName) = 0;

        virtual string quotedString(string str) = 0;

        virtual string escapeString(string str) = 0;

        virtual string quotedNames(string name) = 0;
    protected:

    private:

    };

}

#endif /* PROVIDER_H */

