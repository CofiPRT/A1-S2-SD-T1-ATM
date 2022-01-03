#include "main_header.h"

card *create_card(char *buffer)
{
	// create empty card
	card *new_card = alloc_card();

	// fill the empty card
	sscanf(buffer, "%*s %s %s %s %s ", 	new_card->card_number,
										new_card->PIN,
										new_card->expiry_date,
										new_card->CVV);
	// the first %*s is the name of the command

	return new_card;
}

card *find_card(char *card_number, LC *card_list)
{
	int poz = calculate_poz(card_number, card_list->max_cards);

	LSC *curr_LSC = card_list->first_LSC;

	while (poz && curr_LSC)
	{
		// move to 'poz'
		curr_LSC = curr_LSC->next_LSC;
		poz--;
	}

	if (!curr_LSC)
	{
		// LSC on 'poz' does not even exist
		return NULL;
	}

	card *curr_card = curr_LSC->first_card;

	while (curr_card)
	{
		if (!strcmp(curr_card->card_number, card_number))
		{
			// the card has been found
			return curr_card;
		}
		curr_card = curr_card->next_card;
	}

	// no card has been found
	return NULL;
}

void print_card(card *card, FILE *out_file)
{
	// card info
	fprintf(out_file, 	"(card number: %s, PIN: %s, expiry date: %s, CVV: %s, "
						"balance: %d, status: %s, history: [",
						card->card_number, card->PIN, card->expiry_date,
						card->CVV, card->balance, card->status);

	history *curr_entry = card->latest_entry;

	// history info
	while (curr_entry)
	{
		fprintf(out_file,	"(%s, %s)", curr_entry->result,
										curr_entry->operation);
		if (curr_entry->next_entry)
		{
			// separator
			fprintf(out_file, ", ");
		}

		curr_entry = curr_entry->next_entry;
	}

	fprintf(out_file, "])\n");
}

void add_entry(char *operation, char *result, card *card)
{
	// create new entry
	history *new_entry = alloc_history();

	// fill the entry
	strcpy(new_entry->operation, operation);
	strcpy(new_entry->result, result);

	if (card->latest_entry)
	{
		// if there already is an entry in the card, link this one
		new_entry->next_entry = card->latest_entry;
	}

	// now update the first entry of the card
	card->latest_entry = new_entry;
}

void remove_entry(char *operation, card *card)
{
	history *curr_entry = card->latest_entry;
	if (!strcmp(curr_entry->operation, operation))
	{
		// it's the latest entry
		card->latest_entry = curr_entry->next_entry;
		free_history(curr_entry);
		return;
	}

	while (curr_entry)
	{
		history *prev_entry = curr_entry;
		curr_entry = curr_entry->next_entry;

		if (!strcmp(curr_entry->operation, operation))
		{
			// link the adjacient entries
			prev_entry->next_entry = curr_entry->next_entry;
			free_history(curr_entry);
			return;
		}
	}
}

int calculate_poz(char *card_number, int max_cards)
{
	int i, sum = 0;
	for (i = 0; i < 16; i++)
	{
		sum += (int) (card_number[i] - '0');
	}

	return sum % max_cards;
}

int is_blocked(char *card_number, LC *card_list)
{
	card *card = find_card(card_number, card_list);

	if (!strcmp(card->status, "LOCKED"))
	{
		fprintf(card_list->out_file, 	"The card is blocked. "
										"Please contact the administrator.\n");
		return 1;
	}

	return 0;
}

int is_PIN(char *PIN)
{
	if (strlen(PIN) != 4)
	{
		// non 4-digit PIN
		return 0;
	}

	int i;
	for (i = 0; i < (int) strlen(PIN); i++)
	{
		if (PIN[i] < '0' || PIN[i] > '9')
		{
			// not a digit
			return 0;
		}
	}

	return 1;
}