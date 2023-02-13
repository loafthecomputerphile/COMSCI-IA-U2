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

int hashCode(char* key) {
    int sum = 0;
    int i;
    for (i = 0; i < strlen(key); i++) {
        sum += key[i];
    }
    return sum % SIZE;
}

struct DataItem* search(char* key) {
    int hashIndex = hashCode(key);
    while (hashArray[hashIndex] != NULL) {
        if (strcmp(hashArray[hashIndex]->key, key) == 0) {
            return hashArray[hashIndex];
        }
        hashIndex = (hashIndex + 1) % SIZE;
    }
    return NULL;
}

void insert(char* key, int value) {
    struct DataItem* item = (struct DataItem*)malloc(sizeof(struct DataItem));
    strcpy(item->key, key);
    item->value = value;
    int hashIndex = hashCode(key);
    while (hashArray[hashIndex] != NULL && strcmp(hashArray[hashIndex]->key, key) != 0) {
        hashIndex = (hashIndex + 1) % SIZE;
    }
    hashArray[hashIndex] = item;
}

struct DataItem* delete(struct DataItem* item) {
    char* key = item->key;
    int hashIndex = hashCode(key);
    while (hashArray[hashIndex] != NULL) {
        if (strcmp(hashArray[hashIndex]->key, key) == 0) {
            struct DataItem* temp = hashArray[hashIndex];
            hashArray[hashIndex] = dummyItem;
            return temp;
        }
        hashIndex = (hashIndex + 1) % SIZE;
    }
    return NULL;
}

void display() {
    int i;
    for (i = 0; i < SIZE; i++) {
        if (hashArray[i] != NULL) {
            printf(" (%s,%d)", hashArray[i]->key, hashArray[i]->value);
        }
    }
    printf("\n");
}

void writeToFile(char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    int i;
    for (i = 0; i < SIZE; i++) {
        if (hashArray[i] != NULL) {
            fwrite(hashArray[i], sizeof(struct DataItem), 1, fp);
        }
    }
    fclose(fp);
}

void readFromFile(char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    int i;
    struct DataItem temp;
    while (fread(&temp, sizeof(struct DataItem), 1, fp) == 1) {
        insert(temp.key, temp.value);
    }
    fclose(fp);
}

int main() {
    /*dummyItem = (struct DataItem*)malloc(sizeof(struct DataItem));
    dummyItem->value = -1;
    strcpy(dummyItem->key, "");

    insert("Apple", 10);
    insert("Banana", 20);
    insert("Mango", 30);

    display();

    item = search("Banana");
    if (item != NULL) {
        printf("Element found: %s, %d\n", item->key, item->value);
    } else {
        printf("Element not found\n");
    }

    writeToFile("hash_map.bin");
    memset(hashArray, 0, sizeof(hashArray));*/

    //readFromFile("hash_map.bin");
    //insert("sheesh", 3);
    //insert("sheesh2", 4);
    //writeToFile("hash_map.bin");
    //display();

    return 0;
}








