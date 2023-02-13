#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 100000
#define MAX_KEY_LENGTH 110

struct DataItem {
    int value;
    char key[MAX_KEY_LENGTH];
};

struct DataItem* hashArray[SIZE];
struct DataItem* dummyItem;
struct DataItem* item;

int hashCode(char* key);

struct DataItem* search(char* key);

void insert(char* key, int value);

struct DataItem* delete(struct DataItem* item);

void display();

void writeToFile(char* filename);

void readFromFile(char* filename);

#endif



