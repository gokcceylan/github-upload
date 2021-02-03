#include <stdio.h>//printf scanf fonksiyonlarý için
#include <stdlib.h>//calloc için

typedef struct Account{
	char* ownerName;
	char* password;
	int amount;//piazzadaki açýklamaya göre int kullanýlacak
	int accountCreationYear;	
}Account;

typedef struct Bank{
	char* bankName;
	int maxNumberOfAccounts;//alloke edilecek yeri veriyor
	int curNumberOfAccounts;//for döngülerinde fazladan döngü dönmesini engelliyor
	Account** allAccounts;//pointer dizisi olacak. accountlarýn adresini tutan bir pointer dizisi aslýnda. tutulan adresler, account adresleri
}Bank;

Bank* bank_constructor(char* bankName, int maxNumberOfAccounts){
	Bank* ptr;
	ptr=(Bank*)calloc(1, sizeof(Bank));
	ptr->curNumberOfAccounts = 0;
	ptr->maxNumberOfAccounts = maxNumberOfAccounts;
	ptr->bankName=bankName;//C otomatik yer alloke edip sonuna \0 koyuyor	
	ptr->allAccounts=calloc(maxNumberOfAccounts, sizeof(Account*));
	return ptr;
}
	//do not forget to allocate memory
	//create a bank with the given arguments and return it
	//person constructor gibi
	
void bankInfo(Bank* b){
	printf("Name of the bank: %s\n", b->bankName);
	int j, sum=0;
	for(j=0 ; j < b->curNumberOfAccounts ; j++){
		sum += b->allAccounts[j]->amount;
	}
	printf("Total Capital	: %d\n", sum);
}
	//shows info about the bank and sum of funds deposited in this bank

void customerInfo(Bank *b){	
	printf("Customer List	:\n");
	int i;
	for(i=0 ; i < b->curNumberOfAccounts; i++){
	printf("%s since %d.\n", b->allAccounts[i]->ownerName, b->allAccounts[i]->accountCreationYear);
	}
}
//displays info about ALL the customers of this bank

int createAccount(Bank* b, char* holderName, char* password, int accountCreationYear){

	if(b->curNumberOfAccounts <= b->maxNumberOfAccounts){	
	b->curNumberOfAccounts++;
	int i = b->curNumberOfAccounts;
	b->allAccounts[i-1] = calloc(1, sizeof(Account));
//	b->allAccounts = calloc(1, sizeof(Account*));
	b->allAccounts[i -1]->accountCreationYear = accountCreationYear;
	b->allAccounts[i -1]->ownerName = holderName;
	b->allAccounts[i -1]->password = password;
	b->allAccounts[i -1]->amount = 0;
	return b->curNumberOfAccounts;	
	}
}
//creates an account in the given bank with given info.
//different banks can have the same account number AND combination of bank name and account name will be used to access any account
//returns the account number in that bank
//allocate memory

int depositMoney(Bank* b, int accountNumber, int amount){
	b->allAccounts[accountNumber-1]->amount += amount;
	return amount;
}
//deposits given amount of money to the account with given info
//can be done w/o password
//returns the deposited amount

int withdrawMoney(Bank* b, int accountNumber, char* password, int amount){
	if(password==b->allAccounts[accountNumber-1]->password){
		if(amount <= b->allAccounts[accountNumber-1]->amount){
		b->allAccounts[accountNumber-1]->amount -= amount;
		return amount;
		}
		else{
		printf("Insufficient funds!\n");
		return 0;
		}
	}
	else{
		printf("Wrong Password!\n");
		return 0;
	}
}
//is made if account has sufficient funds && password is correct
//else if printf errors accordingly
//returns the withdrawn amount

int transferMoney(Bank* senderBank, int senderAccount, char* senderPassword, Bank* receiverBank, int receiverAccount, int amount){
	if(senderBank->allAccounts[senderAccount-1]->password==senderPassword){
		if(senderBank->allAccounts[senderAccount-1]->amount>=amount){
			senderBank->allAccounts[senderAccount-1]->amount -= amount;
			receiverBank->allAccounts[receiverAccount-1]->amount += amount;
			return amount;
		}
		else{
			printf("Insufficient funds!\n");
			return 0;
		}
	}
	else{
		printf("Wrong Password!\n");
		return 0;
	}
}
//is made if account has sufficient funds && password is correct
//else if printf errors accordingly
//returns the transferred amount

void checkBalance(Bank* b, int accountNumber, char* password){
	if(b->allAccounts[accountNumber-1]->password==password){
		printf("Account balance: %d\n", b->allAccounts[accountNumber-1]->amount);
	}
	else{
		printf("Wrong Password!\n");
	}
}
//display balance OR the error message depending the correctness of the password accordingly
//don't show balance unless password is correct


int main(){
	Bank* b1 = bank_constructor("BankA", 3);
	Bank* b2 = bank_constructor("BankB", 4);
	int hesap11= createAccount(b1, "Ahmet", "pass", 1980);
	int hesap12= createAccount(b1, "Veli", "word", 2000);
	int hesap21= createAccount(b2, "Mehmet", "sifre", 1990);
	depositMoney(b1, hesap11, 50);	
	depositMoney(b1, hesap12, 80);
	transferMoney(b1, hesap11, "hatali", b2, hesap21, 20);
	transferMoney(b1, hesap11, "pass", b2, hesap21, 20);
	withdrawMoney(b1, hesap12, "hatali", 40);
	withdrawMoney(b1, hesap12, "word", 120);
	withdrawMoney(b1, hesap12, "word", 40);
//	printf("%d %d %d %d %d %d %d %d", hesap11, hesap12, hesap21, a, b, c, d, e);
	checkBalance(b1, hesap11, "hatali");
	checkBalance(b1, hesap11, "pass");
	checkBalance(b2, hesap21, "sifre");
	checkBalance(b1, hesap12, "word");
	customerInfo(b1);	bankInfo(b1);
	bankInfo(b2);
	
	return 0;
}
