#ifndef JSMN_PARSER_H
#define JSMN_PARSER_H

#include "jsmn.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

const jsmntok_t* tok_find_end(const jsmntok_t* tok);
bool tok_is_key(const jsmntok_t* tok);
bool tok_is_value(const jsmntok_t* tok);
bool tok_string_compare(const jsmntok_t* tok, const char* js, const char* key);
int tok_get_size(const jsmntok_t* tok);
const jsmntok_t* tok_get_value(const jsmntok_t* tok);
const jsmntok_t* tok_get_index(const jsmntok_t* tok, int index);
const jsmntok_t* tok_get_key(const jsmntok_t* tok, const char* js, const char* key);
const jsmntok_t* tok_get_leaf(const jsmntok_t* tok, const char* js, const char* key);

const jsmntok_t* tok_find_end(const jsmntok_t* tok)
{
	const jsmntok_t* start_tok = tok;
	// Add ourselves
	const jsmntok_t* end_tok = ++tok;

	int layers = start_tok->size;

	for (int i = 0; i < layers; i++)
	{
		end_tok = tok_find_end(end_tok);
	}

	return end_tok;
}

bool tok_is_key(const jsmntok_t* tok)
{
	return (tok->type == JSMN_STRING && tok->size == 1);
}

bool tok_is_value(const jsmntok_t* tok)
{
	return (tok->size == 0);
}

bool tok_string_compare(const jsmntok_t* tok, const char* js, const char* key)
{
	bool same = false;
	const char* start = js + tok->start;
	const char* end = js + tok->end;

	while (start != end)
	{
		if (*start != *key)
		{
			return same;
		}
		start++;
		key++;
	}

	same = true;

	return same;
}

int tok_get_size(const jsmntok_t* tok)
{
	const jsmntok_t* end = tok_find_end(tok);
	return end - tok;
}

const jsmntok_t* tok_get_value(const jsmntok_t* tok)
{
	const jsmntok_t* value = tok;
	
	if (tok_is_key(tok))
	{
		++value;
	}

	return value;
}

const jsmntok_t* tok_get_index(const jsmntok_t* tok, int index)
{
	const jsmntok_t* tok_at_index = tok;
	
	// Only applicable to objects and arrays (or keys of those)
	if (tok_get_value(tok)->type == JSMN_OBJECT || tok_get_value(tok)->type == JSMN_ARRAY)
	{
		if (tok_get_value(tok)->size > index)
		{
			// Workaround for keys
			if (tok_is_key(tok))
			{
				++tok_at_index;
			}
			// Start at the next token
			++tok_at_index;
			for (int curr_index = 0; curr_index < index; curr_index++)
			{
				tok_at_index = tok_find_end(tok_at_index);
			}
		}
	}

	return tok_at_index;
}

const jsmntok_t* tok_get_key(const jsmntok_t* tok, const char* js, const char* key)
{
	const jsmntok_t* tok_with_key = tok;

	// Only applicable to objects (and keys of objects)
	if (tok_get_value(tok)->type == JSMN_OBJECT)
	{
		// Workaround for keys
		if (tok_is_key(tok))
		{
			++tok_with_key;
		}
		// Start at the next token
		++tok_with_key;
		for (int curr_index = 0; curr_index < tok_get_value(tok)->size; curr_index++)
		{
			// Sanity check
			if (!tok_is_key(tok_with_key))
			{
				return tok;
			}
			if (tok_string_compare(tok_with_key, js, key))
			{
				return tok_with_key;
			}
			tok_with_key = tok_find_end(tok_with_key);
		}
	}

	return tok;
}

const jsmntok_t* tok_get_leaf(const jsmntok_t* tok, const char* js, const char* key)
{
	const jsmntok_t* tok_with_key = tok_get_key(tok, js, key);

	// Check if key found
	if (tok_with_key != tok)
	{
		const jsmntok_t* tok_with_value = tok_get_value(tok_with_key);
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