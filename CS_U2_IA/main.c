#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define USERFILE "UserDataBase.bin"
#define IDFILE "IdDataBase.bin"
//=================================================HASH MAP==========================================================

typedef struct{
	int itemid;
    char name[100];
    int instock;
    float selling_price;
    float cost_price;
} Item;


//define the sale data structure
typedef struct{
	int saleid;
	char userid;
	time_t unix_time;
} Sale;



 
typedef struct{
	int itemid;
	int quantity;
    float cost;
    int success;
} SoldItem;



typedef struct{
	int saleid;
	int itemid;
	int userid;
} MaxIDs;

//define the item sale data structure
typedef struct{
	int saleid;
	SoldItem items[100000];
	float price;
} SaleItems;



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
	int deleted ;
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


Sale CreateNewSale(int id, User user){
	Sale sale;
	sale.saleid = id;
	sale.userid = user.userid;
	sale.unix_time = time(NULL);
	return sale;
}


SoldItem CreateNewSoldItem(Item item, int quantity){
	
	SoldItem solditem;
	
	
	if (item.instock < 1 || item.instock == NULL){
		solditem.success = 0;
		return solditem;
	}else if (quantity < 1){
		solditem.success = 0;
		return solditem;
	}
	
	solditem.itemid = item.itemid;
	solditem.quantity = quantity;
	solditem.cost = item.cost_price*quantity; 
	solditem.success = 1;
	
	return solditem;
}


AccountInfo CreateNewAccount(char uname[], char pword[]){
	
	AccountInfo account;
	
	strcpy(account.uname, uname);
	strcpy(account.pword, pword);
	
	return account;
}


Item CreateNewItem(int id, char name[], int stock, float selling, float cost){
	
	Item item;
	
	item.itemid = id;
	item.instock = stock;
	item.selling_price = selling;
	item.cost_price = cost; 
	strcpy(item.name, name);
	
	return item;
}



DOB CreateNewDOB(char dayob[], char monthob[], char yearob[]){
	
	DOB dob;
	
	strcpy(dob.dayob, dayob);
	strcpy(dob.monthob, monthob);
	strcpy(dob.yearob, yearob);
	
	return dob;
}


User CreateNewUser(int id, AccountInfo account, char fname[20], char lname[20], DOB dob, char role[20]){
	
	User user;
	user.userid = id;
	user.account = account;
	user.dob = dob; 
	user.deleted = 0;
	strcpy(user.fname, fname);
	strcpy(user.lname,lname);
	strcpy(user.role,role);
	
	return user;
}


int IsNewFile(char filename[30]){
	//open file and store it in the  file pointer
	FILE *file = fopen(filename, "a+");
	
    if(file == NULL){
    	//if file is empty return 0
    	return 0;
	}
	//lookthrough the file to the end
    fseek(file, 0, SEEK_END);
    //store the size of the file in size
    int size = ftell(file);
    //close the file
    fclose(file);
	//return the size of the file
    return size;
}



int AddUserData(User users[], int length){
	MaxIDs id;
	int isemptyID = IsNewFile(IDFILE);
	FILE *userdata, *iddata;
	int i = 0;
	int userid = 0;
	if(isemptyID != 0){
		iddata = fopen(IDFILE, "r");
		while(fread(&id, sizeof( MaxIDs), 1, iddata)){
			//loop through all structures in ineventory file
			userid = id.userid;
			
		}
		fclose(iddata);
	
	}else{
		userid = -1;
		userdata = fopen(USERFILE, "wb");
		for(i=0;i<length;i++){
			userid++;
			users[i].userid = userid;
			fwrite(&users[i], sizeof(User), 1, userdata);
		}
		fclose(userdata);
		
		iddata = fopen(IDFILE, "wb");
		id.userid = userid;
		fwrite(&id, sizeof( MaxIDs), 1, iddata);
		fclose(iddata);
		return;
	}
	
	userdata = fopen(USERFILE, "ab");
	for(i=userid;i<userid+length;i++){
		userid++;
		users[i].userid = userid;
		fwrite(&users[i], sizeof(User), 1, userdata);
	}
	fclose(userdata);
	iddata = fopen(IDFILE, "wb");
	id.userid = userid;
	fwrite(&id, sizeof( MaxIDs), 1, iddata);
	fclose(iddata);
	
}


int SeachForUser(){
	return 0;
}

int SaveUsers(User user[]){
	return 0;
}



void PrintAllUsers(int start, int end){
	//printf("%d", length);
	FILE *userdata;
	User users[1000];
	User user;
	int count = 0;
	if (IsNewFile(USERFILE) == 0){
		return;
	}
	userdata = fopen(USERFILE, "r");
	
	while(fread(&user, sizeof( User), 1, userdata)){
		//loop through all structures in ineventory file
		users[count] = user;
		count++;
	}
	
	fclose(userdata);
	if(count < end){
		end = count;
	}
	int i = 0;
	printf("+------------+-----------------------+-----------------------+-----------------------+-------------+\n");
	printf("|     User ID|             First Name|              Last Name|               Position|          DOB|\n");
	printf("+------------+-----------------------+-----------------------+-----------------------+-------------+\n");
	for(i=start;i<end;i++){
		if (users[i].deleted == 0){
			printf("|%12d|%23s|%23s|%23s|%13s|\n", users[i].userid, users[i].fname, users[i].lname, users[i].role, PrintDOB(users[i].dob));
			printf("+------------+-----------------------+-----------------------+-----------------------+-------------+\n");
		}
		
	}
	return;
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
	return;
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
	User users[2] = {CreateNewUser(0, account, "Drew", "Quashie", dob, "manager"), CreateNewUser(0, account, "Drew", "Quashie", dob, "manager")};
	//strcpy(data,  UserId(p1));
	//AddUserData(users, 2);
	int length = (sizeof(users) / sizeof(users[0]));
	//printf("length %d", length);
	PrintAllUsers(0,4);
	
	char idfile[20] = "IdTracker.bin";
	//int size = isNewFile(idfile);
	char userfile[20] = "UserDataBase.bin";
	//AddUserData(users, 2, size, userfile,idfile);
	
	
	return 0;
}


int login(char username[20], char password[20]){
	
}



































