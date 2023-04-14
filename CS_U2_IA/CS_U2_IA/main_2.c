#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <conio.h>

#define USERFILE "UserDataBase.bin"
#define ITEMFILE "ItemDataBase.bin"
#define IDFILE "IdDataBase.bin"
#define SALELOGFILE "SaleLogDataBase.bin"
#define SALEFILE "SaleDataBase.bin"
//=================================================HASH MAP==========================================================

typedef struct{
	int itemid;
    char name[100];
    int instock;
    float selling_price;
    float cost_price;
    int deleted;
} Item;


typedef struct{
	Item data[10000];
} ItemArray;


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
	int userid;
	SoldItem items[10000];
	int length;
	float price;
	time_t unix_time;
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

SaleItems CreateNewSaleItems(int saleid, SoldItem items[], int userid){
	int i;
	float total_price = 0;
	SaleItems saleitem;
	saleitem.saleid = saleid;
	saleitem.userid = userid;
	saleitem.length = sizeof(items)/sizeof(items[0]);
	memcpy(saleitem.items, items, sizeof(items));
	for(i=0;i<saleitem.length;i++){
		total_price = total_price+items[i].cost;
	}
	saleitem.unix_time = time(NULL);
	saleitem.price = total_price;
	return saleitem;
	
}


Sale CreateNewSale(SaleItems sales){
	Sale sale;
	sale.saleid = sales.saleid;
	sale.userid = sales.userid;
	sale.unix_time = sales.unix_time;
	return sale;
}


