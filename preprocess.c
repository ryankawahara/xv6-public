#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


const int MAX_DEFINES = 10;
const int MAX_VAR_LEN = 50;
const int MAX_VAL_LEN = 512;


char defined_vars[10][50];
char defined_vals[10][512]; 
int define_count = 0; 




int fd;

// Holds word and word that it should be replaced with
struct ReplaceWordPair {
    char var[50];
    char val[50];
};

// compares strings
int my_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

//Matches gcc in ignoring cyclical replacements
void check_no_cycles(struct ReplaceWordPair replaceList[], int replaceListSize, struct ReplaceWordPair finalReplaceList[], int *finalReplaceListSize) {
   
    *finalReplaceListSize = 0;
    int i, j;

    for (i = 0; i < replaceListSize; i++) {
        int isCycle = 0;


        for (j = 0; j < replaceListSize; j++) {
            if (i == j) continue;


            if (my_strcmp(replaceList[i].var, replaceList[j].val) == 0 && my_strcmp(replaceList[i].val, replaceList[j].var) == 0) {
                isCycle = 1;
                break;
            }
        }


        if (!isCycle) {
            finalReplaceList[*finalReplaceListSize] = replaceList[i];
            (*finalReplaceListSize)++;  
        }
    }
}


// Checks if character is delimiter
int is_delimiter(char c) {
    // Treat punctuation marks, spaces, and end of string as delimiters
    return (c == ' ' || c == '\0' || c == '.' || c == ',' ||
            c == '!' || c == '?' || c == ';' || c == ':' ||
            c == '\n' || c == '\t');
}
// Custom implementation of strlen: calculates the length of a string
int my_strlen(const char *s) {
    int len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}


// Custom implementation of strcpy: copies one string into another
char* my_strcpy(char *dest, const char *src) {
    char *d = dest;
    while ((*d++ = *src++) != '\0');
    return dest;
}


// Custom implementation of strcat: concatenates two strings
char* my_strcat(char *dest, const char *src) {
    char *d = dest + my_strlen(dest);  // Move to the end of the dest string
    while ((*d++ = *src++) != '\0');  // Copy src to dest
    return dest;
}


// Custom implementation of strchr: finds the first occurrence of a character in a string
char* my_strchr(const char *s, int c) {
    while (*s) {
        if (*s == (char)c) {
            return (char*)s;
        }
        s++;
    }
    return 0;
}


// Custom implementation of strstr: finds the first occurrence of a substring
char* my_strstr(const char *haystack, const char *needle) {
    int needle_len = my_strlen(needle);
    if (needle_len == 0) {
        return (char*)haystack;
    }
    int i;
   
    for (i=0; haystack[i] != '\0'; i++) {
        int j = 0;
        while (haystack[i + j] == needle[j] && needle[j] != '\0') {
            j++;
        }
        if (needle[j] == '\0') {
            return (char*)&haystack[i];
        }
    }
    return 0;
}

// Replace word for #define
void replace_word(char *content, const char *var, const char *val) {
    char buffer[512];
    char *pos;
    int var_len = my_strlen(var);

    while ((pos = my_strstr(content, var)) != 0) {
        int i = 0;
        while (content + i != pos) {
            buffer[i] = content[i];
            i++;
        }
        buffer[i] = '\0';


        my_strcat(buffer, val);
        my_strcat(buffer, pos + var_len);


        my_strcpy(content, buffer);
    }
}

// Used for -D flags
void replace_variables(char *content, struct ReplaceWordPair replacements[], int num_replacements) {
    char buffer[512]; 
    char *pos;         
    int i, j;

    for (i = 0; i < num_replacements; i++) {
        while ((pos = my_strstr(content, replacements[i].var)) != 0) {
            int var_len = my_strlen(replacements[i].var);

            if ((pos == content || is_delimiter(*(pos - 1))) && is_delimiter(*(pos + var_len))) {
                int len_before = pos - content;

                for (j = 0; j < len_before; j++) {
                    buffer[j] = content[j];
                }
                buffer[len_before] = '\0';  

                if (my_strcmp(replacements[i].val, ".") == 0) {
                    my_strcat(buffer, replacements[i].val);
                    if (*(pos + var_len) != ' ') {
                        my_strcat(buffer, " ");
                    }
                    my_strcat(buffer, pos + var_len);  
                }
                else if (replacements[i].val[0] == '\0') {
                    my_strcat(buffer, pos + var_len);  
                }
                else {
                    my_strcat(buffer, replacements[i].val);
                    my_strcat(buffer, pos + var_len);  
                }

                my_strcpy(content, buffer);
            } else {
                content = pos + var_len;
            }
        }
    }
}


void replace_defined_vars(char *content) {
    int i;
    for (i = 0; i < define_count; i++) {
        replace_word(content, defined_vars[i], defined_vals[i]);
    }
}






void store_define(const char *var, const char *val) {
    if (define_count < 10) { 
        my_strcpy(defined_vars[define_count], var);
        my_strcpy(defined_vals[define_count], val);
        define_count++;
    } else {
        printf(1, "Error: Maximum number of defines reached\n");
    }
}


// Function to process #define directives in the file content
void process_define_directives(char *content) {
    char *pos;
    while ((pos = my_strstr(content, "#define")) != 0) {
        char var[50], val[512];
        pos += 7; // Skip the "#define" part
        while (*pos == ' ') pos++;
       
        char *var_start = pos;
        while (*pos != ' ' && *pos != '\0') pos++;
        *pos = '\0';
        my_strcpy(var, var_start);


        pos++; 
        while (*pos == ' ') pos++; 
       
        char *val_start = pos;
        while (*pos != '\n' && *pos != '\0') pos++;
        *pos = '\0';
        my_strcpy(val, val_start);
        store_define(var, val);
        char *rest = pos + 1;
        my_strcpy(my_strstr(content, "#define"), rest);
    }
}


int main(int argc, char *argv[]) {
    char buffer[512];
    int bytes_read;
    int i;
    char content[512];
    int total_length = 0;


    struct ReplaceWordPair replacements[10];
    int replacement_count = 0;

    fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        printf(1, "Error opening file %s\n", argv[1]);
        exit();
    }


    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        for (i = 0; i < bytes_read; i++) {
            content[total_length++] = buffer[i];
        }
    }
    content[total_length] = '\0';


    if (bytes_read < 0) {
        printf(1, "Error reading file\n");
        close(fd);
        exit();
    }


    process_define_directives(content);
    replace_defined_vars(content);

    for (i = 2; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == 'D') {
            char *var = argv[i] + 2;
            char *val = my_strchr(var, '=');
            if (val != 0) {
                *val = '\0';
                val++;
                my_strcpy(replacements[replacement_count].var, var);
                my_strcpy(replacements[replacement_count].val, val);
               
                replacement_count++;
            }
        }
    }


    struct ReplaceWordPair finalReplaceList[10];
    int finalReplaceListSize = 0;


    check_no_cycles(replacements, replacement_count, finalReplaceList, &finalReplaceListSize);


    replace_variables(content, finalReplaceList, finalReplaceListSize); // Replace vars with vals in content


    printf(1, "%s\n", content);


    close(fd);
    exit();
}
