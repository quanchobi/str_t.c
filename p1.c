// Jacob Hanks
// CS270 - Systems Programming
// Dr Calvert, University of Kentucky
// 2 Apr 2023
#include "p1.h"

#define STRSIZE 64 // number of chars to store in a default size str_t. Will automatically allocate more memory as needed for larger strings.

/* A simple death message */
void die_with_error(char *msg, int retval) {
    perror(msg);
    _exit(retval);
}

/* Wrapper function for malloc */
void *Malloc(size_t size) {
    void *ptr;
    if ((ptr = malloc(size)) < 0)
        die_with_error("malloc", 1);
    return ptr;
}

/* Wrapper function for realloc */
void *Realloc(void *oldptr, size_t size) {
    void *newptr;
    if ((newptr = realloc(oldptr, size)) < 0)
        die_with_error("realloc", 2);
    return newptr;
}

/* create a new string variable containing the given char sequence */
/* precondition:  initializer is non-null */
/* precondition:  initializer points to a null-terminated string */
str_t create(char *initializer) {
    // dynamically allocate the struct
    str_t str = Malloc(sizeof(struct strstr));

    // find len of initializer char array
    int len = 0;
    while (*(initializer + len))
        len++;

    // defining string capactiy as a constant multiple of STRSIZE as necessary by the length of the char array (by default 64 bytes)
    str->cap = (len / STRSIZE + 1) * STRSIZE;
    str->chars = Malloc(str->cap * sizeof(char));   // including sizeof(char) for portability (even though char is 1 byte on every system that I am aware of, better to be safe than sorry.)
    str->len = len;

    // copies initializer char array to string
    for (int i = 0; i < str->len; i++)
        *(str->chars + i) = *(initializer + i);

    return str;
}

/* 
 * Frees all memory associated with a strstr
 * Precondition: Takes a strstr as argument. Assumes strstr was created using create.
 */
void destroy(str_t s) {
    /* Free the character string that was malloc'd */
    free(s->chars);

    /* Free the memory allocated to the str_t */
    free(s);
}

/* return the length of the string */
/* precondition: s has been initialized (created) */
int length(str_t s) {
    // return len
    return s->len;
}

/* return 1 if s1 and s2 represent the same string, 0 otherwise  */
/* precondition:  both arguments are non-NULL */
int equals(str_t s1, str_t s2) {
    // ensuring s1 and s2 are the same length
    if (s1->len != s2->len)
        return 0;

    // checking char by char to ensure equality
    for (int i = 0; (i < s1->len) && (i < s2->len); i++) {
        if (*(s1->chars + i) != *(s2->chars + i))
            return 0;
    }
    return 1;
}

/* make the contents of "to" be the contents of "from" */
/* precondition: both are valid (initialized) strings */
void copy(str_t to, str_t from) {
    // reallocating memory if to does not have enough capactiy
    if (from->len > to->cap) {
        to->cap = from->cap;
        to->chars = Realloc(to->chars, to->cap * sizeof(char));
    }

    // updating len of to
    to->len = from->len;

    // copying over chars into to from from
    for (int i = 0; i < from->len; i++)
        *(to->chars + i) = *(from->chars + i);
}

/* modify the contents of the first argument by replacing every
 * character in it that also occurs in the second argument with a
 * space character (integer value \verb.0x20.).  In other words,
 * this has the same semantics as the corresponding function in Project 0.
 */
void censor(str_t orig, str_t bad) {
    // iterate over entire string
    for (int i = 0; i < orig->len; i++) {
        // compare the current char in orig with all of the bad chars
        for (int j = 0; j < bad->len; j++) {
            // if char in orig is bad
            if (*(orig->chars + i) == *(bad->chars + j)) {
                // replace the bad character in orig with space
                *(orig->chars + i) = ' ';
                // no need to check the char in orig further
                break;
            }
        }
    }
}

/* return a pointer to a character array that contains
 * the same sequence of characters as s, but with the end marked by a
 * null (0) byte, according to the C convention.
 * Subsequent changes to s may change the returned C string, including modifying
 * the end marker.  This is intended for ephemeral, one-time use, e.g.,
 * passing to printf().
 */
char *to_chars(str_t s) {
    // null terminate string
    *(s->chars + s->len) = '\0';
    // return pointer to char array in str_t
    return s->chars;
}

/* Extend string s1 by appending the string s2 to it.  s2 is unmodified. */
void append(str_t s1, str_t s2) {
    // reallocate strings capacity
    if (s1->len + s2->len > s1->cap) {
        s1->cap = ((s1->len + s2->len) / STRSIZE + 1) * STRSIZE;
        s1->chars = Realloc(s1->chars, s1->cap * sizeof(char));
    }

    // appending algorithm. Simple for loop.
    for (int i = 0; i < s2->len; i++) {
        *(s1->chars + s1->len) = *(s2->chars + i);
        s1->len++;
    }
}

/* Return a new string of len characters equal to the len characters of s
 * beginning at position start.  The  original string s is unchanged. 
 * If start + len exceeds the actual length of the string, the 
 * returned string consists of the characters from index start to the end of s.
 * Precondition: both start and len are at least 0.
 */
str_t substring(str_t s, int start, int len) {
    // if start is out of bounds
    if (start > s->len)
        return create("");
    // I wanted to use something akin to return create(to_chars(s) + start); here, as I feel that there is a lot of repeated code in substring,
    //  but could not figure out how to stop the substr at length without modifying s or using more memory than was necessary by malloc'ing
    //  another temporary char array.

    // allocating memory for a new str
    str_t substr = Malloc(sizeof(struct strstr));

    // if the desired len of the substr goes out of bounds, make len + start equal to the length of the char array
    if (start + len > s->len)
        len = s->len - start;
    substr->len = len;

    // allocating memory for the new char array
    substr->cap = (len / STRSIZE + 1) * STRSIZE;            // capacity of new substr is smallest multiple of 64 bytes that has room for all chars
    substr->chars = Malloc(substr->cap * sizeof(char));     // allocating needed memory to char array

    // add chars into substr
    for (int i = 0; i < len; i++)
        *(substr->chars + i) = *(s->chars + start + i);
    return substr;
}

/* Edits a str_t orig by removing all instances of targ and replacing them with rpl 
 * Precondition: targ and orig must not be empty, rpl can be any valid string.
 * Postcondition: modifies data in orig 
 */
void edit(str_t targ, str_t rpl, str_t orig) {
    /* If targ or orig is empty or any input (somehow) has negative len, do nothing */
    if (length(targ) < 1 || length(rpl) < 0 || length(orig) < 1)
        return;

    /* Scan orig for targ */
    for (int i = 0; i < orig->len - targ->len + 1; i++) {
        /* Make a substring of the original string with the length of the target length */
        str_t substr = substring(orig, i, targ->len);
        if (equals(targ, substr)) {
            /* Make a substring of the characters before and after the target */
            str_t first = substring(orig, 0, i);
            str_t last = substring(orig, i + targ->len, orig->len);

            /* Append to make final str_t */
            append(first, rpl);
            append(first, last);

            /* Copying final str_t into orig */
            copy(orig, first);

            /* Free first and last */
            destroy(first);
            destroy(last);
        }
        /* Freeing allocated memory every loop */
        destroy(substr);
    }
}
