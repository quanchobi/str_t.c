#include <errno.h>
#include <unistd.h>
#include <malloc.h>

#ifndef P1_H
#define P1_H

struct strstr {
    char *chars;            // points to first char in char array
    unsigned int len;   // length of the string in the array
    unsigned int cap;   // capacity of the array
};
typedef struct strstr *str_t;

/* create a new string variable containing the given char sequence */
/* precondition:  initializer is non-null */
/* precondition:  initializer points to a null-terminated string */
str_t create(char *initializer);

/* frees all memory associated with a strstr */
void destroy(str_t s);

/* return the length of the string */
/* precondition: s has been initialized (created) */
int length(str_t s);

/* return 1 if s1 and s2 represent the same string, 0 otherwise  */
/* precondition:  both arguments are non-NULL */
int equals(str_t s1, str_t s2);


/* make the contents of "to" be equal to the contents of "from".
 * Precondition: both are valid (initialized) strings.
 * Note: this is what is called a "deep copy" - subsequent changes
 * to "from" do not affect  "to".
 */
void copy(str_t to, str_t from);

/* modify the contents of the first argument by replacing every
 * character in it that also occurs in the second argument with a
 * space character (integer value \verb.0x20.).  In other words,
 * this has the same semantics as the corresponding function in Project 0.
 */
void censor(str_t orig, str_t bad);

/* return a pointer to a character array that contains
 * the same sequence of characters as s, but with the end marked by a
 * null (0) byte, according to the C convention.
 * Subsequent changes to s MAY change the returned C string, including modifying
 * the end marker.  This is intended for ephemeral, one-time use, e.g.,
 * passing to printf().
 */
char *to_chars(str_t s);

/* Extend string s1 by appending the string s2 to it.  s2 is unmodified. */
void append(str_t s1, str_t s2);

/* Return a new string of len characters equal to the len characters of s
 * beginning at position start.  The  original string s is unchanged. 
 * If start + len exceeds the actual length of the string, the 
 * returned string consists of the characters from index start to the end of s.
 * Precondition: both start and len are at least 0.
 */
str_t substring(str_t s, int start, int len);

void edit(str_t targ, str_t rpl, str_t orig);
#endif
