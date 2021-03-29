#include "jsmn_parser.h"
#include "unity.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

static jsmn_parser parser;

static jsmntok_t* tokenize(const char* js, size_t len)
{
    jsmntok_t* tokens = NULL;
    uint32_t token_len = 1;
    while (true)
    {
        token_len += 5;
        tokens = realloc(tokens, sizeof(jsmntok_t) * token_len);
		if (tokens == NULL)
		{
			tokens = realloc(tokens, 0);
            return NULL;
		}
        int tokens_read = jsmn_parse(&parser, js, len, tokens, token_len);
		// not enough tokens
		if (tokens_read == JSMN_ERROR_NOMEM)
		{
			continue;
		}
		// bad token or string too short
		else if (tokens_read == JSMN_ERROR_INVAL || tokens_read == JSMN_ERROR_PART)
		{
			tokens = realloc(tokens, 0);
            return NULL;
		}

		// If we read less tokens than provided, done
		if (tokens_read <= token_len)
		{
			// Fix how many tokens we actually have
			// TODO: reallocate?
			token_len = tokens_read;
			break;
		}
    }

    return tokens;
}

void setUp(void)
{
    jsmn_init(&parser);
}

void tearDown(void)
{

}

void test_index_objects()
{
    const char* json_test_string =
    "{"
    "\"Object1\":{\"Key\":0}"
    "\"Object2\":{\"Key\":1}"
    "}";

    jsmntok_t* tokens = tokenize(json_test_string, strlen(json_test_string));

    TEST_ASSERT(tokens != NULL);

    // Access the first (0) index
    const jsmntok_t* obj1 = tok_get_index(tokens, 0);
    TEST_ASSERT(obj1 != tokens);
    TEST_ASSERT_EQUAL_STRING_LEN("Object1", json_test_string + obj1->start, obj1->end - obj1->start);

    // Access first (0) index within dave
    const jsmntok_t* obj1_key = tok_get_index(obj1, 0);
    TEST_ASSERT(obj1_key != obj1);
    TEST_ASSERT_EQUAL_STRING_LEN("Key", json_test_string + obj1_key->start, obj1_key->end - obj1_key->start);

    // Access the second (1) index
    const jsmntok_t* obj2 = tok_get_index(tokens, 1);
    TEST_ASSERT(obj2 != tokens);
    TEST_ASSERT_EQUAL_STRING_LEN("Object2", json_test_string + obj2->start, obj2->end - obj2->start);

    const jsmntok_t* obj3 = tok_get_index(tokens, 2);
    // Equal means nothing found
    TEST_ASSERT(obj3 == tokens);
}

void test_index_objects_with_key()
{
    const char* json_test_string =
    "\"Objects\":"
    "{"
    "\"Object1\":{\"Key\":0}"
    "\"Object2\":{\"Key\":1}"
    "}";

    jsmntok_t* tokens = tokenize(json_test_string, strlen(json_test_string));

    TEST_ASSERT(tokens != NULL);

    // Access the first (0) index
    const jsmntok_t* obj1 = tok_get_index(tokens, 0);
    TEST_ASSERT(obj1 != tokens);
    TEST_ASSERT_EQUAL_STRING_LEN("Object1", json_test_string + obj1->start, obj1->end - obj1->start);

    // Access first (0) index within dave
    const jsmntok_t* obj1_key = tok_get_index(obj1, 0);
    TEST_ASSERT(obj1_key != obj1);
    TEST_ASSERT_EQUAL_STRING_LEN("Key", json_test_string + obj1_key->start, obj1_key->end - obj1_key->start);

    // Access the second (1) index
    const jsmntok_t* obj2 = tok_get_index(tokens, 1);
    TEST_ASSERT(obj2 != tokens);
    TEST_ASSERT_EQUAL_STRING_LEN("Object2", json_test_string + obj2->start, obj2->end - obj2->start);

    const jsmntok_t* obj3 = tok_get_index(tokens, 2);
    // Equal means nothing found
    TEST_ASSERT(obj3 == tokens);
}

