#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "aux_func.h"

#define MAX_WORDS 3

Node_t* create_node(char* word)
{
	//allocate memory for node
	Node_t* node = (Node_t*)calloc(1, sizeof(Node_t));
	if(!node)
	{
		printf("Error allocating memory\n");
		return 0;
	}
	//allocate memory for the word from node
	node->word = (char*)calloc((strlen(word) + 1), sizeof(char));
	if(!node->word)
	{
		printf("Error allocating memory\n");
		return 0;
	}
	strcpy(node->word, word);
	node->next = NULL;

	return node;
}

void free_hashTable(HashTable_t** hashTable)
{
	int i;
	//iterate through hashtable and free every node
	for(i = 0; i < (*hashTable)->size; i++)
	{
		while((*hashTable)->buckets[i])
		{
			Node_t *to_be_deleted = (*hashTable)->buckets[i];
			(*hashTable)->buckets[i] = (Node_t*)(*hashTable)->buckets[i]->next;
			free(to_be_deleted->word);
			free(to_be_deleted);
		}
	}
	//free the array with buckets
	free((*hashTable)->buckets);
	//free the structure hashtable
	free(*hashTable);
}

HashTable_t* create_hashTable(unsigned int size)
{
	int i;
	//allocate memory for hashtable
	HashTable_t* hashTable = (HashTable_t*)calloc(1, sizeof(HashTable_t));
	if(!hashTable)
	{
		printf("Error allocating memory\n");
		return 0;
	}
	//set the size for hashtable
	hashTable->size = size;
	//allocate memory for buckets
	hashTable->buckets = (Node_t**)calloc(hashTable->size, sizeof(Node_t*));
	if(!hashTable->buckets)
	{
		printf("Error allocating memory\n");
		return 0;
	}

	return hashTable;
}

void add(HashTable_t *hashTable, char *word)
{
	int hash_position = hash(word, hashTable->size);
	if(hashTable->buckets[hash_position] == NULL)
	{
		hashTable->buckets[hash_position] = create_node(word);
	}
	else
	{
		Node_t* pointer_to_node = hashTable->buckets[hash_position];
		while(1)
		{
			//if the bucket is not null, it is iterating thorugh the list of nodes in order
			//to find the end, where will be putted the new node
			if(strcmp(word, pointer_to_node->word) == 0)
				return;
			if(pointer_to_node->next == NULL)
				break;
			pointer_to_node = (Node_t*)pointer_to_node->next;
		}

		pointer_to_node->next = (struct Node_t*)create_node(word);
	}
}

void remove_word(HashTable_t *hashTable, char *word)
{
	int hash_position = hash(word, hashTable->size);
	Node_t* pointer_to_node;
	if(hashTable->buckets[hash_position] == NULL)
		return;
	if(strcmp(hashTable->buckets[hash_position]->word, word) == 0)
	{
		//if the wanted words is the first in list, it is deleted
		Node_t* to_be_deleted = hashTable->buckets[hash_position];
		hashTable->buckets[hash_position] = (Node_t*)hashTable->buckets[hash_position]->next;
		free(to_be_deleted->word);
		free(to_be_deleted);
		return;
	}
	
	pointer_to_node = hashTable->buckets[hash_position];
	while(pointer_to_node->next != NULL && strcmp(((Node_t*)pointer_to_node->next)->word, word) != 0)
	{
		pointer_to_node = (Node_t*)pointer_to_node->next;
	}
	
	//here we are at the word that must be deleted or at the end of list
	if(pointer_to_node->next != NULL)
	{
		Node_t* to_be_deleted = (Node_t*)pointer_to_node->next;
		pointer_to_node->next = ((Node_t*)pointer_to_node->next)->next;
		free(to_be_deleted->word);
		free(to_be_deleted);
	}
}

void clear(HashTable_t* hashTable)
{
	int i;
	//iterates through hashtable and delete all the nodes
	for(i = 0 ; i < hashTable->size; i++)
	{
		while(hashTable->buckets[i])
		{
			Node_t* pointer_to_node = hashTable->buckets[i];
			hashTable->buckets[i] = (Node_t*)hashTable->buckets[i]->next;
			free(pointer_to_node->word);
			free(pointer_to_node);
		}
	}
}

void find(HashTable_t* hashTable, char *word, FILE *f)
{
	int hash_position = hash(word, hashTable->size);
	Node_t* pointer_to_node = hashTable->buckets[hash_position];
	if(pointer_to_node == NULL)
	{
		fprintf(f, "False\n");
		return;
	}
	
	while(pointer_to_node && strcmp(pointer_to_node->word, word) != 0)
	{
		pointer_to_node = (Node_t*)pointer_to_node->next;
	}

	if(pointer_to_node != NULL)
		fprintf(f, "True\n");
	else
		fprintf(f, "False\n");
}

void print_bucket(HashTable_t* hashTable, int bucket_index, FILE *f)
{
	Node_t* pointer_to_node;
	if(hashTable->buckets[bucket_index] == NULL)
		return;
	pointer_to_node = hashTable->buckets[bucket_index];
	//iterate through the bucket and print all the words with a space beetwen its
	while(pointer_to_node)
	{
		fprintf(f, "%s ", pointer_to_node->word);
		pointer_to_node = (Node_t*)pointer_to_node->next;
	}
	fprintf(f, "\n");
}

