#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _ATM_HEADER_
#define _ATM_HEADER_

#define MAXCHARS 100

typedef struct history
{
	char *result;
	char *operation;
	struct history *next_entry;

} history;

typedef struct card
{
	char *card_number;
	char *PIN;
	char *expiry_date;
	char *CVV;
	int balance;
	int PIN_attempts;
	char *status;
	struct history *latest_entry;
	struct card *next_card;
} card;

typedef struct LSC
{
	struct card *first_card;
	struct LSC *next_LSC;
} LSC;

typedef struct LC
{
	char *buffer;
	FILE *in_file;
	FILE *out_file;
	int curr_cards;
	int max_cards;
	struct card *inserted_card;
	struct LSC *first_LSC;
} LC;

// alloc_free_functions.c
history *alloc_history();
card *alloc_card();
LSC *alloc_LSC();
LC *alloc_LC(int max_cards);
void free_history(history *entry);
void free_card(card *card);
void free_LSC(LSC *LSC);
void free_LC(LC *LC);

// support_functions.c
card *create_card(char *buffer);
card *find_card(char *card_number, LC *card_list);
void print_card(card *card, FILE *out_file);
void add_entry(char *operation, char *result, card *card);
void remove_entry(char *operation, card *card);
int calculate_poz(char *card_number, int max_cards);
int is_blocked(char *card_number, LC *card_list);
int is_PIN(char *PIN);

// client_functions.c
void insert_card(LC *card_list);
void recharge(LC *card_list);
void withdraw(LC *card_list);
void balance_inquiry(LC *card_list);
void transfer(LC *card_list);
void pin_change(LC *card_list);
void cancel_card(LC *card_list);

// admin_functions.c
void add_card(card *new_card, LC *card_list);
void delete_card(LC *card_list);
void reverse(LC *card_list);
void unblock_card(LC *card_list);
void show_database(LC *card_list);
void rearrange(LC *card_list);

// source.c functions
void find_function(LC *card_list);

#endif