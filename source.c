#include "main_header.h"

void find_function(LC *card_list)
{
	char *operation = malloc(MAXCHARS * sizeof(char));
	sscanf(card_list->buffer, "%s ", operation);

	if (!strcmp(operation, "add_card"))
	{
		add_card(create_card(card_list->buffer), card_list);
	}
	if (!strcmp(operation, "delete_card"))
	{
		delete_card(card_list);
	}
	if (!strcmp(operation, "reverse_transaction"))
	{
		reverse(card_list);
	}
	if (!strcmp(operation, "unblock_card"))
	{
		unblock_card(card_list);
	}
	if (!strcmp(operation, "show"))
	{
		show_database(card_list);
	}
	if (!strcmp(operation, "insert_card"))
	{
		insert_card(card_list);
	}
	if (!strcmp(operation, "recharge"))
	{
		recharge(card_list);
	}
	if (!strcmp(operation, "cash_withdrawal"))
	{
		withdraw(card_list);
	}
	if (!strcmp(operation, "balance_inquiry"))
	{
		balance_inquiry(card_list);
	}
	if (!strcmp(operation, "transfer_funds"))
	{
		transfer(card_list);
	}
	if (!strcmp(operation, "pin_change"))
	{
		pin_change(card_list);
	}
	if (!strcmp(operation, "cancel"))
	{
		cancel_card(card_list);
	}
	free(operation);
}

int main()
{
	FILE *in_file; // open input file for reading
	in_file = fopen("input.in", "r");
	if (!in_file)
	{
		fprintf(stderr, "Input file could not be opened!\n");
		return -1;
	}

	int max_cards;
	fscanf(in_file, "%d ", &max_cards);

	LC *card_list = alloc_LC(max_cards);

	FILE *out_file; // open output file for writing
	out_file = fopen("output.out", "w");
	if (!out_file)
	{
		fprintf(stderr, "Output file could not be opened!\n");
		return -1;
	}

	card_list->in_file = in_file;
	card_list->out_file = out_file;

	// start doing operations
	while (fgets(card_list->buffer, MAXCHARS, in_file))
	{
		if (card_list->buffer[strlen(card_list->buffer) - 1] == '\n')
		{
			// get rid of it entirely, to facilitate the use of sscanf
			card_list->buffer[strlen(card_list->buffer) - 1] = '\0';
		}
		find_function(card_list);
	}

	free_LC(card_list);
	fclose(in_file);
	fclose(out_file);

	return 0;
}