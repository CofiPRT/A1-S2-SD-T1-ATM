#include "main_header.h"

void add_card(card *new_card, LC *card_list)
{
	// make sure it doesn't already exist
	card *test_card = find_card(new_card->card_number, card_list);
	
	if (test_card)
	{
		fprintf(card_list->out_file, "The card already exists\n");
		free_card(new_card); // not needed
		return;
	}

	if (card_list->curr_cards == card_list->max_cards)
	{
		rearrange(card_list);
	}

	int poz = calculate_poz(new_card->card_number, card_list->max_cards);

	LSC *curr_LSC = card_list->first_LSC; // start moving through the LSCs

	if (!curr_LSC)
	{
		// initialize if necessary
		curr_LSC = alloc_LSC();
		card_list->first_LSC = curr_LSC;
	}

	while (poz)
	{
		LSC *prev_LSC = curr_LSC; // in order to link the LSCs
		curr_LSC = curr_LSC->next_LSC; // move through the LSCs
		if (!curr_LSC)
		{
			curr_LSC = alloc_LSC();
			prev_LSC->next_LSC = curr_LSC; // link
		}
		poz--;
	}

	if (curr_LSC->first_card)
	{
		// if the LSC is not empty, link this card
		new_card->next_card = curr_LSC->first_card;
	}
	
	// now update the first card of the LSC
	curr_LSC->first_card = new_card;

	(card_list->curr_cards)++; // one more card added
}

void delete_card(LC *card_list)
{
	char *card_number = malloc(MAXCHARS * sizeof(char));
	sscanf(card_list->buffer, "%*s %s ", card_number);

	int poz = calculate_poz(card_number, card_list->max_cards);

	LSC *curr_LSC = card_list->first_LSC;

	while (poz && curr_LSC)
	{
		// move to 'poz'
		curr_LSC = curr_LSC->next_LSC;
		poz--;
	}

	card *curr_card = curr_LSC->first_card;

	if (!strcmp(curr_card->card_number, card_number))
	{
		// in case it's the first card
		curr_LSC->first_card = curr_card->next_card;
		free_card(curr_card);
		free(card_number);
		(card_list->curr_cards)--; // one less card
		return;
	}

	while (curr_card)
	{
		card *prev_card = curr_card;
		curr_card = curr_card->next_card;

		if (!strcmp(curr_card->card_number, card_number))
		{
			// link the adjacent cards
			prev_card->next_card = curr_card->next_card;
			free_card(curr_card);
			free(card_number);
			(card_list->curr_cards)--;
			return; // success
		}
	}
}

void reverse(LC *card_list)
{
	char *card_number1 = malloc(MAXCHARS * sizeof(char)); // source
	char *card_number2 = malloc(MAXCHARS * sizeof(char)); // destination
	int sum;
	sscanf(card_list->buffer, "%*s %s %s %d ", 	card_number1,
												card_number2,
												&sum);

	char *operation_to_search = malloc(MAXCHARS * sizeof(char));
	sprintf(operation_to_search, "%s %s %s %d", "transfer_funds",
												card_number1,
												card_number2,
												sum);

	card *card1 = find_card(card_number1, card_list);
	card *card2 = find_card(card_number2, card_list);

	if (sum > card2->balance)
	{
		fprintf(card_list->out_file, "The transaction cannot be reversed\n");
	}
	else
	{
		remove_entry(operation_to_search, card2);

		card1->balance += sum;
		card2->balance -= sum;
		add_entry(card_list->buffer, "SUCCESS", card1);
	}

	free(card_number1);
	free(card_number2);
	free(operation_to_search);
}

void unblock_card(LC *card_list)
{
	char *card_number = malloc(MAXCHARS * sizeof(char));
	sscanf(card_list->buffer, "%*s %s ", card_number);

	card *card = find_card(card_number, card_list);

	card->PIN_attempts = 0;
	strcpy(card->status, "ACTIVE");

	free(card_number);
}

void show_database(LC *card_list)
{
	char *card_number = malloc(MAXCHARS * sizeof(char));
	if (sscanf(card_list->buffer, "%*s %s ", card_number) != EOF)
	{
		print_card(find_card(card_number, card_list), card_list->out_file);
		free(card_number);
		return;
	}

	LSC *curr_LSC = card_list->first_LSC;
	int index = 0;

	while (curr_LSC)
	{
		fprintf(card_list->out_file, "pos%d: [", index);

		card *curr_card = curr_LSC->first_card;

		if (curr_card)
		{
			// first card
			fprintf(card_list->out_file, "\n");
		}

		while (curr_card)
		{
			print_card(curr_card, card_list->out_file);

			curr_card = curr_card->next_card;
		}
		fprintf(card_list->out_file, "]\n");

		curr_LSC = curr_LSC->next_LSC;
		index++;
	}

	free(card_number);
}

void rearrange(LC *card_list)
{
	card_list->max_cards *= 2;
	LC *new_LC = alloc_LC(card_list->max_cards);

	LSC *curr_LSC = card_list->first_LSC; // start moving through the LSCs

	while (curr_LSC)
	{
		card *curr_card = curr_LSC->first_card;

		while (curr_card)
		{
			card *prev_card = curr_card;
			curr_card = curr_card->next_card;

			prev_card->next_card = NULL; // only move this card
			add_card(prev_card, new_LC);
		}

		// remove the link in order to not free all the cards with 'free_LSC'
		LSC *prev_LSC = curr_LSC;
		curr_LSC->first_card = NULL;
		curr_LSC = curr_LSC->next_LSC;
		free_LSC(prev_LSC);
	}

	// link the newly created LSC to our main 'card_list'
	card_list->first_LSC = new_LC->first_LSC;
	new_LC->first_LSC = NULL;
	free_LC(new_LC);
}

