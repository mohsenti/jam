//
// Created by mohsen on 18/05/16.
//

#ifndef DBA_CURSOR_H
#define DBA_CURSOR_H

#include <string>

using namespace std;

namespace DBA {

    typedef enum {
        ftInteger,
        ftString,
        ftDouble
    } FieldType;

    class Cursor {
    public:

        Cursor() { }

        virtual ~Cursor() { }

        int asInteger(unsigned long col);

        double asDouble(unsigned long col);

        string asString(unsigned long col);

        bool isNull(unsigned long col);

        virtual FieldType getFieldType(unsigned long col) = 0;

        virtual string fieldName(unsigned long col) = 0;

        virtual int indexOfField(string fieldName) = 0;

        virtual const char *getData(unsigned long col) = 0;

        virtual bool next() = 0;

        virtual bool prev() = 0;

        virtual bool first() = 0;

        virtual bool last() = 0;

        virtual unsigned long fieldCount() = 0;

        virtual unsigned long recordCount() = 0;

    };

}

#endif //DBA_CURSOR_H
