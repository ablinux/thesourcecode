#include "dataBase.h"
#include <Sd.h>
#include "sysCfg.h"

#define TRFILE dataBase
#include "TraceServer.h"
#define DATA_BASE
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
        TRACE(ERROR,DATA_BASE,"Dir for Db files does not exist, creating...");//Serial.println("Dir for Db files does not exist, creating...");
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
            TRACE(INFO,DATA_BASE,"Openning current table... ");//Serial.print("Openning current table... ");
            EDB_Status result = db.open(0);
            if (result == EDB_OK)
            {
                TRACE(INFO,DATA_BASE,"FIle opened")//Serial.println("DONE");
            }
            else
            {
                TRACE(ERROR,DATA_BASE,"Error in file opening Cannot find database file");//Serial.println("ERROR");
                Serial.println("Did not find database in the file " + String(DATA_BASE_FILE_NAME));
                TRACE(DEBUG,DATA_BASE,"Creating new table... ");//Serial.print("Creating new table... ");
                db.create(0, TABLE_SIZE, (unsigned int)datalogSize);
                TRACE(DEBUG,DATA_BASE,"File crated.. ");//Serial.println("DONE");
                return false;
            }
        }
        else
        {
            TRACE(ERROR,DATA_BASE,"Error in file opening Cannot find database file");//Serial.println("Could not open file " + String(DATA_BASE_FILE_NAME));
            return false;
        }
    }
    else
    {
        TRACE(INFO,DATA_BASE,"Creating table... ");//Serial.print("Creating table... ");
        // create table at with starting address 0
        dbFile = SD.open(DATA_BASE_FILE_NAME, FILE_WRITE);
        db.create(0, TABLE_SIZE, (unsigned int)sizeof(DATA_BASE_FILE_NAME));
        TRACE(DEBUG,DATA_BASE,"Table created... ");//Serial.println("DONE");
    }
}