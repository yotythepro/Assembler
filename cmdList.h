/*
// Created by User on 08/03/2023.
*/

#ifndef UNTITLED1_CMDLIST_H
#define UNTITLED1_CMDLIST_H

#define DATA 1
#define REG  2
#define VAR  4

typedef char AcceptedTypes;

typedef struct cmd {
    char *name;
    int opCode;
    int argCount;
    AcceptedTypes argTypes[2];
} Cmd;

extern const Cmd COMMANDS[16];

#endif
