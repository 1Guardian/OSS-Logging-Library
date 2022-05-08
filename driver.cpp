/***********************************************************************
Driver code for project 1:

Author: Tyler Martin
Date: 9/13/2021

-Generates text randomly for messages from a pool of 4 lorem-ipsum
strings and attaches a 'type' to them (w,f,i,e).

Testing steps:
Makes a logger instance, then adds messages, then prints messages, 
then writes messages, then clears the log and exits.
***********************************************************************/
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <chrono>
#include <thread>
#include "LoggerLibrary.h"

/***********************************************************************
Function: printHelp()

Description: Prints the Help message triggered by the use of the -h
flag and aborts execution when printing is complete.

***********************************************************************/
void printHelp(){
    std::cout << "Arbitrary Commandline messenger system: " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Execution Method: ./driver <command switches> " << std::endl;
    std::cout << "Switch Options:" << std::endl;
    std::cout << "[-h] :- Displays the 'help' message" << std::endl;
    std::cout << "[-t x] :- Will send a message every 'x' second on average" << std::endl;
    std::cout << "[logfilename] :- Will append the output to the designated logfile" << std::endl;
    exit(EXIT_SUCCESS);
}

/***********************************************************************
Function: setFlag(int, int*, int*, std::string*)

Description: sets the flags for the other commandline switches.
            If -t is passed, a message reaffirming the time is sent, 
            and the 'second' variable is updated. 
            If a filename is passed instead, the flag is altered to read 2, and 
            the 'outputFile' variable is updated.
            All variables are updated via pointers.

***********************************************************************/
void setFlag(int choiceSwitch, int* flag, int* second, std::string* outputFile, char* suppliedfile){
    if (choiceSwitch == 1){
        std::cout << "Time has been set to " << optarg << " second " << std::endl;
        *second = atoi(optarg);
        *flag = 1;
    }
    else {
        std::string file_name(suppliedfile);
        *outputFile = file_name;
        std::cout << "Output file set to: " << *outputFile << std::endl;
        *flag = 2;
    }
}

/***********************************************************************
Function: sendMessage(int, int, int, std::string[], char[], LoggerLibrary)

Description: Push Lorem-ipsum messages to it on an interval of either every 
             second (no -t time), or every n (-t n second) basis. 
             Will push 6 messages and successfully log them, or it will reach 
             a fatal (1/4 chance), and it will exit with the fatal protocol.

***********************************************************************/
void sendMessage(int second, int currentCalls, int maximumCalls, std::string LoremIpsum[], char typeOfMessage[], LoggerLibrary* Log){
    if (currentCalls < maximumCalls){
        //wait designated amount of time
        std::this_thread::sleep_for(std::chrono::seconds(second));

        //get the random message and random type
        int z=rand()%4;
        //lower chances of FATAL message terminating the test program
        if(z == 2){z=rand()%4;}
        char *cstr = new char[LoremIpsum[z].length() + 1];
        strcpy(cstr, LoremIpsum[z].c_str());
        Log->addmsg(typeOfMessage[z], cstr);
        delete [] cstr;

        //call self again with an increment to the counter
        sendMessage(second, currentCalls+1, maximumCalls, LoremIpsum, typeOfMessage, Log);
    }
}

int main(int argc, char *argv[])
{
    //variables
    srand (time(NULL));
    int flag=0, opt, second=1, randomTime;
    std::string outputFile = "messages.log";
    std::string LoremIpsum[4] = {"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.", "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.", "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.", "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."};
    char typeOfMessage[4] = {'i', 'w', 'f', 'e'};
    int optcounter=0;
    
    //FIXME: NEED TO HANDLE THE OUTPUT FILE OPTION W/O THE NEED FOR THE -O FLAG
    while ((opt = getopt(argc, argv, ":t:h")) != -1) {
        switch (opt) {

        //If the given flag is -h, give a help message
        case 'h':
            printHelp();
            break;
        
        // if the flag -t is given, log the average time
        case 't':
            optcounter = 1;
            setFlag(1, &flag, &second, &outputFile, argv[1]);
            break;

        // an unknown argument is supplied
        case '?':
            std::cout << "Invalid option supplied. Terminating." << std::endl;
            exit(EXIT_FAILURE);
            break;
        }

    }

    //If we make it past the getopt check, then it's either empty
    //or we have a message log name to use
    if (optcounter == 0){
        if(argc > 1){
            setFlag(2, &flag, &second, &outputFile, argv[argc-1]);
        }
    }
    //Checking if an opt was supplied and a name was supplied
    if (optcounter == 1){
        if(argc > 3){
            setFlag(2, &flag, &second, &outputFile, argv[argc-1]);
        }
    }

    // Testing by creating instance of library
    LoggerLibrary Log;

    //sending messages to the log using the designated time if provided
    sendMessage(second, 0, 5, LoremIpsum, typeOfMessage, &Log);

    //Get log and print it
    char* getlogandprint = Log.getlog();
    printf("Getting Log to Display: \n%s", getlogandprint);

    //Write the message 
    Log.savelog(&outputFile[0]);

    //Test Clearing it
    Log.clearlog();


   exit(EXIT_SUCCESS);
}