void test_index_array()
{
    const char* json_test_string =
    "["
        "\"Item1\""
        "\"Item2\""
    "]";

    jsmntok_t* tokens = tokenize(json_test_string, strlen(json_test_string));

    TEST_ASSERT(tokens != NULL);

    // Access the first (0) index
    const jsmntok_t* item1 = tok_get_index(tokens, 0);
    TEST_ASSERT(item1 != tokens);
    TEST_ASSERT_EQUAL_STRING_LEN("Item1", json_test_string + item1->start, item1->end - item1->start);

    // Access first (0) index within dave
    const jsmntok_t* item1_key = tok_get_index(item1, 0);
    // Equal means nothing found
    TEST_ASSERT(item1_key == item1);

    // Access the second (1) index
    const jsmntok_t* item2 = tok_get_index(tokens, 1);
    TEST_ASSERT(item2 != tokens);
    TEST_ASSERT_EQUAL_STRING_LEN("Item2", json_test_string + item2->start, item2->end - item2->start);

    const jsmntok_t* item3 = tok_get_index(tokens, 2);
    // Equal means nothing found
    TEST_ASSERT(item3 == tokens);
}

void test_index_array_with_key()
{
    const char* json_test_string =
    "\"Array\":"
    "["
        "\"Item1\""
        "\"Item2\""
    "]";

    jsmntok_t* tokens = tokenize(json_test_string, strlen(json_test_string));

    TEST_ASSERT(tokens != NULL);

    // Access the first (0) index
    const jsmntok_t* item1 = tok_get_index(tokens, 0);
    TEST_ASSERT(item1 != tokens);
    TEST_ASSERT_EQUAL_STRING_LEN("Item1", json_test_string + item1->start, item1->end - item1->start);

    // Access first (0) index within dave
    const jsmntok_t* item1_key = tok_get_index(item1, 0);
    // Equal means nothing found
    TEST_ASSERT(item1_key == item1);

    // Access the second (1) index
    const jsmntok_t* item2 = tok_get_index(tokens, 1);
    TEST_ASSERT(item2 != tokens);
    TEST_ASSERT_EQUAL_STRING_LEN("Item2", json_test_string + item2->start, item2->end - item2->start);

    const jsmntok_t* item3 = tok_get_index(tokens, 2);
    // Equal means nothing found
    TEST_ASSERT(item3 == tokens);
}

void test_index_array_of_objects()
{
    const char* json_test_string =
    "["
        "\"Object1\":"
        "{"
            "\"Name\": \"Dave\","
            "\"Age\": 30"
        "},"
        "\"Object2\":"
        "{"
            "\"Size\": 5"
        "},"
        "\"Object3\":"
        "{"
            "\"City\": \"Lisbon\","
            "\"Population\": 1534678,"
            "\"ZIP\": 9088"
        "}"
    "]";

    jsmntok_t* tokens = tokenize(json_test_string, strlen(json_test_string));

    TEST_ASSERT(tokens != NULL);

    // Access the first (0) index
    const jsmntok_t* obj1 = tok_get_index(tokens, 0);
    TEST_ASSERT(obj1 != tokens);
    TEST_ASSERT_EQUAL_STRING_LEN("Object1", json_test_string + obj1->start, obj1->end - obj1->start);

    // Access the second (1) index within obj1
    const jsmntok_t* obj1_second = tok_get_index(obj1, 1);
    TEST_ASSERT(obj1_second != obj1);
    TEST_ASSERT_EQUAL_STRING_LEN("Age", json_test_string + obj1_second->start, obj1_second->end - obj1_second->start);

    // Access the third (2) index
    const jsmntok_t* obj3 = tok_get_index(tokens, 2);
    TEST_ASSERT(obj3 != tokens);
    TEST_ASSERT_EQUAL_STRING_LEN("Object3", json_test_string + obj3->start, obj3->end - obj3->start);

    // Access the second (1) index within obj3
    const jsmntok_t* obj3_second = tok_get_index(obj3, 1);
    TEST_ASSERT(obj3_second != obj3);
    TEST_ASSERT_EQUAL_STRING_LEN("Population", json_test_string + obj3_second->start, obj3_second->end - obj3_second->start);

    // Access the fourth (3) index
    const jsmntok_t* obj4 = tok_get_index(tokens, 3);
    // Equal means nothing found
    TEST_ASSERT(obj4 == tokens);
}

