#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {word_len = 6};
char* readline();
char** split_string(char*);

struct node {
    struct node *next;
    char word[word_len];
    int count;
};

/**
 * @brief inserts a word from the magazine to a linked list.
 * @param head is a pointer to the first node of the linked list.
 * @param input_word a pointer to the word we add to the linked list.
 * @return 0 if the insert was a success or 1 if it failed.
 */
int insert_word(struct node *head , char *input_word) {
	struct node *current_node = head;
	/* search if word already exists */
	while (current_node->next) {
		/* if the word already appears add 1 to the counter */
		if(strcmp(current_node->word, input_word) == 0) {
			current_node->count=current_node->count+1;	
			return 0;
		}
		current_node=current_node->next;
	} 
	struct node *new_node = malloc(sizeof(struct node) * 1);
	/* check if malloc worked */
	if(new_node == NULL) {
		return 1;
	}
	/* if the word is new add it to the linked list end */
	strcpy(new_node->word, input_word);
	new_node->count=1;
	current_node->next=new_node;
	new_node->next=NULL;
	return 0;
}

/**
 * @brief checks if the word exists in linked list and if we havent used it yet.
 * @param head is a pointer to the first node of the linked list.
 * @param input_word a pointer to the word we want to check.
 * @return 0 if the word was available to use or 1 if it was not.
 */
int check_word(struct node *head , char *input_word) {
	struct node *current_node = head;
	/* as long as you didn't find the word, contine to the next */
	while (strcmp(current_node->word, input_word) != 0) {
		/* if we finished checking all the linked list */
		if(!current_node->next) {
			return 1;			
		}
		current_node=current_node->next;
	}
	/* checks if the word we found was used up */
	if(current_node->count == 0) {
		return 1;
	}
	/* update the counter */
	current_node->count=current_node->count-1;		
	return 0;
}

/**
 * @brief free the memmory for all the nodes from the linked list.
 * @param head is a pointer to the first node of the linked list.
 */
void list_destroy(struct node *head) {
	struct node *cursor; 
	struct node *cursor_copy; 
	cursor = head;
	/* free all the nodes on the linked list, starting with the head */
	while (cursor) { 
		cursor_copy = cursor; 
		cursor = cursor->next;
		free(cursor_copy);
	}
}

/**
 * @brief checks if all the words in the note exists also in the magazine.
 * @param magazine_count is the number of words in the magazine.
 * @param magazine is an array of words in the magazine.
 * @param note_count is the number of words in the note.
 * @param note is an array of words in the not.
 * @return 0 if all words in note exists in magazine or 1 if not.
 * @note case sensitive and number of repetition counts.
 */
void checkMagazine(int magazine_count, 
				   char** magazine, 
				   int note_count, 
				   char** note) {
	/* check if there are enough words in the magazine */
	if(magazine_count<note_count) {
	    printf("No");
		exit(1);
	}
	/* create the first node */
	struct node *head = malloc(sizeof(struct node) * 1);
	head->next=NULL;
	strcpy(head->word, *(magazine));
	head->count=1;
	/* insert all the words in the magazine to a linked list */
	for (int i=1; i<magazine_count; i++) {
		/* checks if insert_word failed */
		if(insert_word(head, *(magazine + i)) == 1){
			exit(1); 
		}		
    }
	/* checks if all the words in the note apeared in the linked list */
	for (int i = 0; i < note_count; i++) {
		/* checks if word was missing */
	    if(check_word(head, *(note + i)) == 1){
			printf("No");
			exit(1);  
		}		
    }
	printf("Yes");
	/* free all the nodes we created */
	list_destroy(head);
}

int main()
{
    char** mn = split_string(readline());

    char* m_endptr;
    char* m_str = mn[0];
    int m = strtol(m_str, &m_endptr, 10);

    if (m_endptr == m_str || *m_endptr != '\0') { exit(EXIT_FAILURE); }

    char* n_endptr;
    char* n_str = mn[1];
    int n = strtol(n_str, &n_endptr, 10);

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    char** magazine_temp = split_string(readline());

    char** magazine = malloc(m * sizeof(char*));

    for (int i = 0; i < m; i++) {
        char* magazine_item = *(magazine_temp + i);

        *(magazine + i) = magazine_item;
    }

    int magazine_count = m;

    char** note_temp = split_string(readline());

    char** note = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        char* note_item = *(note_temp + i);

        *(note + i) = note_item;
    }

    int note_count = n;

    checkMagazine(magazine_count, magazine, note_count, note);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!line) {
            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);
    } else {
        data = realloc(data, data_length + 1);

        data[data_length] = '\0';
    }

    return data;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}
