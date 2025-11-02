#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

struct node
{
    char data[256];
    int priority;
    struct node* next;
};

//глобальные указатели на структуры
struct node* priority_head = NULL;
struct node* queue_front = NULL, * queue_rear = NULL;
struct node* stack_top = NULL;

// Прототипы функций
struct node* create_node(int with_priority);
void clear_list(struct node** head);
void print_list(struct node* head, const char* title);
void change_priority_for_all_with_name(char* data, int new_priority); // ИЗМЕНЕНО

//создаем новый элемент
struct node* create_node(int with_priority) {
    struct node* p = (struct node*)malloc(sizeof(struct node));
    if (p == NULL) {
        printf("Ошибка памяти\n");
        return NULL;
    }

    printf("Введите название: ");
    scanf("%s", p->data);

    if (with_priority) {
        printf("Введите приоритет: ");
        scanf("%d", &p->priority);
    }
    else {
        p->priority = 0;
    }

    p->next = NULL;
    return p;
}

//очистка списка
void clear_list(struct node** head) {
    struct node* current = *head;
    while (current != NULL) {
        struct node* next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}

//просмотр списка
void print_list(struct node* head, const char* title) {
    printf("\n%s:\n", title);
    if (head == NULL) {
        printf("Пусто\n");
        return;
    }

    struct node* current = head;
    while (current != NULL) {
        if (current->priority > 0) {
            printf("- %s (приоритет: %d)\n", current->data, current->priority);
        }
        else {
            printf("- %s\n", current->data);
        }
        current = current->next;
    }
}

void change_priority_for_all_with_name(char* data, int new_priority) {
    struct node* current = priority_head;
    int changed_count = 0;

    struct node* nodes_to_change[100];
    int count = 0;

    while (current != NULL && count < 100) {
        if (strcmp(current->data, data) == 0) {
            nodes_to_change[count] = current;
            count++;
        }
        current = current->next;
    }

    if (count == 0) {
        printf("Элементы с названием '%s' не найдены\n", data);
        return;
    }

    for (int i = 0; i < count; i++) {
        struct node* node_to_change = nodes_to_change[i];
        int old_priority = node_to_change->priority;

        if (old_priority == new_priority) {
            continue;
        }

        struct node* current_search = priority_head;
        struct node* prev = NULL;

        while (current_search != NULL && current_search != node_to_change) {
            prev = current_search;
            current_search = current_search->next;
        }

        if (current_search == NULL) continue;

        if (prev == NULL) {
            priority_head = current_search->next;
        }
        else {
            prev->next = current_search->next;
        }

        current_search->priority = new_priority;
        current_search->next = NULL;

        if (priority_head == NULL || new_priority > priority_head->priority) {
            current_search->next = priority_head;
            priority_head = current_search;
        }
        else {
            struct node* insert_current = priority_head;
            while (insert_current->next != NULL &&
                insert_current->next->priority >= new_priority) {
                insert_current = insert_current->next;
            }
            current_search->next = insert_current->next;
            insert_current->next = current_search;
        }

        changed_count++;
    }

    printf("Приоритет изменен для %d элементов с названием '%s'. Новый приоритет: %d\n",
        changed_count, data, new_priority);
}

//ПРИОРИТЕТНАЯ ОЧЕРЕДЬ 

void priority_add() {
    struct node* new_node = create_node(1);
    if (new_node == NULL) return;

    //если список пуст или новый элемент имеет высший приоритет
    if (priority_head == NULL || new_node->priority > priority_head->priority) {
        new_node->next = priority_head;
        priority_head = new_node;
        printf("Добавлен: %s\n", new_node->data);
        return;
    }

    //поиск места для вставки
    struct node* current = priority_head;
    while (current->next != NULL && current->next->priority >= new_node->priority) {
        current = current->next;
    }

    new_node->next = current->next;
    current->next = new_node;
    printf("Добавлен: %s\n", new_node->data);
}

void priority_remove() {
    if (priority_head == NULL) {
        printf("Очередь пуста\n");
        return;
    }

    struct node* temp = priority_head;
    printf("Удален: %s\n", temp->data);
    priority_head = priority_head->next;
    free(temp);
}

void priority_change() {
    if (priority_head == NULL) {
        printf("Очередь пуста\n");
        return;
    }

    char search_name[256];
    printf("Введите название элемента для изменения приоритета: ");
    scanf("%s", search_name);

    struct node* current = priority_head;
    int count = 0;
    int position = 1;


    current = priority_head;
    while (current != NULL) {
        if (strcmp(current->data, search_name) == 0) {
            count++;
        }
        current = current->next;
    }

    if (count == 0) {
        printf("Элемент с названием '%s' не найден\n", search_name);
        return;
    }

    printf("\nНайдено %d элементов с названием '%s':\n", count, search_name);
    current = priority_head;
    position = 1;
    while (current != NULL) {
        if (strcmp(current->data, search_name) == 0) {
            printf("- %s (приоритет: %d)\n",
                current->data, current->priority, position);
        }
        current = current->next;
        position++;
    }

    int new_priority;
    printf("\nВведите новый приоритет для всех элементов '%s': ", search_name);
    scanf("%d", &new_priority);

    // Проверяем, не совпадает ли новый приоритет с текущим у всех элементов
    current = priority_head;
    int all_same = 1;
    while (current != NULL) {
        if (strcmp(current->data, search_name) == 0 && current->priority != new_priority) {
            all_same = 0;
            break;
        }
        current = current->next;
    }

    if (all_same) {
        printf("Приоритет не изменен (новый приоритет совпадает с текущим у всех элементов)\n");
        return;
    }

    // Изменяем приоритет у всех элементов с таким названием
    change_priority_for_all_with_name(search_name, new_priority);
}

void priority_find() {
    if (priority_head == NULL) {
        printf("Очередь пуста\n");
        return;
    }

    char search_name[256];
    printf("Введите название элемента для поиска: ");
    scanf("%s", search_name);

    struct node* current = priority_head;
    int position = 1;
    int found_count = 0;

    printf("\nРезультаты поиска '%s':\n", search_name);
    while (current != NULL) {
        if (strcmp(current->data, search_name) == 0) {
            printf("%d. %s (приоритет: %d, позиция: %d)\n",
                found_count + 1, current->data, current->priority, position);
            found_count++;
        }
        current = current->next;
        position++;
    }

    if (found_count == 0) {
        printf("Элементы с названием '%s' не найдены\n", search_name);
    }
    else {
        printf("Всего найдено: %d элементов\n", found_count);
    }
}

void priority_menu() {
    int choice;
    do {
        printf("\n--- ПРИОРИТЕТНАЯ ОЧЕРЕДЬ ---\n");
        printf("1. Добавить элемент\n");
        printf("2. Удалить элемент\n");
        printf("3. Показать очередь\n");
        printf("4. Изменить приоритет элемента\n");
        printf("5. Найти элемент по имени\n");
        printf("0. Назад\n");
        printf("Выбор: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: priority_add(); break;
        case 2: priority_remove(); break;
        case 3: print_list(priority_head, "Приоритетная очередь"); break;
        case 4: priority_change(); break;
        case 5: priority_find(); break;
        }
    } while (choice != 0);
}

//ОЧЕРЕДЬ (FIFO)

void queue_add() {
    struct node* new_node = create_node(0);
    if (new_node == NULL) return;

    if (queue_rear == NULL) {
        queue_front = new_node;
        queue_rear = new_node;
    }
    else {
        queue_rear->next = new_node;
        queue_rear = new_node;
    }
    printf("Добавлен в очередь: %s\n", new_node->data);
}

void queue_remove() {
    if (queue_front == NULL) {
        printf("Очередь пуста\n");
        return;
    }

    struct node* temp = queue_front;
    printf("Удален из очереди: %s\n", temp->data);
    queue_front = queue_front->next;

    if (queue_front == NULL) {
        queue_rear = NULL;
    }

    free(temp);
}

void queue_menu() {
    int choice;
    do {
        printf("\n--- ОЧЕРЕДЬ (FIFO) ---\n");
        printf("1. Добавить в очередь\n");
        printf("2. Удалить из очереди\n");
        printf("3. Показать очередь\n");
        printf("0. Назад\n");
        printf("Выбор: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: queue_add(); break;
        case 2: queue_remove(); break;
        case 3: print_list(queue_front, "Очередь"); break;
        }
    } while (choice != 0);
}

//СТЕК (LIFO)

void stack_push() {
    struct node* new_node = create_node(0);
    if (new_node == NULL) return;

    new_node->next = stack_top;
    stack_top = new_node;
    printf("Добавлен в стек: %s\n", new_node->data);
}

void stack_pop() {
    if (stack_top == NULL) {
        printf("Стек пуст\n");
        return;
    }

    struct node* temp = stack_top;
    printf("Удален из стека: %s\n", temp->data);
    stack_top = stack_top->next;
    free(temp);
}

void stack_menu() {
    int choice;
    do {
        printf("\n--- СТЕК (LIFO) ---\n");
        printf("1. Добавить в стек\n");
        printf("2. Удалить из стека\n");
        printf("3. Показать стек\n");
        printf("0. Назад\n");
        printf("Выбор: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: stack_push(); break;
        case 2: stack_pop(); break;
        case 3: print_list(stack_top, "Стек"); break;
        }
    } while (choice != 0);
}

//ГЛАВНОЕ МЕНЮ

int main() {
    setlocale(LC_ALL, "RU");
    int choice;

    printf("=== СТРУКТУРЫ ДАННЫХ ===\n");

    do {
        printf("\nГлавное меню:\n");
        printf("1. Приоритетная очередь\n");
        printf("2. Очередь (FIFO)\n");
        printf("3. Стек (LIFO)\n");
        printf("4. Очистить все\n");
        printf("0. Выход\n");
        printf("Выбор: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: priority_menu(); break;
        case 2: queue_menu(); break;
        case 3: stack_menu(); break;
        case 4:
            clear_list(&priority_head);
            clear_list(&queue_front); queue_rear = NULL;
            clear_list(&stack_top);
            printf("Все структуры очищены\n");
            break;
        }
    } while (choice != 0);

    clear_list(&priority_head);
    clear_list(&queue_front);
    clear_list(&stack_top);

    return 0;
}