SoldItem CreateNewSoldItem(Item item, int quantity){
	
	SoldItem solditem;
	
	
	if (item.instock < 1 ){
		solditem.success = 0;
		return solditem;
	}else if (quantity < 1){
		solditem.success = 0;
		return solditem;
	}
	
	solditem.itemid = item.itemid;
	solditem.quantity = quantity;
	solditem.cost = item.selling_price*quantity; 
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
	item.deleted = 0;
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



typedef struct {
	int id[10000];
	int length;
} USERSEARCHLNAME;


USERSEARCHLNAME UserSearchLname(User users[10000], char lname[20], int count){
	int i;
	int id[10000];
	int length = 0;
	for(i=0;i<=count;i++){
		if(strcmp(users[i].lname, lname) == 0){
			id[length] = i;
			//printf("%d \n", id[i]);
			length++;
		}
	}
	USERSEARCHLNAME data;
	memcpy(data.id, id, 10000);
	data.length = length;
	return data;
}



int SearchForUser(char lname[20]){
	printf("hello\n");
	if(IsNewFile(USERFILE) == 0){
		printf("User database does not exist or is empty");
		return 0;
	}
	FILE *userdata;
	User users[10000];
	User user;
	int count = -1;
	
	userdata = fopen(USERFILE, "r");
	
	while(fread(&user, sizeof( User), 1, userdata)){
		//loop through all structures in ineventory file
		if(user.deleted == 0){
			count++;
			users[count] = user;
		}
		
	}
	fclose(userdata);
	
	USERSEARCHLNAME UserLname = UserSearchLname(users, lname, count);
	
	PrintUsers(users, UserLname.id, UserLname.length);
	
	
	return 0;
}

int SaveUsers(User user[]){
	return 0;
}


void PrintUsers(User users[10000], int id[10000], int length){
	int i;
	int index;
	printf("+------------+-----------------------+-----------------------+-----------------------+-------------+\n");
	printf("|     User ID|             First Name|              Last Name|               Position|          DOB|\n");
	printf("+------------+-----------------------+-----------------------+-----------------------+-------------+\n");
	for(i=0;i<length;i++){
		index = id[i];
		printf("|%12d|%23s|%23s|%23s|%13s|\n", users[index].userid, users[index].fname, users[index].lname, users[index].role, PrintDOB(users[index].dob));
		printf("+------------+-----------------------+-----------------------+-----------------------+-------------+\n");
	}
}



void PrintAllUsers(int start, int end){
	//printf("%d", length);
	FILE *userdata;
	User users[10000];
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
		printf("%d\n", end);
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
}

void PrintItems(Item items[10000], int start, int end){
	int i = 0;
	printf("+--------+----------------------------------------------+-----------+----------------+-------------+\n");
	printf("| Item ID|                                     Item Name|   In Stock|   Selling Price|   Cost Price|\n");
	printf("+--------+----------------------------------------------+-----------+----------------+-------------+\n");
	for(i=start;i<end;i++){
		if (items[i].deleted == 0){
			printf("|%8d|%46s|%11d|%16.2f|%13.2f|\n", items[i].itemid, items[i].name, items[i].instock, items[i].selling_price, items[i].cost_price);
			printf("+--------+----------------------------------------------+-----------+----------------+-------------+\n");
		}
	}
}


int PrintAllItems(Item items[], int start, int end){
	//printf("%d", length);
	FILE *itemdata;
	//ItemArray data;
	
	Item item;
	int count = 0;
	if (IsNewFile(ITEMFILE) == 0){
		return count;
	}
	itemdata = fopen(ITEMFILE, "r");
	
	while(fread(&item, sizeof( Item), 1, itemdata)){
		//loop through all structures in ineventory file
		
		if (item.deleted == 0){
			items[count] = item;
			count++;
		}
	}
	fclose(itemdata);
	if(count < end){
		end = count;
		
	}
	int i = 0;
	printf("+--------+----------------------------------------------+-----------+----------------+-------------+\n");
	printf("| Item ID|                                     Item Name|   In Stock|   Selling Price|   Cost Price|\n");
	printf("+--------+----------------------------------------------+-----------+----------------+-------------+\n");
	for(i=start;i<end;i++){
		printf("|%8d|%46s|%11d|%16.2f|%13.2f|\n", items[i].itemid, items[i].name, items[i].instock, items[i].selling_price, items[i].cost_price);
		printf("+--------+----------------------------------------------+-----------+----------------+-------------+\n");
	
	}
	//memcpy(data.items, items, 10000);
	//return data;
	return count;
}

Sale * PrintAllSaleLog(int start, int end){
	//printf("%d", length);
	FILE *saledata;
	Sale sales[1000000000];
	Sale sale;
	
	int count = 0;
	if (IsNewFile(SALELOGFILE) == 0){
		printf("The Sales Log is Empty. Please make a sale before trying to print the Logs\n");
		printf("Press enter to continue...\n");
		getchar();
		return sales;
	}
	
	saledata = fopen(SALELOGFILE, "r");
	while(fread(&sale, sizeof(Sale), 1, saledata)){
		//loop through all structures in ineventory file
		sales[count] = sale;
		count++;
	}
	fclose(saledata);
	
	if(count < end){
		end = count;
	}
	
	int i = 0;
	printf("+---------+---------+---------------+\n");
	printf("|  Sale ID|  User ID|   Time of Sale|\n");
	printf("+---------+---------+---------------+\n");
	for(i=start;i<end;i++){
		printf("|%9d|%9d|%15d|\n", sales[i].saleid, sales[i].userid, sales[i].unix_time);
		printf("+---------+---------+---------------+\n");
	}
	//memcpy(data.items, items, 10000);
	//return data;
	return sales;
}

SaleItems * PrintSaleItems(int saleid, int start, int end){
	//printf("%d", length);
	FILE *saledata, *itemdata;
	SoldItem sales[10000];
	Item items[10000];
	Item item;
	SaleItems sale;
	
	int count = 0;
	if (IsNewFile(SALEFILE) == 0){
		printf("The Sales Log is Empty. Please make a sale before trying to print the Logs\n");
		printf("Press enter to continue...\n");
		getchar();
		return sales;
	}
	
	
	
	saledata = fopen(SALEFILE, "r");
	while(fread(&sale, sizeof(SaleItems), 1, saledata)){
		//loop through all structures in ineventory file
		if(sale.saleid == saleid){
			memcpy(sales, sale.items, 10000);
			itemdata = fopen(ITEMFILE, "r");
			while(fread(&item, sizeof(Item), 1, itemdata)){
				items[count] = item;
				count++;
			}
			fclose(itemdata);
			break;
		}
	}
	fclose(saledata);
	
	
	/*
	if(count < end){
		end = count;
	}
	
	int i = 0;
	printf("+---------+---------+---------------+\n");
	printf("|  Sale ID|  User ID|   Time of Sale|\n");
	printf("+---------+---------+---------------+\n");
	for(i=start;i<end;i++){
		printf("|%9d|%9d|%15d|\n", sales[i].saleid, sales[i].userid, sales[i].unix_time);
		printf("+---------+---------+---------------+\n");
	}
	//memcpy(data.items, items, 10000);
	//return data;
	*/
	return sales;
}

int LinearSearch(){
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

void MakeSale(int userid){
	char input[] = "";
	int found = 0;
	Item item;
	int saleid;
	int idx = 0;
	SoldItem sold[10000];
	FILE *saledata, *iddata, *itemdata, *iddata2;
	int count = 0;
	MaxIDs id;
	char ids[7];
	int quantity;
	int valid = 0;
	int i = 0;
	Item items[10000];
	int length = PrintAllItems(items, 0,10);
	//Item items[10000];
	
	//memcpy(items, &item1, 10000);
	
	int page = 1;
	int max_pages = ceil((float)length/10);
	printf("+--------------+\n");
	printf("|page: %3d/%-3d|\n", page, max_pages);
	printf("+--------------+\n");
	iddata = fopen(IDFILE, "rb+");
	while(fread(&id, sizeof( MaxIDs), 1, iddata)){
		//loop through all structures in ineventory file
		if(id.itemid > -1){
			valid = 1;
			break;
		}
	}
	fclose(iddata);
	if (valid == 1){
	
		printf("If you want to move to the next page enter [D]\nif you want to move back enter [A]\nif you want to finish the sale [X]\nif you want to select the item enter the index number or type the item name\nEnter input here: ");
		scanf(" %[^\n]%*c", input);
		printf("");
		while(1){
			if (strcmp(input, "D") == 0){
				if (page < max_pages){
					page++;
					system("cls");
					PrintAllItems(items,10*(page-1),10*page);
					printf("+--------------+\n");
					printf("|page: %3d/%-3d|\n", page, max_pages);
					printf("+--------------+\n");
				}else{
					system("cls");
					PrintAllItems(items,10*(page-1),10*page);
					printf("+--------------+\n");
					printf("|page: %3d/%-3d|\n", page, max_pages);
					printf("+--------------+\n");
				}
				printf("If you want to move to the next page enter [D]\nif you want to move back enter [A]\nif you want to finish the sale [X]\nif you want to select the item enter the index number or type the item name\nEnter input here: ");
				scanf(" %[^\n]%*c", input);
				printf("");
			}else if (strcmp(input, "A") == 0){
				if (page > 1){
					page--;
					system("cls");
					PrintAllItems(items,10*(page-1),10*page);
					printf("+--------------+\n");
					printf("|page: %3d/%-3d|\n", page, max_pages);
					printf("+--------------+\n");
				}else{
					system("cls");
					PrintAllItems(items,10*(page-1),10*page);
					printf("+--------------+\n");
					printf("|page: %3d/%-3d|\n", page, max_pages);
					printf("+--------------+\n");  
				}
				printf("If you want to move to the next page enter [D]\nif you want to move back enter [A]\nif you want to finish the sale [X]\nif you want to select the item enter the index number or type the item name\nEnter input here: ");
				scanf(" %[^\n]%*c", input);
				printf("");
			}else if (strcmp(input, "X") == 0){
				break;
			}else{
				found = 0;
				for(i = 0; i < length; i++){
					sprintf(ids, "%d", items[i].itemid);
					if (strcmp(input, ids) == 0 || strcmp(input, items[i].name) == 0){
						item = items[i];
						idx = i;
						found = 1;
						break;
					}
				}
				if (found == 1){
				
					printf("Enter the quantity: ");
					scanf(" %d", &quantity);
					printf("");
					if (item.instock >= quantity){
						sold[count] = CreateNewSoldItem(item, quantity);
						
						item.instock = item.instock-quantity;
						items[idx] = item;
						count++;
					}else{
						printf("There is not enough inventory in stock. There are %d units of this item in stock\n", item.instock);
						printf("Press enter to continue...\n");
						getchar();
					}
					printf("If you want to move to the next page enter [D]\nif you want to move back enter [A]\nif you want to finish the sale [X]\nif you want to select the item enter the index number or type the item name\nEnter input here: ");
					printf("");
					scanf(" %[^\n]%*c", input);
					printf("");
				}else{
					
					printf("If you want to move to the next page enter [D]\nif you want to move back enter [A]\nif you want to finish the sale [X]\nif you want to select the item enter the index number or type the item name\nEnter input here: ");
					printf("");
					scanf(" %[^\n]%*c", input);
					printf("");
				}
			}
			
		}
		
		if(IsNewFile(IDFILE) != 0){
			iddata = fopen(IDFILE, "rb+");
			while(fread(&id, sizeof( MaxIDs), 1, iddata)){
				//loop through all structures in ineventory file
				saleid = id.saleid+1;
			}
			fclose(iddata);
			
			itemdata = fopen(ITEMFILE, "wb");
			for(i=0;i<length;i++){
				fwrite(&items[i], sizeof(Item), 1, itemdata);
			}
			fclose(itemdata);
			
			SaleItems saleitems = CreateNewSaleItems(saleid, sold, userid);
			Sale salelog = CreateNewSale(saleitems);
			
			iddata = fopen(IDFILE, "r");
			while(fread(&id, sizeof( MaxIDs), 1, iddata)){
				//loop through all structures in ineventory file
				id.saleid = saleid;
				iddata2 = fopen(IDFILE, "wb");
				fwrite(&id, sizeof( MaxIDs), 1, iddata2);
				fclose(iddata2);
			}
			fclose(iddata);
			
			saledata = fopen(SALEFILE, "ab");
			fwrite(&saleitems, sizeof(SaleItems), 1, saledata);
			fclose(saledata);
			
			saledata = fopen(SALELOGFILE, "ab");
			fwrite(&salelog, sizeof(Sale), 1, saledata);
			fclose(saledata);
		}
	}else{
		printf("Please register items to make sales\n");
		printf("Press enter to continue...\n");
		getchar();
	}
	return;
}


void AddItemData(Item items[], int length){
	MaxIDs id, data;
	int isemptyID = IsNewFile(IDFILE);
	FILE *itemdata, *iddata, *iddata2;
	int i = 0;
	int count;
	int itemid = 0;
	if(isemptyID != 0){
		iddata = fopen(IDFILE, "r");
		while(fread(&id, sizeof( MaxIDs), 1, iddata)){
			//loop through all structures in ineventory file
			itemid = id.itemid;
		}
		fclose(iddata);
	
	}else{
		itemid = -1;
		itemdata = fopen(ITEMFILE, "wb");
		for(i=0;i<length;i++){
			itemid++;
			items[i].itemid = itemid;
			fwrite(&items[i], sizeof(Item), 1, itemdata);
		}
		fclose(itemdata);
		MaxIDs id = {-1,-1,-1};
		iddata = fopen(IDFILE, "wb");
		id.itemid = itemid;
		fwrite(&id, sizeof( MaxIDs), 1, iddata);
		fclose(iddata);
		return;
	}
	
	itemdata = fopen(ITEMFILE, "ab");
	count = itemid;
	for(i=0;i<length;i++){
		itemid++;
		items[i].itemid = itemid;
		fwrite(&items[i], sizeof(Item), 1, itemdata);
	}
	fclose(itemdata);
	
	iddata = fopen(IDFILE, "r");
	while(fread(&id, sizeof( MaxIDs), 1, iddata)){
		//loop through all structures in ineventory file
		id.itemid = itemid;
		iddata2 = fopen(IDFILE, "wb");
		fwrite(&id, sizeof( MaxIDs), 1, iddata2);
		fclose(iddata2);
		
	}
	fclose(iddata);
	
	
}


void AddUserData(User users[], int length){
	MaxIDs id, data;
	int isemptyID = IsNewFile(IDFILE);
	FILE *userdata, *iddata, *iddata2;
	int i = 0;
	int count;
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
		MaxIDs id = {-1,-1,-1};
		id.userid = userid;
		
		fwrite(&id, sizeof( MaxIDs), 1, iddata);
		fclose(iddata);
		return;
	}
	userdata = fopen(USERFILE, "ab");
	count = userid;
	for(i=0;i<length;i++){
		userid++;
		users[i].userid = userid;
		fwrite(&users[i], sizeof(User), 1, userdata);
	}
	fclose(userdata);
	iddata = fopen(IDFILE, "r");
	
	while(fread(&id, sizeof( MaxIDs), 1, iddata)){
		//loop through all structures in ineventory file
		id.userid = userid;
		iddata2 = fopen(IDFILE, "wb");
		fwrite(&id, sizeof( MaxIDs), 1, iddata2);
		fclose(iddata2);
	}
	fclose(iddata);
	
}


int main(int argc, char *argv[]) {
	//User p1 = {"1", "bigman", "pop123", "Drew", "Quashie", "28", "05", "2104", "m"};
	//char data[20];
	DOB dob = CreateNewDOB("28", "05", "2004");
	DOB dob1 = CreateNewDOB("28", "05", "2005");
	DOB dob2 = CreateNewDOB("28", "05", "2006");
	AccountInfo account = CreateNewAccount("quashman", "pop123");
	User users[3] = {
		CreateNewUser(0, account, "Drew", "Quashie", dob, "manager"), 
		CreateNewUser(0, account, "Drew", "Quashie", dob1, "manager"),
		CreateNewUser(0, account, "John", "Doe", dob2, "manager")
	};
	
	Item items[3] = {
		CreateNewItem(0, "Orchard 24pk", 100, 72, 55 ), 
		CreateNewItem(0, "Holiday Snacks Party Pack", 100, 32.50, 20 ),
		CreateNewItem(0, "Blue Waters 24pk", 100, 76, 50 )
	};
	

	//strcpy(data,  UserId(p1));
	//AddUserData(users, 3);
	
	//printf("length %d", length);
	//PrintAllUsers(0,9);
	printf("\n\n\n");
	//AddItemData(items, 3);
	//Item *item = PrintAllItems(0,9);
	MakeSale(3);
	//char lname[20] = "Doe";
	//SearchForUser(lname);
	char idfile[20] = "IdTracker.bin";
	//int size = isNewFile(idfile);
	char userfile[20] = "UserDataBase.bin";
	//AddUserData(users, 2, size, userfile,idfile);
	
	
	return 0;
}


int login(char username[20], char password[20]){
	
}


