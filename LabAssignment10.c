#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LEN 50

struct Trie *createTrie();

// Trie structure
struct Trie
{	
    struct Trie* children[26];
    int wordCount;
};

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    int len = strlen(word);

    if (pTrie == NULL)
    {
        return;
    }

    for (int i = 0; i < len; i++)
    {
        //If the child representing this letter is null
        if (pTrie->children[word[i] - 'a'] == NULL)
        {
            pTrie->children[word[i] - 'a'] = createTrie(); //Add a new node there
        }
        pTrie = pTrie->children[word[i] - 'a'];
    }

    pTrie->wordCount += 1; //Increment this node's word count
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    int len = strlen(word);

    if (pTrie == NULL)
    {
        return 0;
    }

    for (int i = 0; i < len; i++)
    {
        //If the child representing this letter is null
        if (pTrie->children[word[i] - 'a'] == NULL)
        {
            return 0; //The word isn't in the trie at all
        }
        pTrie = pTrie->children[word[i] - 'a'];
    }

    return pTrie->wordCount; //Get the word count
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    //If the tree is already null, there's nothing to free
    if (pTrie == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < 26; i++)
    {
        //If the child is already null, check the next one
        if (pTrie->children[i] == NULL)
        {
            continue;
        }
        deallocateTrie(pTrie->children[i]); //If the child isn't null yet, free it and its children
    }

    //Free the node itself
    free(pTrie);
    pTrie = NULL;

    return pTrie;
}

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie* trie = (struct Trie*)malloc(sizeof(struct Trie));
    //Initialize children to null
    for (int i = 0; i < 26; i++)
    {
        trie->children[i] = NULL;
    }
    trie->wordCount = 0;
    return trie;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE* file;
    file = fopen("dictionary.txt", "r");
    if (file == NULL)
    {
        printf("File read error!\n");
        return -1;
    }

    //Get the number of words
    int num = 0;
    fscanf(file, "%d\n", &num);

    //Shove all the words into an array
    for(int i = 0; i < num; i++)
    {
        char* word = (char*)malloc(MAX_WORD_LEN * sizeof(char));
        fscanf(file, "%s", word); 
        pInWords[i] = word;
    }

    return num;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");

    //Free inwords
    for (int i = 0; i < numWords; i++)
    {
        free(inWords[i]);
    }
	return 0;
}