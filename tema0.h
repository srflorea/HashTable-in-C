

typedef struct Node {
	struct Node_t *next;
	char *word;
} Node_t;

typedef struct HashTable {
	Node_t **buckets;
	unsigned int size;
} HashTable_t;

/* Function that adds a word in the hashtable
* It doesn't allows tha same word twice
*/
void add(HashTable_t *hashTable, char *word);

/* Function that removes a word from hashtable
*  The word could doesn't exist.
*/
void remove_word(HashTable_t *hashTable, char *word);

/* Function that clears the table.
*/
void clear();

/* Function that is searching in hashtable
*  It writes 'True' or 'False' on a new line in the
*  specified output file, or at tha console if the
*  output file is missing.
*/
void find(HashTable_t* hashTable, char *word, FILE *f);

/* Function that writes the words from a bucket on
*  a line separated with space in the given output file
*  or at the console if the output file doesn't exist.
*/
void print_bucket(HashTable_t* hashTable, int bucket_index, FILE* f);

/* Function that writes all the buckets on different lines
*  starting with the bucket number 0, in the given output file
*  or at the console if that parameter is missing.
*/
void print(HashTable_t* hashTable, FILE *f);

/* Function that doubles the size of the hashtable
*  The words are redistributed in the order that were
*  introduced in hashtable.
*/
void resize_double(HashTable_t** hashTable);

/* Function that halves the size of the hashtable
*  The words are redistributed in the order that were
*  introduced in hashtable.
*  The surplus of memory is deallocated.
*/
void resize_halve(HashTable_t** hashTable);