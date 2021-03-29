/*
 * MIT License
 *
 * Copyright (c) 2021 Max Lichtenegger
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef JSMN_PARSER_H
#define JSMN_PARSER_H

#include "jsmn.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

/**
 * @brief Jumpt to the next JSON element.
 *
 * Objects and arrays do have sizes, but the length of each element is
 * unknown. In JSMN, all the tokens are in an array, so this function just
 * helps finding the next JSON element.
 *
 * @param tok 							Pointer to the current token. Tokens
 * 										must be ordered in an array.
 * @return const jsmntok_t* 			Pointer to the token of the next
 *										element. If no next element is found
 *										the given token is returned.
 */
const jsmntok_t* tok_next_elem(const jsmntok_t* tok);

/**
 * @brief Check if the given token is a name.
 *
 * JSMN does not specify names, however a JSMN_STRING with size one is a
 * JSON name compliant to rfc8259.
 *
 * @param tok							Pointer to the token to check. 
 * @return true 						if token points to a name.
 * @return false 						if token points to something else.
 */
bool tok_is_name(const jsmntok_t* tok);

/**
 * @brief Check if the given token is a leaf.
 * 
 * Leaf as in does not contain any nested objects or arrays.
 *
 * @param tok 							Pointer to the token to check.
 * @return true 						if oken points to a leaf.
 * @return false 						if oken points to something else.
 */
bool tok_is_leaf(const jsmntok_t* tok);

/**
 * @brief Compare the value pointed to by the token with another string.
 * 
 * Since tokens just point to substrings of a JSON string, comparison of
 * strings does not include zero terminator, i.e. length checks.
 *
 * @param tok 							Pointer to token to check.
 * @param js 							JSON string tokens point to.
 * @param value 						String value to compare to.
 * @return true 						if strings are identical. 
 * @return false 						if strings mismatch.
 */
bool tok_string_compare(const jsmntok_t* tok, const char* js, const char* value);

/**
 * @brief Get the value of a name/value pair.
 * 
 * @param tok 							Pointer to the token whose value we
 * 										want to get.
 * @return const jsmntok_t* 			Pointer to the value, returns the given
 *										token if it is a value.
 */
const jsmntok_t* tok_get_value(const jsmntok_t* tok);

/**
 * @brief Get the token at the given index.
 *
 * Works on objects and arrays and on names of these.
 * 
 * @param tok 							Pointer to the object or array
 *										(or their name).
 * @param index 						Index within the given object/array.
 * @return const jsmntok_t* 			Pointer to the token at the given
 *										index. If index is out of range, the
 *										given token is returned.
 */
const jsmntok_t* tok_get_index(const jsmntok_t* tok, int index);

/**
 * @brief Get the token with the given name.
 * 
 * Only works on objects (and their names), since they can contain name/value
 * pairs.
 *
 * @param tok							Pointer to the object (or its name).
 * @param js 							JSON string tokens point to.
 * @param name 							The name to get.
 * @return const jsmntok_t* 			Pointer to the token with the given
 *										name, returns the given token if the
 *										name could not be found.
 */
const jsmntok_t* tok_get_name(const jsmntok_t* tok, const char* js, const char* name);

/**
 * @brief Get the token with the value for the given name, if it is a leaf.
 * 
 * Convenience function combining tok_get_name and tok_get_value. Also checks
 * if returned value is a leaf.
 * 
 * @param tok 							Pointer to the object (or its name).
 * @param js 							JSON string tokens point to.
 * @param name 							The name whose value to get.
 * @return const jsmntok_t* 			Pointer to the token of the value with
 *										the given name, returns the given token
 *										if the name could not be found or the
 *										value is not a leaf.
 */
const jsmntok_t* tok_get_leaf(const jsmntok_t* tok, const char* js, const char* name);

/*****************************************************************************/
/* Functions                                                                 */
/*****************************************************************************/
const jsmntok_t* tok_next_elem(const jsmntok_t* tok)
{
	const jsmntok_t* start_tok = tok;
	// Add ourselves
	const jsmntok_t* end_tok = ++tok;

	int layers = start_tok->size;

	// TODO: Make sure we have a way out of the recursion
	for (int i = 0; i < layers; i++)
	{
		end_tok = tok_next_elem(end_tok);
	}

	return end_tok;
}

bool tok_is_name(const jsmntok_t* tok)
{
	return (tok->type == JSMN_STRING && tok->size == 1);
}

bool tok_is_value(const jsmntok_t* tok)
{
	return (tok->size == 0);
}

bool tok_string_compare(const jsmntok_t* tok, const char* js, const char* value)
{
	bool same = false;
	const char* start = js + tok->start;
	const char* end = js + tok->end;

	while (start != end)
	{
		if (*start != *value)
		{
			return same;
		}
		start++;
		value++;
	}

	same = true;

	return same;
}

const jsmntok_t* tok_get_value(const jsmntok_t* tok)
{
	const jsmntok_t* value = tok;
	
	if (tok_is_name(tok))
	{
		++value;
	}

	return value;
}

const jsmntok_t* tok_get_index(const jsmntok_t* tok, int index)
{
	const jsmntok_t* tok_at_index = tok;
	
	// Only applicable to objects and arrays (or names of those)
	if (tok_get_value(tok)->type == JSMN_OBJECT || tok_get_value(tok)->type == JSMN_ARRAY)
	{
		if (tok_get_value(tok)->size > index)
		{
			// Workaround for names
			if (tok_is_name(tok))
			{
				++tok_at_index;
			}
			// Start at the next token
			++tok_at_index;
			for (int curr_index = 0; curr_index < index; curr_index++)
			{
				tok_at_index = tok_next_elem(tok_at_index);
			}
		}
	}

	return tok_at_index;
}

const jsmntok_t* tok_get_name(const jsmntok_t* tok, const char* js, const char* name)
{
	const jsmntok_t* tok_with_name = tok;

	// Only applicable to objects (and keys of objects)
	if (tok_get_value(tok)->type == JSMN_OBJECT)
	{
		// Workaround for keys
		if (tok_is_name(tok))
		{
			++tok_with_name;
		}
		// Start at the next token
		++tok_with_name;
		for (int curr_index = 0; curr_index < tok_get_value(tok)->size; curr_index++)
		{
			// Sanity check
			if (!tok_is_name(tok_with_name))
			{
				return tok;
			}
			if (tok_string_compare(tok_with_name, js, name))
			{
				return tok_with_name;
			}
			tok_with_name = tok_next_elem(tok_with_name);
		}
	}

	return tok;
}

const jsmntok_t* tok_get_leaf(const jsmntok_t* tok, const char* js, const char* name)
{
	const jsmntok_t* tok_with_name = tok_get_name(tok, js, name);

	// Check if key found
	if (tok_with_name != tok)
	{
		const jsmntok_t* tok_with_value = tok_get_value(tok_with_name);
		if (tok_is_value(tok_with_value))
		{
			return tok_with_value;
		}
	}

	return tok;
}

#ifdef __cplusplus
}
#endif

#endif /* JSMN_PARSER_H */