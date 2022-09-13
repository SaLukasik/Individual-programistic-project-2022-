/** @file
 * Implementacja klasy przechowującej przekierowania numerów telefonicznych
 *
 * @author Sara Łukasik <sa.lukasik@student.uw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "phone_forward.h"


/**
 * To jest implementacja struktury przechowującej przekierowania
 * numerów telefonów.
 */
struct PhoneForward {
    Node *from; ///< korzeń drzewa z numerami przekierowywanymi
    Node *to; ///< korzeń drzewa z numerami, na które są przekierowania
};

/**
 * To jest implementacja struktury przechowującej ciąg numerów telefonów.
 */
struct PhoneNumbers {
    char **numbers;    ///< tablica przechowująca numery telefonów
    size_t size;       ///< aktualny rozmiar tablicy @p numbers
    size_t last;       ///< indeks wskazujący pierwsze wolne miejsce w tablicy
};

/** @brief Kopiuje tablicę wartości typu char.
 * Tworzy nową tablicę wartości typu char, a następnie przepisuje na nią
 * kolejne elementy @p str.
 * @param[in] str – wskaźnik na tablicę charów, która będzię kopiowana;
 * @return Wskaźnik na zaalokowaną tablicę charów, której długość i wartości
 *         odpowiadają przekazanej tablicy, NULL jeśli alokacja pamięci się nie
 *         powiodła.
 */
static inline char *copyString(char const *str) {
    assert(str != NULL);

    char *copy = malloc(strlen(str) + 1);
    if (copy == NULL)
        return NULL;
    strcpy(copy, str);
    return copy;
}

/** @brief Tworzy nową strukturę typu PhoneNumbers.
 * Tworzy nową strukturę niezawierającą żadnych numerów.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * alokować pamięci.
 */
static inline PhoneNumbers *phnumNew(void) {
    PhoneNumbers *new = malloc(1 * sizeof(PhoneNumbers));
    if (new == NULL)
        return NULL;
    new->numbers = malloc(sizeof(const char *));
    if (new->numbers == NULL) {
        free(new);
        return NULL;
    }
    new->size = 1;
    new->last = 0;
    return new;
}

/** @brief Dodaje numer.
 * Dodaje przekierowanie wszystkich numerów mających prefiks @p num1, na numery,
 * w których ten prefiks zamieniono odpowiednio na prefiks @p num2. Każdy numer
 * @param[in] phones – wskaźnik na strukturę przechowującą numery;
 * @param[in] number - numer, który ma być dodany.
 * @return Wartość @p true, jeśli dodawanie numeru powiodło się lub
 *         wartość @p false, jeśli wystąpił błąd pamięci. */
static inline bool phnumAdd(PhoneNumbers *phones, char const *number) {
    for (size_t i = 0; i < phones->last; i++) {
        if (strcmp(phones->numbers[i], number) == 0) {
            return true;
        }
    }
    if (phones->size == phones->last) {
        phones->size = 2 * phones->size;
        char **ptr = realloc(phones->numbers,
                             (phones->size) * sizeof(char const *));
        if (ptr == NULL)
            return false;
        else
            phones->numbers = ptr;
    }
    phones->numbers[phones->last] = copyString(number);
    if (phones->numbers[phones->last] == NULL)
        return false;
    phones->last = phones->last + 1;
    return true;
}

/** @brief Sprawdza, czy tablica wartości typu char to poprawny numer.
 * Sprawdza, czy tablica charów składa się z samych cyfr i kończy znakiem "\0".
 * @param[in] number – wskaźnik na tablicę wartości typu char, która
 *                     będzię sprawdzana.
 * @return Wartość @p true, jeśli @p number jest wskaźnikiem na tablicę
 *         poprawnie reprezentującą numer.
 *         Wartość @p false,  jeśli @p number jest wskaźnikiem na tablicę
 *         niepoprawnie reprezentującą numer.
 */
static inline bool isItNumber(char const *number) {
    if (number == NULL)
        return false;
    if (number[0] == '\0')
        return false;
    size_t i = 0;
    while (isdigit(number[i]) || number[i] == TEN || number[i] == ELEVEN)
        i++;
    if (number[i] == '\0')
        return true;
    else
        return false;
}

