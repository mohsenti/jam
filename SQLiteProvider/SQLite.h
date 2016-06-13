//
// Created by mohsen on 19/05/16.
//

#ifndef DBA_SQLITE_H
#define DBA_SQLITE_H

#include <vector>
#include "sqlite3.h"
#include "Provider.h"

namespace DBA {
    namespace SQLite {

        class SQLiteCursor : public Cursor {

        public:
            SQLiteCursor(int fieldCount, char **fields);

            virtual ~SQLiteCursor();

            virtual FieldType getFieldType(unsigned long col) override;

            virtual string fieldName(unsigned long col) override;

            virtual int indexOfField(string fieldName) override;

            virtual const char *getData(unsigned long col) override;

            virtual bool next() override;

            virtual bool prev() override;

            virtual bool first() override;

            virtual bool last() override;

            virtual unsigned long fieldCount() override;

            virtual unsigned long recordCount() override;

        private:
            void AddRow(char **&row);

            vector<char *> fields;
            vector<char **> rows;
            unsigned long currentRow;

            friend class SQLiteProvider;
        };

        class SQLiteProvider : public Provider {
        public:

            virtual string quotedNames(string name) override;

            virtual string quotedString(string str) override;

            virtual string escapeString(string str) override;

            virtual vector<string> listOfDatabases() override;

            virtual vector<string> listOfTables() override;

            virtual bool tableExists(string tableName) override;

            SQLiteProvider(const string &fileName) : fileName(fileName) { }

            virtual string getClientVersion() override;

            virtual bool connect() override;

            virtual void disConnect() override;

            virtual bool query(string query, Cursor *&cursor) override;

            virtual string getError() override;

            virtual int getErrorNumber() override;

            const string &getFileName() const {
                return fileName;
            }

            void setFileName(const string &fileName) {
                this->fileName = fileName;
            }

        private:
            static int fetchRows(void *cursor, int argc, char **argv, char **azColName);

            string fileName;
            sqlite3 *db;
        };

    }
}
#endif //DBA_SQLITE_H
