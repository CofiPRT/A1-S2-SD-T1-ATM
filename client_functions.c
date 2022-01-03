#include "main_header.h"

void insert_card(LC *card_list)
{
	char *card_number = malloc(MAXCHARS * sizeof(char));
	char *PIN = malloc(MAXCHARS * sizeof(char));	
	sscanf(card_list->buffer, "%*s %s %s ", card_number, PIN);
	card *card = find_card(card_number, card_list);

	if (is_blocked(card_number, card_list))
	{
		// locked card
		add_entry(card_list->buffer, "FAIL", card);
		free(card_number);
		free(PIN);
		return;
	}

	if (strcmp(card->PIN, PIN))
	{
		// incorrect PIN
		fprintf(card_list->out_file, "Invalid PIN\n");
		add_entry(card_list->buffer, "FAIL", card);
		(card->PIN_attempts)++;

		if (card->PIN_attempts == 3)
		{
			strcpy(card->status, "LOCKED");
			fprintf(card_list->out_file,
					"The card is blocked. "
					"Please contact the administrator.\n");
		}
		free(card_number);
		free(PIN);
		return;
	}

	// correct PIN
	if (!strcmp(card->status, "NEW"))
	{
		// PIN has not been changed
		fprintf(card_list->out_file, "You must change your PIN.\n");
	}

	card->PIN_attempts = 0; // finally a successful attempt
	card_list->inserted_card = card;
	add_entry(card_list->buffer, "SUCCESS", card);
	free(card_number);
	free(PIN);
}

void recharge(LC *card_list)
{
	char *card_number = malloc(MAXCHARS * sizeof(char));
	int sum;
	sscanf(card_list->buffer, "%*s %s %d ", card_number, &sum);

	card *card = find_card(card_number, card_list);

	if (is_blocked(card_number, card_list))
	{
		// locked card
		add_entry(card_list->buffer, "FAIL", card);
		free(card_number);
		return;
	}

	if (sum % 10)
	{
		fprintf(card_list->out_file,
				"The added amount must be multiple of 10\n");
		add_entry(card_list->buffer, "FAIL", card);
	}
	else
	{
		card->balance += sum;
		fprintf(card_list->out_file, "%d\n", card->balance);
		add_entry(card_list->buffer, "SUCCESS", card);
	}

	free(card_number);
	// disconnect card
	card_list->inserted_card = NULL;
}

void withdraw(LC *card_list)
{
	char *card_number = malloc(MAXCHARS * sizeof(char));
	int sum;
	sscanf(card_list->buffer, "%*s %s %d ", card_number, &sum);

	card *card = find_card(card_number, card_list);

	if (is_blocked(card_number, card_list))
	{
		// locked card
		add_entry(card_list->buffer, "FAIL", card);
		free(card_number);
		return;
	}

	if (sum % 10)
	{
		fprintf(card_list->out_file,
				"The requested amount must be multiple of 10\n");
		add_entry(card_list->buffer, "FAIL", card);
	}
	else if (sum > card->balance)
	{
		fprintf(card_list->out_file, "Insufficient funds\n");
		add_entry(card_list->buffer, "FAIL", card);
	}
	else
	{
		card->balance -= sum;
		fprintf(card_list->out_file, "%d\n", card->balance);
		add_entry(card_list->buffer, "SUCCESS", card);
	}

	free(card_number);
	// disconnect card
	card_list->inserted_card = NULL;
}

void balance_inquiry(LC *card_list)
{
	char *card_number = malloc(MAXCHARS * sizeof(char));
	sscanf(card_list->buffer, "%*s %s ", card_number);

	card *card = find_card(card_number, card_list);

	if (is_blocked(card_number, card_list))
	{
		// locked card
		add_entry(card_list->buffer, "FAIL", card);
		free(card_number);
		return;
	}

	fprintf(card_list->out_file, "%d\n", card->balance);

	add_entry(card_list->buffer, "SUCCESS", card);
	free(card_number);
	// disconnect card
	card_list->inserted_card = NULL;
}

void transfer(LC *card_list)
{
	char *card_number1 = malloc(MAXCHARS * sizeof(char));
	char *card_number2 = malloc(MAXCHARS * sizeof(char));
	int sum;
	sscanf(card_list->buffer, "%*s %s %s %d ", 	card_number1,
												card_number2,
												&sum);

	card *card1 = find_card(card_number1, card_list);
	card *card2 = find_card(card_number2, card_list);

	if (is_blocked(card_number1, card_list))
	{
		// locked card
		add_entry(card_list->buffer, "FAIL", card1);
		free(card_number1);
		free(card_number2);
		return;
	}

	if (sum % 10)
	{
		fprintf(card_list->out_file,
				"The transferred amount must be multiple of 10\n");
		add_entry(card_list->buffer, "FAIL", card1);
	}
	else if (sum > card1->balance)
	{
		fprintf(card_list->out_file, "Insufficient funds\n");
		add_entry(card_list->buffer, "FAIL", card1);
	}
	else
	{
		card1->balance -= sum;
		card2->balance += sum;
		fprintf(card_list->out_file, "%d\n", card1->balance);
		add_entry(card_list->buffer, "SUCCESS", card1);
		add_entry(card_list->buffer, "SUCCESS", card2);
	}

	free(card_number1);
	free(card_number2);
	// disconnect card
	card_list->inserted_card = NULL;
}

void pin_change(LC *card_list)
{
	char *card_number = malloc(MAXCHARS * sizeof(char));
	char *PIN = malloc(MAXCHARS * sizeof(char));
	sscanf(card_list->buffer, "%*s %s %s ", card_number, PIN);

	card *card = find_card(card_number, card_list);

	if (is_blocked(card_number, card_list))
	{
		// locked card
		add_entry(card_list->buffer, "FAIL", card);
		free(card_number);
		free(PIN);
		return;
	}

	if (is_PIN(PIN))
	{
		strcpy(card->PIN, PIN);
		strcpy(card->status, "ACTIVE");
		add_entry(card_list->buffer, "SUCCESS", card);
		// disconnect card
		card_list->inserted_card = NULL;
	}
	else
	{
		fprintf(card_list->out_file, "Invalid PIN\n");
		add_entry(card_list->buffer, "FAIL", card);
	}

	free(card_number);
	free(PIN);
	// disconnect card
	card_list->inserted_card = NULL;
}

void cancel_card(LC *card_list)
{
	char *card_number = malloc(MAXCHARS * sizeof(char));
	sscanf(card_list->buffer, "%*s %s ", card_number);

	card *card = find_card(card_number, card_list);

	card_list->inserted_card = NULL;

	add_entry(card_list->buffer, "SUCCESS", card);
	free(card_number);
}