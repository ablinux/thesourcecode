#include "dataBase.h"
#include <Sd.h>
#include "sysCfg.h"

static File dbFile;

// The read and write handlers for using the SD Library
// Also blinks the led while writing/reading
void writer(unsigned long address, byte data)
{
    dbFile.seek(address);
    dbFile.write(data);
    dbFile.flush();
}

byte reader(unsigned long address)
{
    dbFile.seek(address);
    byte b = dbFile.read();
    return b;
}

// Create an EDB object with the appropriate write and read handlers
EDB db(&writer, &reader);

bool dataBaseInit(uint16_t datalogSize)
{
    // Check dir for db files
    if (!SD.exists("/DB"))
    {
        Serial.println("Dir for Db files does not exist, creating...");
        SD.mkdir("/DB");
    }
    if (SD.exists(DATA_BASE_FILE_NAME))
    {

        dbFile = SD.open(DATA_BASE_FILE_NAME, FILE_WRITE);

        // Sometimes it wont open at first attempt, espessialy after cold start
        // Let's try one more time
        if (!dbFile)
        {
            dbFile = SD.open(DATA_BASE_FILE_NAME, FILE_WRITE);
        }

        if (dbFile)
        {
            Serial.print("Openning current table... ");
            EDB_Status result = db.open(0);
            if (result == EDB_OK)
            {
                Serial.println("DONE");
            }
            else
            {
                Serial.println("ERROR");
                Serial.println("Did not find database in the file " + String(DATA_BASE_FILE_NAME));
                Serial.print("Creating new table... ");
                db.create(0, TABLE_SIZE, (unsigned int)datalogSize);
                Serial.println("DONE");
                return false;
            }
        }
        else
        {
            Serial.println("Could not open file " + String(DATA_BASE_FILE_NAME));
            return false;
        }
    }
    else
    {
        Serial.print("Creating table... ");
        // create table at with starting address 0
        dbFile = SD.open(DATA_BASE_FILE_NAME, FILE_WRITE);
        db.create(0, TABLE_SIZE, (unsigned int)sizeof(DATA_BASE_FILE_NAME));
        Serial.println("DONE");
    }
}