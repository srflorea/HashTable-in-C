#include "tema0.h"

/*Function that creates a new node in order to be putted in hashtable.
*It returns the new node.
*/
Node_t* create_node(char* word);

/*Function that destroies a hashtable.
* It frees all the allocated memory for this hashtable.
*/
void free_hashTable(HashTable_t** hashTable);

/*Function that creates a new hashtable.
* It allocs memory for 'size' bucketes, number which is the parameter for
* this function.
*/
HashTable_t* create_hashTable(unsigned int size);

/*Function that creates a copy of a hashtable.
*/
void copy(HashTable_t *from_hashTable, HashTable_t **to_hashTable);

/*Function that receives a line readed from input and returns
* an array with the words from that line.
* It also returns the number of finded words.
*/
char **parse_line(char *line, int *counter);

/*Function that will interpret the commands and will call the right functions.
*/
void read_and_parse(HashTable_t **hashTable, FILE *input);