void print(HashTable_t* hashTable, FILE *f)
{
	int i;
	//iterate through hashtable anf call "print_bucket" function for every bucket
	for(i = 0; i < hashTable->size; i++)
	{
		print_bucket(hashTable, i, f);
	}
}

void copy(HashTable_t *from_hashTable, HashTable_t **to_hashTable)
{
	int i;
	//iterate through hashtable and for every node from every bucket call the "add" function 
	for(i = 0; i < from_hashTable->size; i++)
	{
		Node_t* pointer_to_node = from_hashTable->buckets[i];
		while(pointer_to_node)
		{
			add(*to_hashTable, pointer_to_node->word);
			pointer_to_node = (Node_t*)pointer_to_node->next;
		}
	}

	//at the end, free the old hashtable
	free_hashTable(&from_hashTable);
}

void resize_double(HashTable_t** hashTable)
{
	//create a empty hashtable and copy the old one in the new one by calling the "copy" function
	//the new size will be doubled
	HashTable_t* new_hashTable = create_hashTable((*hashTable)->size * 2);
	copy(*hashTable, &new_hashTable);
	*hashTable = new_hashTable;
}

void resize_halve(HashTable_t** hashTable)
{
	//create a empty hashtable and copy the old one in the new one by calling the "copy" function
	//the new size will be half of the old one
	HashTable_t* new_hashTable = create_hashTable((*hashTable)->size / 2);
	copy(*hashTable, &new_hashTable);
	*hashTable = new_hashTable;
}

char **parse_line(char *line, int *counter)
{
	char *word;
	char **words = (char**)calloc(MAX_WORDS,  sizeof(char*));
	if(!words)
	{
		printf("Error allocating memory\n");
		return 0;
	}
	word = strtok(line, " \n\r");
	*counter = 0;
	while(word != NULL)
	{
		words[*counter] = (char*)calloc((strlen(word) + 1), sizeof(char));
		if(!words[*counter])
		{
			printf("Error allocating memory\n");
			return 0;
		}
		strcpy (words[*counter], word);
		(*counter)++;
		word = strtok(NULL, " \n\r");
	}

	return words;
}

void read_and_parse(HashTable_t **hashTable, FILE *input)
{
	char buffer[20000];
	char **words;
	int no_words;
	int i;
	FILE *output;

	//while it can be read a new line, parse the line and call the right function
	while(fgets(buffer, 20000, input))
	{		
		//printf("%s", buffer);
		words = parse_line(buffer, &no_words);
		if(no_words == 0)
			continue;
		if(strcmp(words[0], "add") == 0)
		{
			add((*hashTable), words[1]);
		}
		else if(strcmp(words[0], "remove") == 0)
		{
			remove_word((*hashTable), words[1]);
		}
		else if(strcmp(words[0], "clear") == 0)
		{
			clear((*hashTable));
		}
		else if(strcmp(words[0], "find") == 0)
		{
			if(no_words == 3)
			{
				output = fopen(words[2], "a");
				if(!output)
				{
					printf("Error opening file '%s'\n", words[2]);
				}
			}
			else
				output = stdout;
			find((*hashTable), words[1], output);
			fprintf(output, "\n");

			if(no_words == 3)
				fclose(output);
		}
		else if(strcmp(words[0], "print_bucket") == 0)
		{
			if(no_words == 3)
			{
				output = fopen(words[2], "a");
				if(!output)
				{
					printf("Error opening file '%s'\n", words[2]);
				}
			}
			else
				output = stdout;
			print_bucket((*hashTable), atoi(words[1]), output);
			fprintf(output, "\n");
			if(no_words == 3)
				fclose(output);
		}
		else if(strcmp(words[0], "print") == 0)
		{
			if(no_words == 2)
			{
				output = fopen(words[1], "a");
				if(!output)
				{
					printf("Error opening file '%s'\n", words[1]);
				}
			}
			else output = stdout;
			
			print((*hashTable), output);
			fprintf(output, "\n");

			if(no_words == 2)
				fclose(output);
		}
		else if(strcmp(words[0], "resize") == 0)
		{
			if(strcmp(words[1], "double") == 0)
			{
				resize_double(hashTable);
			}
			else if (strcmp(words[1], "halve") == 0)
			{
				resize_halve(hashTable);
			}
		}
		
		/*for(i = 0; i < MAX_WORDS; i++)
		{
			if(words[i])
				free(words[i]);
		}
		free(words);*/
	}
}


int main(int argc,char *argv[])
{
	HashTable_t* hashTable;
	int size, i;
	FILE* input;
	int a;
	if(argc < 2)
	{
		printf("The program is executed with too few argumentes! \n");
		return 0;
	}

	size = atoi(argv[1]);
	hashTable = create_hashTable(size);
	
	if(argc > 2)
	{
		for(i = 2; i < argc; i++)
		{
			input = fopen(argv[i], "r");
			if(!input)
			{
				printf("Error opening file '%s'\n", argv[i]);
				return 0;
			}
			//call the function that will interprate the commands from input file
			read_and_parse(&hashTable, input);
			fclose(input);
		}
	}
	else
	{
		//call the function that will interprate the commands from standard input
		read_and_parse(&hashTable, stdin);
	}

	//free the hashtable
	free_hashTable(&hashTable);
	return 1;
}