PhoneForward *phfwdNew(void) {
    PhoneForward *new = malloc(1 * sizeof(PhoneForward));
    if (new == NULL)
        return NULL;
    else {
        new->from = nodeNew(NULL, 0);
        if (new->from == NULL) {
            free(new);
            return NULL;
        }
        new->to = nodeNew(NULL, 0);
        if (new->to == NULL) {
            free(new->from);
            free(new);
            return NULL;
        }
        new->to->backward = nodeNew(NULL, 0);
        if (new->to->backward == NULL) {
            free(new->from);
            free(new->to);
            free(new);
            return NULL;
        }
    }
    return new;
}

void phfwdDelete(PhoneForward *pf) {
    if (pf != NULL) {
        completeDelete(pf->from);
        completeDelete(pf->to);
        free(pf);
    }
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {
    if (pf == NULL || !isItNumber(num1) || !isItNumber(num2) ||
        !strcmp(num1, num2))
        return false;
    Node *from = findOrCreateNode(pf->from, num1);
    if (from == NULL)
        return false;
    Node *to = findOrCreateNode(pf->to, num2);
    if (to == NULL)
        return false;
    char *new_value_from = copyString(num2);
    char *new_mine = copyString(num1);
    char *new_value_to = copyString(num2);
    char *new_value_subtree = copyString(num1);
    if (new_value_from == NULL || new_mine == NULL || new_value_to == NULL) {
        multiFree(4, new_value_from, new_mine, new_value_to,
                  new_value_subtree);
        return false;
    }
    if (to->backward == NULL) {
        to->backward = nodeNew(NULL, 0);
        if (to->backward == NULL) {
            multiFree(4, new_value_from, new_mine, new_value_to,
                      new_value_subtree);
            return false;
        }
    }
    Node *subtree = findOrCreateNode(to->backward, num1);
    if (subtree == NULL) {
        multiFree(4, new_value_from, new_mine, new_value_to,
                  new_value_subtree);
        return false;
    }
    if (from->value != NULL) {
        Node *old = findNode(pf->to, from->value);
        Node *to_delete = findNode(old->backward, from->mine);
        free(to_delete->value);
        to_delete->value = NULL;
    }
    multiFree(4, subtree->value, from->value, from->mine, to->value);
    subtree->value = new_value_subtree;
    from->value = new_value_from;
    from->mine = new_mine;
    to->value = new_value_to;
    return true;
}

void phfwdRemove(PhoneForward *pf, char const *num) {
    if (pf != NULL && isItNumber(num)) {
        Node *to_remove = findNodeToRemove(pf->from, num);
        nodeDelete(to_remove, pf->to);
    }
}

PhoneNumbers *phfwdGet(PhoneForward const *pf, char const *num) {
    if (pf == NULL)
        return NULL;
    PhoneNumbers *result = phnumNew();
    if (result == NULL)
        return NULL;
    if (!isItNumber(num))
        return result;
    const char *forward = num;
    Node *longest = findLongest(pf->from, &forward);
    assert(longest != NULL);
    char *number;
    if (longest->value != NULL) {
        number = malloc(strlen(forward) + strlen(longest->value) + 1);
        if (number == NULL) {
            phnumDelete(result);
            return NULL;
        }
        strcpy(number, longest->value);
        number = strcat(number, forward);
    } else {
        number = copyString(num);
    }
    if (phnumAdd(result, number) == false) {
        free(number);
        phnumDelete(result);
        return NULL;
    }
    free(number);
    return result;
}

/** @brief Uzupełnia tablicę numerów takimi numerami, które pochodzą od
 * odpowiedniego przekierowania.
 * Jeśli @p form wynosi NULL, uzupełnia tablicę numerów numerami zgodnymi ze s
 * pecyfikacją zawartą w opisie funkcji @p phfwdReverse. Gdy @p from ma inną
 * wartość, uzupełnia zgodnie ze specyfikacją funkcji @p phfwdGetReverse.
 * Numery znajduje w drzewie odwróconych przekierowań.
 * @param[in] to – wskaźnik na drzewo odwróconych przekierowań;
 * @param[in] from – wskaźnik na drzewo przekierowań;
 * @param[in] pf – wskaźnik na strukturę przechowującą przekierowania;
 * @param[in] num – tablica wartości typu char, reprezentująca numer telefonu.
 * @param[in, out] phones – wskaźnik na strukturę zawierającą tablicę numerów.
 * @return wskaźnik na strukturę zawierającą tablicę numerów, lub NULL, jeśli
 * nie udało się alokować pamięci.
 */
static inline PhoneNumbers *
findNumbers(Node *to, Node *from, PhoneForward const *pf, char const *num,
            PhoneNumbers *phones) {
    char const *what = num;
    while (num[0] != '\0' && to != NULL && phones != NULL) {
        size_t digit = digitFinder(num[0]);
        to = to->numbers[digit];
        num = num + 1;
        Node *node;
        if (to != NULL)
            node = to->backward;
        else
            node = NULL;
        size_t i = 0;
        while (node != NULL) {
            while (i < DIGITS && node->numbers[i] == NULL && phones != NULL)
                i++;
            if (i == DIGITS) {
                if (node->value != NULL) {
                    char *number = malloc(
                            strlen(num) + strlen(node->value) + 1);
                    if (number == NULL) {
                        phnumDelete(phones);
                        phones = NULL;
                    } else {
                        strcpy(number, node->value);
                        strcat(number, num);
                        if (from != NULL && num[0] != '\0') {
                            PhoneNumbers *found = phfwdGet(pf, number);
                            if (found != NULL &&
                                strcmp(phnumGet(found, 0), what) == 0) {
                                if (!phnumAdd(phones, number)) {
                                    free(number);
                                    phnumDelete(phones);
                                    phnumDelete(found);
                                    return NULL;
                                }
                            }
                            phnumDelete(found);
                        } else {
                            if (!phnumAdd(phones, number)) {
                                free(number);
                                phnumDelete(phones);
                                return NULL;
                            }
                        }
                        free(number);
                    }
                }
                i = node->index + 1;
                Node *next = node->parent;
                node = next;
            } else {
                Node *next = node->numbers[i];
                i = 0;
                node = next;
            }
        }
    }
    return phones;
}

/** @brief Funkcja porównująca dwa numery telefonu.
 * Funkcja porównuje kepsykograficznie dwa numery telefonów.
 * @param[in] val1 – wskaźnik na tablicę wartości typu char, preprezentującą
 *                  pierwszy numer.
 * @param[in] val2 – wskaźnik na tablicę wartości typu char, preprezentującą
 *                  drugi numer.
 * @return Wartość 0, jeśli numery są równe, -1, jeśli pierwszy numer jest
 *         mniejszy i 1, jeśli drugi numer jest mniejszy.
 */
static inline int compare(const void *val1, const void *val2) {
    char *num1 = *(char **) val1;
    char *num2 = *(char **) val2;

    while (num1[0] == num2[0] && num1[0] != '\0' && num2[0] != '\0') {
        num1 += 1;
        num2 += 1;
    }
    if (num1[0] == '\0' && num2[0] == '\0') {
        return 0;
    } else if (num1[0] == '\0') {
        return -1;
    } else if (num2[0] == '\0') {
        return 1;
    } else {
        size_t a = digitFinder(num1[0]);
        size_t b = digitFinder(num2[0]);
        if (a < b) return -1;
        else return 1;
    }
}

PhoneNumbers *phfwdReverse(PhoneForward const *pf, char const *num) {
    if (pf == NULL)
        return NULL;
    PhoneNumbers *result = phnumNew();
    if (result == NULL)
        return NULL;
    if (!isItNumber(num))
        return result;
    if (phnumAdd(result, num) == false) {
        phnumDelete(result);
        return NULL;
    }
    result = findNumbers(pf->to, NULL, pf, num, result);
    if (result != NULL)
        qsort(result->numbers, result->last, sizeof(char *),
              compare);

    return result;
}

void phnumDelete(PhoneNumbers *pnum) {
    if (pnum != NULL) {
        if (pnum->numbers != NULL)
            for (size_t i = 0; i < pnum->last; i++)
                free(pnum->numbers[i]);
        multiFree(2, pnum->numbers, pnum);
    }
}

char const *phnumGet(PhoneNumbers const *pnum, size_t idx) {
    if (pnum == NULL || idx >= pnum->last)
        return NULL;
    char const *result = pnum->numbers[idx];
    return result;
}

PhoneNumbers *phfwdGetReverse(PhoneForward const *pf, char const *num) {
    if (pf == NULL)
        return NULL;
    PhoneNumbers *result = phnumNew();
    if (result == NULL)
        return NULL;
    if (!isItNumber(num))
        return result;
    result = findNumbers(pf->to, pf->from, pf, num, result);
    PhoneNumbers *pnum;
    pnum = phfwdGet(pf, num);
    if (strcmp(phnumGet(pnum, 0), num) == 0) {
        if (phnumAdd(result, num) == false) {
            phnumDelete(result);
            phnumDelete(pnum);
            return NULL;
        }
    }
    phnumDelete(pnum);
    if (result != NULL)
        qsort(result->numbers, result->last, sizeof(char *),
              compare);

    return result;
}