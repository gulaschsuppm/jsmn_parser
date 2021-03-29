# JSMN Parser

[![Build Status](https://travis-ci.com/gulaschsuppm/jsmn_parser.svg?branch=main)](https://travis-ci.com/github/gulaschsuppm/jsmn_parser)
[![codecov](https://codecov.io/gh/gulaschsuppm/jsmn_parser/branch/main/graph/badge.svg)](https://codecov.io/gh/gulaschsuppm/jsmn_parser)

Provides helper functions for [jsmn](https://github.com/zserge/jsmn).
Since [jsmn](https://github.com/zserge/jsmn) provides tokenization of the JSON string and those tokens are inside an array, additional
parsing is needed to quickly access name/value pairs and indexes within the JSON objects.

## Philosophy

Same as jsmn, so **robust**, **fast**, **portable** and **simple**.

## Usage

As with jsmn, download `jsmn_parser.h` include it and done. (Also need to download `jsmn.h` from [github](https://github.com/zserge/jsmn), however it is included by `jsmn_parser.h`)

```
#include "jsmn_parser.h"

const char* example_json =
"{"
"\"Paul\":"
"   {"
"       \"Age\": 30,"
"       \"Children\":"
"       ["
"           \"Barbara\","
"           \"Peter\""
"       ]"
"   },"
"\"Anna\":"
"   {"
"       \"Age\":41,"
"       \"Children\":"
"       []"
"   }"
"}";

...

// Do jsmn things
jsmn_parser p;
jsmntok_t t[128]; /* We expect no more than 128 JSON tokens */

jsmn_init(&p);
r = jsmn_parse(&p, example_json, strlen(example_json), t, 128);

// Access the tokens
const jsmntok_t* pauls_second_child = tok_get_index(tok_get_name(tok_get_index(t, 0), s, "Children"), 1);

...

```

## API

All functions return either the found token or if nothing was found, the token that was provided. So always check if the
token changed before continuing.

### `tok_get_index`

Returns the token at the given index. Provide a base token pointing either to an object or an array or to their names.
(I.e. the token for `Paul` or the token after which is the actual object token.)

```
// Gets the token pointing to "Anna"
const jsmntok_t* anna = tok_get_index(t, 1);
```

### `tok_get_name`

Returns the token with the given name. Provide a base token pointing to an object or its name.
Also need to provide the json string to compare the name.

```
// Gets the token pointing to "Anna"
const jsmntok_t* anna = tok_get_name(t, example_json, "Anna");
```

### `tok_get_value`

Returns the value of the given name.

```
// Gets the token pointing to the age of "Anna"
const jsmntok_t* anna = tok_get_name(t, example_json, "Anna");
const jsmntok_t* age_name = tok_get_name(anna, example_json, "Age");
const jsmntok_t* age_value = tok_get_value(age_name);
```

### `tok_get_leaf`

Returns the token of the value with the given name, if this value is not nested anymore.

```
// Gets the token pointing to the age of "Anna"
const jsmntok_t* anna = tok_get_name(t, example_json, "Anna");
const jsmntok_t* age_value = tok_get_leaf(anna, example_json, "Age");
```