void test_name_objects()
{
    const char* json_test_string =
    "{"
    "\"Object1\":{\"Key\":0}"
    "\"Object2\":{\"Key\":1}"
    "}";

    jsmntok_t* tokens = tokenize(json_test_string, strlen(json_test_string));

    TEST_ASSERT(tokens != NULL);

    // Access the first (0) index
    const jsmntok_t* obj1 = tok_get_name(tokens, json_test_string, "Object1");
    TEST_ASSERT(obj1 != tokens);
    TEST_ASSERT_EQUAL_STRING_LEN("Object1", json_test_string + obj1->start, obj1->end - obj1->start);

    // Try to access key
    const jsmntok_t* key = tok_get_name(tokens, json_test_string, "Key");
    // Equal means nothing found
    TEST_ASSERT(key == tokens);

    // Access first (0) index within dave
    const jsmntok_t* obj1_key = tok_get_name(obj1, json_test_string, "Key");
    TEST_ASSERT(obj1_key != obj1);
    TEST_ASSERT_EQUAL_STRING_LEN("Key", json_test_string + obj1_key->start, obj1_key->end - obj1_key->start);

    // Access the second (1) index
    const jsmntok_t* obj2 = tok_get_name(tokens, json_test_string, "Object2");
    TEST_ASSERT(obj2 != tokens);
    TEST_ASSERT_EQUAL_STRING_LEN("Object2", json_test_string + obj2->start, obj2->end - obj2->start);

    const jsmntok_t* obj3 = tok_get_name(tokens, json_test_string, "Object3");
    // Equal means nothing found
    TEST_ASSERT(obj3 == tokens);
}

void test_name_string_in_array()
{
    const char* json_test_string =
    "{"
    "\"Array\":[\"FakeKey1\",\"FakeKey2\"]"
    "}";

    jsmntok_t* tokens = tokenize(json_test_string, strlen(json_test_string));

    TEST_ASSERT(tokens != NULL);

    // Access the key "Array"
    const jsmntok_t* arr = tok_get_name(tokens, json_test_string, "Array");
    TEST_ASSERT(arr != tokens);
    TEST_ASSERT_EQUAL_STRING_LEN("Array", json_test_string + arr->start, arr->end - arr->start);

    // Try to access a string in an array
    const jsmntok_t* fake_key = tok_get_name(arr, json_test_string, "FakeKey1");
    // Equal means nothing found
    TEST_ASSERT(fake_key == arr);
}

void test_example_in_readme()
{
    const char* json_test_string =
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

    jsmntok_t* tokens = tokenize(json_test_string, strlen(json_test_string));

    const jsmntok_t* pauls_second_child = tok_get_index(tok_get_name(tok_get_name(tokens, json_test_string, "Paul"), json_test_string, "Children"), 1);
    TEST_ASSERT(pauls_second_child != tokens);
    TEST_ASSERT_EQUAL_STRING_LEN("Peter", json_test_string + pauls_second_child->start, pauls_second_child->end - pauls_second_child->start);

    const jsmntok_t* annas_age = tok_get_leaf(tok_get_index(tokens, 1), json_test_string, "Age");
    TEST_ASSERT(annas_age != tokens);
    TEST_ASSERT_EQUAL_STRING_LEN("41", json_test_string + annas_age->start, annas_age->end - annas_age->start);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_index_objects);
    RUN_TEST(test_index_objects_with_key);
    RUN_TEST(test_index_array);
    RUN_TEST(test_index_array_with_key);
    RUN_TEST(test_index_array_of_objects);

    RUN_TEST(test_name_objects);
    RUN_TEST(test_name_string_in_array);

    RUN_TEST(test_example_in_readme);
    
    return UNITY_END();
}