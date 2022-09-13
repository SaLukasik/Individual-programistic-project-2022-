/** @file
 * Implementacja Interfeju klasy wykonującej operacje na drzewie numerów
 * telefonów.
 *
 * @author Sara Łukasik <sa.lukasik@student.uw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"
#include <stdarg.h>

void multiFree(unsigned int count, ...) {
    va_list list;
    va_start(list, count);
    for (size_t j = 0; j < count; j++) {
        free(va_arg(list, void*));
    }
    va_end(list);
}

Node *nodeNew(Node *parent, size_t index) {
    Node *new = malloc(1 * sizeof(Node));
    if (new == NULL)
        return NULL;
    for (size_t i = 0; i < DIGITS; i++)
        new->numbers[i] = NULL;

    new->value = NULL;
    new->mine = NULL;
    new->backward = NULL;
    new->parent = parent;
    new->index = index;
    return new;
}

void completeDelete(Node *node) {
    if (node != NULL) {
        Node *end = node->parent;
        while (node != end) {
            size_t i = 0;
            while (i < DIGITS && node->numbers[i] == NULL)
                i++;
            if (i == DIGITS) {
                Node *next = node->parent;
                if (node->backward != NULL)
                    completeDelete(node->backward);
                multiFree(3, node->value, node->mine, node);
                node = next;
            } else {
                Node *next = node->numbers[i];
                node->numbers[i] = NULL;
                node = next;
            }
        }
    }
}

size_t digitFinder(char num) {
    if (num == TEN)
        return 10;
    else if (num == ELEVEN)
        return 11;
    else return num - '0';
}

Node *findNode(Node *node, char const *num) {
    size_t digit = digitFinder(num[0]);
    while (num[0] != '\0') {
        assert(digit <= 11);
        if (node->numbers[digit] == NULL) {
            return NULL;
        }
        node = node->numbers[digit];
        num = num + 1;
        digit = digitFinder(num[0]);
    }
    return node;
}


Node *findOrCreateNode(Node *node, char const *num) {
    assert(num != NULL);
    if (node == NULL)
        return NULL;
    size_t digit = digitFinder(num[0]);
    while (num[0] != '\0') {
        assert(digit <= 11);
        if (node->numbers[digit] == NULL) {
            node->numbers[digit] = nodeNew(node, digit);
            if (node->numbers[digit] == NULL)
                return NULL;
        }
        node = node->numbers[digit];
        num = num + 1;
        digit = digitFinder(num[0]);
    }
    return node;
}


Node *findNodeToRemove(Node *node, char const *num) {
    assert(num != NULL);
    if (node == NULL)
        return NULL;
    size_t digit = digitFinder(num[0]);
    while (node->numbers[digit] != NULL && num[1] != '\0') {
        node = node->numbers[digit];
        num = num + 1;
        digit = digitFinder(num[0]);
    }
    if (num[1] == '\0') {
        Node *to_return = node->numbers[digit];
        node->numbers[digit] = NULL;
        return to_return;
    } else
        return NULL;
}

Node *findLongest(Node *node, const char **num) {
    assert(num != NULL);
    if (node == NULL)
        return NULL;
    size_t digit = digitFinder(*num[0]);
    Node *longest = node;
    const char *sufix = *num;
    while (*num[0] != '\0' && node->numbers[digit] != NULL) {
        if (node->value != NULL) {
            longest = node;
            sufix = *num;
        }
        node = node->numbers[digit];
        *num = *num + 1;
        digit = digitFinder(*num[0]);
    }
    if (node->value != NULL) {
        longest = node;
        sufix = *num;
    }
    *num = sufix;
    return longest;
}

/** @brief Sprawdza, czy zadany węzeł nie ma synów.
 * Funkcja iteruje się po tablicy ze wskaźnikami na synów węzła sprawdzając,
 * czy któryś z nich nie jest NULLEM.
 * @param[in] node – wskaźnik na strukturę reprezentującą drzewo numemerów;
 * @return Wartość @p true, jeśli węzeł nie ma synów.
 *         Wartość @p false, jeśli węzeł ma co najmniej jednego syna.
 */
static inline bool isEmpty(Node *node) {
    if (node == NULL)
        return true;
    bool empty = true;
    for (size_t i = 0; i < DIGITS; i++) {
        if (node->numbers[i] != NULL) empty = false;
    }
    return empty;
}

/** @brief Czyści drzewo.
 * Po usunięciu węzła w drzewie @p back funckja sprawdza, czy nie prowadziła
 * do niego nie wypełniona wartościami ścieżka i usuwa ją.
 * @param[in] back – wskaźnik na strukturę reprezentującą drzewo numemerów;
 */
static inline void backCleaner(Node *back) {
    while (back->parent != NULL &&
           isEmpty(back->backward) &&
           isEmpty(back)) {
        Node *next_to_delete = back->parent;
        next_to_delete->numbers[back->index] = NULL;
        multiFree(3, back->value, back->backward,
                  back);
        back = next_to_delete;
    }
}

/** @brief Czyści drzewo.
 * Po usunięciu węzła w drzewie @p node funckja sprawdza, czy nie prowadziła
 * do niego nie wypełniona wartościami ścieżka i usuwa ją.
 * @param[in] node – wskaźnik na strukturę reprezentującą drzewo numemerów;
 */
static inline void frontCleaner(Node *node) {
    while (node != NULL && node->parent != NULL && isEmpty(node) &&
           node->value == NULL) {
        Node *next = node->parent;
        next->numbers[node->index] = NULL;
        multiFree(3, node->value, node->mine, node);
        node = next;
    }
}

void nodeDelete(Node *node, Node *to) {
    if (node != NULL) {
        Node *end = node->parent;
        while (node != end) {
            size_t i = 0;
            while (i < DIGITS && node->numbers[i] == NULL)
                i++;
            if (i == DIGITS) {
                Node *next = node->parent;
                if (to != NULL && node->value != NULL) {
                    Node *back = findNode(to, node->value);
                    if (back != NULL) {
                        Node *to_delete = findNodeToRemove(back->backward,
                                                           node->mine);
                        nodeDelete(to_delete, NULL);
                        backCleaner(back);
                    }
                }
                multiFree(3, node->value, node->mine, node);
                node = next;
            } else {
                Node *next = node->numbers[i];
                node->numbers[i] = NULL;
                node = next;
            }
        }
        frontCleaner(node);
    }
}
