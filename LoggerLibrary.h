#ifndef LOGGERLIBRARY_H
#define LOGGERLIBRARY_H

/***********************************************************************
Header File for the Logger Library:

Author: Tyler Martin
Date: 9/13/2021

-implemented as a class so that in the future I can spawn
 instances of the logging system for various parts of my 
 OS. a vector was also added to use as the queue system for 
 the logs.
***********************************************************************/

#include <time.h>
#include <vector>

class LoggerLibrary
{
    //Struct to store data
    private:
        struct Log{
            time_t time;     // Time stampchar
            char type; //message type : Warning/Fatal/Information/Error
            char* message; // Message 'string'
        };

        //Queue Bound to a class so that this library is easier to allow
        //multiple pipeless logging sessions for the future.
        std::vector<LoggerLibrary::Log> LogList;

    public:
        //Adds a message to the List of messages
        int addmsg (const char type, const char*msg);

        //Clears the list of messages
        void clearlog();

        //Print the log of messages after casting it to a 'string'
        char* getlog();

        //Save the log to the specified filename
        int savelog (char* filename);
};

#endif