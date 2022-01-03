#include "main_header.h"

history *alloc_history()
{
	history *new_entry = malloc(sizeof(history));

	new_entry->result = malloc(MAXCHARS * sizeof(char));
	new_entry->operation = malloc(MAXCHARS * sizeof(char));

	new_entry->next_entry = NULL;

	return new_entry;
}

card *alloc_card()
{
	card *new_card = malloc(sizeof(card));

	new_card->card_number = malloc(MAXCHARS * sizeof(char));
	new_card->PIN = malloc(MAXCHARS * sizeof(char));
	new_card->expiry_date = malloc(MAXCHARS * sizeof(char));
	new_card->CVV = malloc(MAXCHARS * sizeof(char));

	new_card->balance = 0;
	new_card->PIN_attempts = 0;

	new_card->status = malloc(MAXCHARS * sizeof(char));
	strcpy(new_card->status, "NEW");

	new_card->latest_entry = NULL;
	new_card->next_card = NULL;

	return new_card;
}

LSC *alloc_LSC()
{
	LSC *new_LSC = malloc(sizeof(LSC));

	new_LSC->first_card = NULL;
	new_LSC->next_LSC = NULL;

	return new_LSC;
}

LC *alloc_LC(int max_cards)
{
	LC *card_list = malloc(sizeof(LC));

	card_list->buffer = malloc(MAXCHARS * sizeof(char));

	card_list->in_file = NULL;
	card_list->out_file = NULL;

	card_list->curr_cards = 0;
	card_list->max_cards = max_cards;

	card_list->inserted_card = NULL;
	card_list->first_LSC = NULL;

	return card_list;
}

void free_history(history *entry)
{
	free(entry->result);
	free(entry->operation);
	entry->next_entry = NULL;

	free(entry);
}

void free_card(card *card)
{
	free(card->card_number);
	free(card->PIN);
	free(card->expiry_date);
	free(card->CVV);
	free(card->status);

	while (card->latest_entry)
	{
		// free all entries
		history *prev_entry = card->latest_entry;
		card->latest_entry = card->latest_entry->next_entry;
		free_history(prev_entry);
	}

	card->next_card = NULL;

	free(card);
}

void free_LSC(LSC *LSC)
{
	while (LSC->first_card)
	{
		// free all cards
		card *prev_card = LSC->first_card;
		LSC->first_card = LSC->first_card->next_card;
		free_card(prev_card);
	}

	LSC->next_LSC = NULL;

	free(LSC);
}

void free_LC(LC *LC)
{
	free(LC->buffer);

	LC->in_file = NULL;
	LC->out_file = NULL;
	LC->inserted_card = NULL;
	while (LC->first_LSC)
	{
		// free all LSCs
		LSC *prev_LSC = LC->first_LSC;
		LC->first_LSC = LC->first_LSC->next_LSC;
		free_LSC(prev_LSC);
	}

	free(LC);
}