/***********************************************************************
Logging Library for project 1:

Author: Tyler Martin
Date: 9/13/2021

-Manages the logs by adding messages, printing them, writing them, 
 or deleting them as requested. perror is used when a failure of 
 any sort is reached(see below).
***********************************************************************/
#include <time.h>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cerrno>

//Corresponding Header File
#include "LoggerLibrary.h"

//=======================================================================
//Definitions for prototypes given in the class header definition section
//=======================================================================

/***********************************************************************
Function: LoggerLibrary::addmsg(const char, const char*)

Description: Adds a message to the List of messages

***********************************************************************/
int LoggerLibrary::addmsg(const char type, const char* msg){
    
    //Check if char* array is valid
    if (msg == NULL){
        //no message
        errno = 75;

        //get executable name
        std::string exe;
        std::ifstream("/proc/self/comm") >> exe;
        exe.append(": Error");

        //bring in perror
        perror(&exe[0]);
        return -1;
    }
    //Check if type is valid
    if (type != 'e' && type != 'i' && type != 'f' && type != 'w'){
        //no message
        errno = 75;

                //get executable name
        std::string exe;
        std::ifstream("/proc/self/comm") >> exe;
        exe.append(": Error");

        //bring in perror
        perror(&exe[0]);

        //also give additional information:
        std::cout << "\nError Specifics: Message Type is Invalid. \n";
        return -1;
    }

    //get time received
    time_t curtime;
    time(&curtime);

    //convert char array to string via constructor
    std::string msg_string(msg);

    //make an instance of our log to add to out list/stack/array/vector
    struct Log newLog;

    //populate the struct
    newLog.type = type;
    char* message_char = new char[msg_string.length()];
    strcpy(message_char, msg_string.c_str());
    newLog.message = message_char;
    newLog.time = curtime;

    //Add our newly created struct into our list/stack/array/vector
    LoggerLibrary::LogList.push_back(newLog);

    //check if the log was a fatal error 
    if (type == 'f'){
        //notify
        std::cout << "Fatal Message Passed. Writing log to \"savelog\" and exiting.\n";
        //save the log and exit
        //(such a circtuitous route to adding a string char
        //was implemented due to warning being thrown by g++
        //upon conversion from const char* to char*)
        std::string logstring = "savelog";
        char* logname = new char[logstring.length()];
        strcpy(logname, logstring.c_str());
        savelog(logname);
        exit(EXIT_FAILURE);
    }

    return 0;
}

/***********************************************************************
Function: LoggerLibrary::clearlog()

Description: Clears the list of messages and frees the memory associated
             with the message char* array

***********************************************************************/
void LoggerLibrary::clearlog(){

    //Pop all of our logs off our list/stack/array/vector until we are left with none
    for(int i=0; i < LoggerLibrary::LogList.size(); i++){
        //free the memory taken up by the char array and pop the object off the stack
        free(LogList[i].message);
        LoggerLibrary::LogList.pop_back();
    }

}

/***********************************************************************
Function: LoggerLibrary::clearlog()

Description: returns the log of messages after casting it to a char* array

***********************************************************************/
char* LoggerLibrary::getlog(){

    //return a NULL if the log is not valid (empty) and sets errno
    if (LoggerLibrary::LogList.empty()){
        //no message err
        errno = 75;

        //get executable name
        std::string exe;
        std::ifstream("/proc/self/comm") >> exe;
        exe.append(": Error");

        //bring in perror
        perror(&exe[0]);
        return NULL;
    }

    //string to hold our monstrosity of a log
    std::string allLogsConcat="";

    //get each log and hold it as a string 
    for(int i=0; i < LoggerLibrary::LogList.size(); i++){

        //get executable name
        std::string exe;
        std::ifstream("/proc/self/comm") >> exe;
        
        //append the bin name
        exe.append(": ");
        allLogsConcat.append(exe);

        //Setting up the data for simple appending afterward
        std::string messageType;

        //Determine type by switch
        switch (LoggerLibrary::LogList[i].type) {

            //Set appending string to WARNING for 'w' char
            case 'w':
                messageType = "WARNING";
                break;
            //Set appending string to INFO for 'i' char
            case 'i':
                messageType = "INFO";
                break;
            //Set appending string to FATAL for 'f' char
            case 'f':
                messageType = "FATAL";
                break;
            //Set appending string to ERROR for 'e' char
            case 'e':
                messageType = "ERROR";
                break;
            //No need to cause a program abortion for an unknown type, just acknowledge the NULL status
            default:
                messageType = "UNKNOWN";
                break;
        }

        //Append the error type
        allLogsConcat.append(messageType);
        allLogsConcat.append(": ");

        //Append the error message itself
        std::string messageConverted(LoggerLibrary::LogList[i].message);
        allLogsConcat.append(messageConverted);
        allLogsConcat.append(": ");

        //Append the time the error occurred
        allLogsConcat.append(ctime(&LoggerLibrary::LogList[i].time));
        allLogsConcat.append("\n");
    }

    //Once done, return the start of string in a pointer
    return &allLogsConcat[0];
}

/***********************************************************************
Function: LoggerLibrary::savelog (char*){

Description: Save the log to the specified filename

***********************************************************************/
int LoggerLibrary::savelog (char* filename){
    //Check if char* array is valid
    if (filename == NULL){
        //no such file
        errno = 2;

        //get executable name
        std::string exe;
        std::ifstream("/proc/self/comm") >> exe;
        exe.append(": Error");

        //bring in perror
        perror(&exe[0]);
        return -1;
    }

    //get fstream ready
    std::ofstream fout;

    //write data
    //FIXME ADD: HANDLERS FOR ERRS
    fout.open(filename);
    
    //If the file can't be opened for some reaso, return error and close file attempt
    if( !fout ) { 
        //get executable name
        std::string exe;
        std::ifstream("/proc/self/comm") >> exe;
        exe.append(": Error");

        //bring in perror
        perror(&exe[0]);
        return -1;
    }
    //write the data
    else{
        //get the log
        char* c = LoggerLibrary::getlog();

        //write the log
        fout << c;
        fout.close();
        return 0;
    }

}