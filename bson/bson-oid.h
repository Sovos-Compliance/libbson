/*
 * Copyright 2013 10gen Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#if !defined (BSON_INSIDE) && !defined (BSON_COMPILATION)
#error "Only <bson/bson.h> can be included directly."
#endif


#ifndef BSON_OID_H
#define BSON_OID_H


#include <time.h>

#include "bson-context.h"
#include "bson-macros.h"
#include "bson-types.h"


BSON_BEGIN_DECLS


/**
 * bson_oid_compare:
 * @oid1: The bson_oid_t to compare as the left.
 * @oid2: The bson_oid_t to compare as the right.
 *
 * A qsort() style compare function that will return less than zero
 * if @oid1 is less than @oid2, zero if they are the same, and greater
 * than zero if @oid2 is greater than @oid1.
 *
 * Returns: A qsort() style compare integer.
 */
int
bson_oid_compare (const bson_oid_t *oid1,
                  const bson_oid_t *oid2);


/**
 * bson_oid_copy:
 * @src: A bson_oid_t to copy from.
 * @dst: A bson_oid_t to copy to.
 *
 * Copies the contents of @src to @dst.
 */
void
bson_oid_copy (const bson_oid_t *src,
               bson_oid_t       *dst);


/**
 * bson_oid_equal:
 * @oid1: A bson_oid_t.
 * @oid2: A bson_oid_t.
 *
 * Compares for equality of @oid1 and @oid2. If they are equal, then
 * TRUE is returned, otherwise FALSE.
 *
 * Returns: A boolean indicating the equality of @oid1 and @oid2.
 */
bson_bool_t
bson_oid_equal (const bson_oid_t *oid1,
                const bson_oid_t *oid2);


/**
 * bson_oid_is_valid:
 * @str: A string to validate.
 * @length: The length of @str.
 *
 * Validates that @str is a valid OID string. @length MUST be 12, but is
 * provided as a parameter to simplify calling code.
 *
 * Returns: TRUE if @str can be passed to bson_oid_init_from_string().
 */
bson_bool_t
bson_oid_is_valid (const char *str,
                   size_t      length);


/**
 * bson_oid_get_time_t:
 * @oid: A bson_oid_t.
 *
 * Fetches the time for which @oid was created.
 *
 * Returns: A time_t.
 */
time_t
bson_oid_get_time_t (const bson_oid_t *oid);


/**
 * bson_oid_hash:
 * @oid: A bson_oid_t to hash.
 *
 * Hashes the bytes of the provided bson_oid_t using DJB hash.
 * This allows bson_oid_t to be used as keys in a hash table.
 *
 * Returns: A hash value corresponding to @oid.
 */
bson_uint32_t
bson_oid_hash (const bson_oid_t *oid);


/**
 * bson_oid_init:
 * @oid: A bson_oid_t.
 *
 * Generates bytes for a new bson_oid_t and stores them in @oid. The bytes
 * will be generated according to the specification and includes the current
 * time, first 3 bytes of MD5(hostname), pid (or tid), and monotonic counter.
 */
void
bson_oid_init (bson_oid_t     *oid,
               bson_context_t *context);


/**
 * bson_oid_init_from_data:
 * @oid: A bson_oid_t to initialize.
 * @bytes: A 12-byte buffer to copy into @oid.
 *
 * Initializes an @oid from @data. @data MUST be a buffer of at least 12 bytes.
 * This method is analagous to memcpy()'ing data into @oid.
 */
void
bson_oid_init_from_data (bson_oid_t         *oid,
                         const bson_uint8_t *data);


/**
 * bson_oid_init_from_string:
 * @oid: A bson_oid_t
 * @str: A string containing at least 24 characters.
 *
 * Parses @str containing hex formatted bytes of an object id and places
 * the bytes in @oid.
 */
void
bson_oid_init_from_string (bson_oid_t *oid,
                           const char *str);


/**
 * bson_oid_init_sequence:
 * @oid: A bson_oid_t.
 * @context: A bson_context_t.
 *
 * Initializes @oid with the next oid in the sequence. The first 4 bytes
 * contain the current time and the following 8 contain a 64-bit integer
 * in big-endian format.
 *
 * The bson_oid_t generated by this function is not guaranteed to be globally
 * unique. Only unique within this context. It is however, guaranteed to be
 * sequential.
 */
void
bson_oid_init_sequence (bson_oid_t     *oid,
                        bson_context_t *context);


/**
 * bson_oid_to_string:
 * @oid: A bson_oid_t.
 * @str: A location to store the resulting string.
 *
 * Formats a bson_oid_t into a string. @str must contain enough bytes
 * for the resulting string which is 25 bytes with a terminating NUL-byte.
 */
void
bson_oid_to_string (const bson_oid_t *oid,
                    char              str[static 25]);


