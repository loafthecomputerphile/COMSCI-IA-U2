#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>



//=================================================HASH MAP==========================================================
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


//===================================================================================================================

typedef struct{
	int itemid;
    char name[100];
    int instock;
    float selling_price;
    float cost_price;
} Item;

Item CreateNewItem(int id, char name[], int stock, float selling, float cost){
	
	Item item;
	
	item.itemid = id;
	item.instock = stock;
	item.selling_price = selling;
	item.cost_price = cost; 
	strcpy(item.name, name);
	
	return item;
}

//define the sale data structure
typedef struct{
	int saleid;
	char userid;
	time_t unix_time;
} Sale;

Sale CreateNewSale(int id, User user){
	Sale sale;
	sale.saleid = id;
	sale.userid = user.userid;
	sale.unix_time = time(NULL);
	return sale;
}

 
typedef struct{
	int itemid;
	int quantity;
    float cost;
    int success = 1;
} SoldItem;

SoldItem CreateNewSoldItem(Item item, int quantity){
	
	SoldItem solditem;
	
	
	if (item.instock < 1 || item.instock == NULL){
		solditem.success = 0;
		return solditem;
	}else if (quantity < 1){
		solditem.success = 0;
		return solditem;
	}
	
	solditem.itemid = item.id;
	solditem.quantity = quantity;
	solditem.cost = solditem.cost_price*quantity; 
	solditem.success = 1;
	
	return solditem;
}


//define the item sale data structure
typedef struct{
	int saleid;
	SoldItem items[100000];
	float price;
} SaleItems;

SaleItems CreateNewSaleItems(Sale sale, SoldItem items[], int itemlen){
	int i;
	float total_price = 0;
	SaleItems saleitem;
	saleitem.saleid = sale.saleid;
	memcpy(saleitem.items, items, sizeof(items));
	for(i=0;i<itemlen;i++){
		total_price = total_price+items[i].cost;
	}
	saleitem.price = total_price;
	return saleitem;
	
}

//define the user data structure

typedef struct{
	char uname[16];
	char pword[11];
} AccountInfo;

typedef struct{
	char dayob[3];
	char monthob[3];
	char yearob[5];
} DOB;

typedef struct{
	int userid;
	AccountInfo account;
	char fname[20];
	char lname[20];
	DOB dob;
	char role[20];
} User;


char *slice(char string[], int back, int end){
	int i = 0;
	int size = strlen(string);
	//printf("%s\n", string);
	char *result = malloc(end + 1);
	if (back == 1){
		for(i=end;i>0;i--){
			result[end-i] = string[size-i];
		}
		return result;
	}
	for(i=0;i<end;i++){
		result[i] = string[i];
	}
	return result;
	
}

char* mystrcat( char* dest, char* src )
{
	while (*dest) dest++;
	while (*dest++ = *src++);
	return --dest;
}


char *PrintDOB(DOB dob){
	char date[11];     
	sprintf(date, "%s/%s/%s", dob.dayob, dob.monthob, dob.yearob);
	return date;
}



AccountInfo CreateNewAccount(char uname[], char pword[]){
	
	AccountInfo account;
	
	strcpy(account.uname, uname);
	strcpy(account.pword, pword);
	
	return account;
}


DOB CreateNewDOB(char dayob[], char monthob[], char yearob[]){
	
	DOB dob;
	
	strcpy(dob.dayob, dayob);
	strcpy(dob.monthob, monthob);
	strcpy(dob.yearob, yearob);
	
	return dob;
}


User CreateNewUser(int id, AccountInfo account, char fname[], char lname[], DOB dob, char role[]){
	
	User user;
	user.userid = id;
	user.account = account;
	user.dob = dob; 
	strcpy(user.fname, fname);
	strcpy(user.lname,lname);
	strcpy(user.role,role);
	
	return user;
}



void PrintAllUsers(User users[], int start){
	//printf("%d", length);
	int i = 0;
	printf("+------------+-----------------------+-----------------------+-----------------------+-------------+\n");
	printf("|     User ID|             First Name|              Last Name|               Position|          DOB|\n");
	printf("+------------+-----------------------+-----------------------+-----------------------+-------------+\n");
	for(i=start;i<start+20;i++){
		printf("|%12s|%23s|%23s|%23s|%13s|\n", users[i].userid, users[i].fname, users[i].lname, users[i].role, PrintDOB(users[i].dob));
		printf("+------------+-----------------------+-----------------------+-----------------------+-------------+\n");
	}
}


void PrintAllItems(Item items[], int start){
	//printf("%d", length);
	int i = 0;
	printf("+--------+----------------------------------------------+-----------+----------------+-------------+\n");
	printf("| Item ID|                                     Item Name|   In Stock|   Selling Price|   Cost Price|\n");
	printf("+--------+----------------------------------------------+-----------+----------------+-------------+\n");
	for(i=start;i<start+20;i++){
		printf("|%8d|%46s|%11d|%16f|%13s|\n", items[i].itemid, items[i].name, items[i].instock, items[i].selling_price, items[i].cost_price);
		printf("+------------+-----------------------+-----------------------+-----------------------+-------------+\n");
	}
}



int binarySearch(int array[], int x, int low, int high) {
	while (low <= high) {
		int mid = low + (high - low) / 2;
		if (array[mid] == x)
		  return mid;
		if (array[mid] < x)
		  low = mid + 1;
		else
		  high = mid - 1;
	}
	return -1;
}


int main(int argc, char *argv[]) {
	//User p1 = {"1", "bigman", "pop123", "Drew", "Quashie", "28", "05", "2104", "m"};
	//char data[20];
	DOB dob = CreateNewDOB("28", "05", "2004");
	AccountInfo account = CreateNewAccount("quashman", "pop123");
	User users[2] = {CreateNewUser(account, "Drew", "Quashie", dob, "manager"), CreateNewUser(account, "Drew", "Quashie", dob, "manager")};
	//strcpy(data,  UserId(p1));
	int length = (sizeof(users) / sizeof(users[0]));
	PrintAllUsers(users, length);
	return 0;
}




































