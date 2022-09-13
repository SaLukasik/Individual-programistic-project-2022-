/** @file
 * Interfejs klasy wykonującej operacje na drzewie numerów telefonów.
 *
 * @author Sara Łukasik <sa.lukasik@student.uw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef PHONE_NUMBERS_TREE_H
#define PHONE_NUMBERS_TREE_H

/**
 * Liczba cyfr w systemie dziesiętnym.
 */
#define DIGITS 12

/**
 * Oznaczenie cyfry 10
 */
#define TEN '*'

/**
 * Oznaczenie cyfry 11
 */
#define ELEVEN '#'

/**
 * To jest implementacja struktury reprezentującej węzeł drzewa
 * numerów telefonu.Posiada przekierowanie numeru i adresy
 * numerów,które go rozszerzają.
 */
typedef struct Node {
    struct Node *numbers[DIGITS];   ///< tablica struktur typu Node
    struct Node *backward;  ///< drzewo z numerami przekierowującymi
    char *value;    ///< tablica wartości typu char, zawierająca przekierowanie
    char *mine; ///< tablica wartości typu char, zawierająca numer telefonu
    struct Node *parent;    ///< wskaźnik na rodzica węzła
    size_t index;   ///<- oznaczenie, którym dzieckiem rodzica jest węzeł
} Node;

/** @brief Zwalnia przekazane w argumentach wskaźniki
 * Zwalnia @p count wskaźników przekazanych jako argumenty.
 * @param[in] count – ilość wskaźników do usunięcia.
 */
void multiFree(unsigned int count, ...);

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę typu Node, która nie posiada żadnych przekierowań.
 * @param[in] parent – wskaźnik na rodzica węzła;
 * @param[in] index – numer węzła.
 * @return Wskaźnik na strukturę typu Node lub NULL w przypadku błędu
 *         alokacji pamięci.
 */
Node *nodeNew(Node *parent, size_t index);

/** @brief Konwertuje cyfrę zapisaną jako char na int.
 * Przyjmuje jedną z cyfr, które mogą tworzyć numer telefonu i
 * zwraca wartość typu size_t, która jej odpwoiada.
 * @param[in] num – wartość typu char, odpowiadająca jednej cyfrze z numeru.
 * @return wartość typu size_t odpowiadająca przekazanej warotści typu char.*/
size_t digitFinder(char num);

/** @brief Szuka węzła w drzewie numerów.
 * Szuka w drzewie numerów zadanego numeru. W przypadku nieznalezienia go,
 * zwraca null.
 * @param[in] node – wskaźnik na strukturę reprezentującą drzewo numemerów;
 * @param[in] num – tablica wartości typu char, reprezentująca numer telefonu.
 * @return Wskaźnik na węzeł reprezentujący zadany numer @p num lub
 *         NULL jeśli @p node ma wartość NULL lub nie znaleziono węzła.
 */
Node *findNode(Node *node, char const *num);

/** @brief Szuka lub tworzy węzeł w drzewie numerów.
 * Szuka w drzewie numerów zadanego numeru. W przypadku nieznalezienia go,
 * tworzy węzeł reprezentujący odpowiedni numer oraz w razie potrzeby węzły
 * stanowiące do niego ścieżkę.
 * @param[in] node – wskaźnik na strukturę reprezentującą drzewo numemerów;
 * @param[in] num – tablica wartości typu char, reprezentująca numer telefonu.
 * @return Wskaźnik na węzeł reprezentujący zadany numer @p num lub
 *         NULL jeśli @p node
 * ma wartość NULL lub doszło do błędu alokacji pamięci.
 */
Node *findOrCreateNode(Node *node, char const *num);

/** @brief Szuka węzła w drzewie numerów.
 * Szuka w drzewie numerów węzła reprezentującego zadany numer.
 * @param[in] node – wskaźnik na strukturę reprezentującą drzewo numemerów;
 * @param[in] num – tablica wartości typu char, reprezentująca numer telefonu.
 * @return Wskaźnik na węzeł reprezentujący zadany numer @p num
 *         lub NULL jeśli @p node
 * ma wartość NULL, lub szukany węzeł nie istnieje.
 */
Node *findNodeToRemove(Node *node, char const *num);

/** @brief Szuka najdłuższej ścieżki w drzewie, która pasując do danego numeru.
 * Szuka w drzewie najdłuższej ścieżki z tych, które tworzą prefiks zadanego
 * numeru.
 * @param[in] node – wskaźnik na strukturę reprezentującą drzewo numemerów;
 * @param[in, out] num – wskaźnikiem na tablicę wartości typu char,
 *                       reprezentująca numer telefonu.
 * @return Wskaźnik na węzeł reprezentujący najdłuższą ścieżkę, lub
 *         NULL jeśli @p node ma wartość NULL.
 */
Node *findLongest(Node *node, char const **num);

/** @brief Usuwa strukturę typu Node.
 * Usuwa strukturę @p node oraz wyszstkie struktury Node, które są pod nią.
 * @param[in] node – wskaźnik na strukturę, która ma być usunięta.
 */
void completeDelete(Node *node);

/** @brief Usuwa strukturę typu Node.
 * Usuwa strukturę @p node oraz wyszstkie struktury Node, które są pod nią.
 * @param[in] node – wskaźnik na strukturę, która ma być usunięta.
 * @param[in] to - wskażnik na strukturę przechowującą numery odpowiadające
 *                 przekierowaniom
 */
void nodeDelete(Node *node, Node *to);

#endif //PHONE_NUMBERS_TREE_H