/**
 * bson_oid_compare_unsafe:
 * @oid1: A bson_oid_t.
 * @oid2: A bson_oid_t.
 *
 * Performs a qsort() style comparison between @oid1 and @oid2.
 *
 * This function is meant to be as fast as possible and therefore performs
 * no argument validation. That is the callers responsibility.
 *
 * Returns: An integer < 0 if @oid1 is less than @oid2. Zero if they are equal.
 *          An integer > 0 if @oid1 is greater than @oid2.
 */
static BSON_INLINE int
bson_oid_compare_unsafe (const bson_oid_t *oid1,
                         const bson_oid_t *oid2)
{
   return memcmp(oid1, oid2, sizeof *oid1);
}


/**
 * bson_oid_equal_unsafe:
 * @oid1: A bson_oid_t.
 * @oid2: A bson_oid_t.
 *
 * Checks the equality of @oid1 and @oid2.
 *
 * This function is meant to be as fast as possible and therefore performs
 * no checks for argument validity. That is the callers responsibility.
 *
 * Returns: TRUE if @oid1 and @oid2 are equal; otherwise FALSE.
 */
static BSON_INLINE bson_bool_t
bson_oid_equal_unsafe (const bson_oid_t *oid1,
                       const bson_oid_t *oid2)
{
   return !memcmp(oid1, oid2, sizeof *oid1);
}

/**
 * bson_oid_hash_unsafe:
 * @oid: A bson_oid_t.
 *
 * This function performs a DJB style hash upon the bytes contained in @oid.
 * The result is a hash key suitable for use in a hashtable.
 *
 * This function is meant to be as fast as possible and therefore performs no
 * validation of arguments. The caller is responsible to ensure they are
 * passing valid arguments.
 *
 * Returns: A bson_uint32_t containing a hash code.
 */
static BSON_INLINE bson_uint32_t
bson_oid_hash_unsafe (const bson_oid_t *oid)
{
   bson_uint32_t hash = 5381;
   bson_uint32_t i;

   for (i = 0; i < sizeof oid->bytes; i++) {
      hash = ((hash << 5) + hash) + oid->bytes[i];
   }

   return hash;
}


/**
 * bson_oid_copy_unsafe:
 * @src: A bson_oid_t to copy from.
 * @dst: A bson_oid_t to copy into.
 *
 * Copies the contents of @src into @dst. This function is meant to be as
 * fast as possible and therefore performs no argument checking. It is the
 * callers responsibility to ensure they are passing valid data into the
 * function.
 */
static BSON_INLINE void
bson_oid_copy_unsafe (const bson_oid_t *src,
                      bson_oid_t       *dst)
{
   memcpy(dst, src, sizeof *src);
}


/**
 * bson_oid_parse_hex_char:
 * @hex: A character to parse to its integer value.
 *
 * This function contains a jump table to return the integer value for a
 * character containing a hexidecimal value (0-9, a-f, A-F). If the character
 * is not a hexidecimal character then zero is returned.
 *
 * Returns: An integer between 0 and 15.
 */
static BSON_INLINE bson_uint8_t
bson_oid_parse_hex_char (char hex)
{
   switch (hex) {
   case '0': return 0;
   case '1': return 1;
   case '2': return 2;
   case '3': return 3;
   case '4': return 4;
   case '5': return 5;
   case '6': return 6;
   case '7': return 7;
   case '8': return 8;
   case '9': return 9;
   case 'a': case 'A': return 0xa;
   case 'b': case 'B': return 0xb;
   case 'c': case 'C': return 0xc;
   case 'd': case 'D': return 0xd;
   case 'e': case 'E': return 0xe;
   case 'f': case 'F': return 0xf;
   default: return 0;
   }
}


/**
 * bson_oid_init_from_string_unsafe:
 * @oid: A bson_oid_t to store the result.
 * @str: A 24-character hexidecimal encoded string.
 *
 * Parses a string containing 24 hexidecimal encoded bytes into a bson_oid_t.
 * This function is meant to be as fast as possible and inlined into your
 * code. For that purpose, the function does not perform any sort of bounds
 * checking and it is the callers responsibility to ensure they are passing
 * valid input to the function.
 */
static BSON_INLINE void
bson_oid_init_from_string_unsafe (bson_oid_t *oid,
                                  const char *str)
{
   int i;

   for (i = 0; i < 12; i++) {
      oid->bytes[i] = ((bson_oid_parse_hex_char(str[2*i]) << 4) |
                       (bson_oid_parse_hex_char(str[2*i+1])));
   }
}


/**
 * bson_oid_get_time_t_unsafe:
 * @oid: A bson_oid_t.
 *
 * Fetches the time @oid was generated.
 *
 * Returns: A time_t containing the UNIX timestamp of generation.
 */
static BSON_INLINE time_t
bson_oid_get_time_t_unsafe (const bson_oid_t *oid)
{
   bson_uint32_t t;
   memcpy(&t, oid, 4);
   return BSON_UINT32_FROM_BE(t);
}


BSON_END_DECLS


#endif /* BSON_OID_H */
