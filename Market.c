#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h> // <Enter> �Է°� �ϴ�

// ��ǰ ������ �����ϴ� ����ü ����
typedef struct {
    int id;
    char name[50];
    int quantity;
    float price;
} Item;

// ��ǰ ����Ʈ ��� ����
typedef struct ItemNode {
    Item item;
    struct ItemNode* next;
} ItemNode;

// ��ǰ ����Ʈ ����
typedef struct {
    ItemNode* head;
} ItemList;

// �� ������ �����ϴ� ����ü ����
typedef struct {
    int visit_prob;
    int buy_prob;
    int item_prob[50];
    char type[20];  // �� ���� (�л�, ������, �ֺ�)
} Customer;

// �� ����Ʈ ��� ����
typedef struct CustomerNode {
    Customer customer;
    struct CustomerNode* next;
} CustomerNode;

// �� ����Ʈ ����
typedef struct {
    CustomerNode* head;
} CustomerList;

float capital = 1000000;  // �ʱ� �ں� 100����
float daily_profit = 0;  // ���� ����
float total_profit = 0;  // �� ����

typedef struct {
    int item_id;
    char item_name[50];
    int sold_quantity;
    float price;
} PurchaseSummary;

// ���� ��� ����Ʈ ��� ����
typedef struct PurchaseNode {
    PurchaseSummary summary;
    struct PurchaseNode* next;
} PurchaseNode;

// ���� ��� ����Ʈ ����
typedef struct {
    PurchaseNode* head;
} PurchaseList;

//�ʱ� ��ǰ ����Ʈ
void initialize_items(ItemList* itemList) {
    Item items[50] = {
        // �Ǿ�ǰ
        {1, "������", 20, 3500},
        {2, "��ħ��", 20, 3500},
        {3, "��â��", 50, 1000},
        {4, "����ũ", 50, 5000},
        {5, "�ռҵ���", 30, 4000},
        // ���� �� �����
        {6, "�����", 40, 1500},
        {7, "�������� Ĩ", 40, 1700},
        {8, "���ݸ�", 50, 1800},
        {9, "����", 50, 500},
        {10, "��", 50, 500},
        {11, "���̴�", 30, 1500},
        {12, "�ݶ�", 30, 1500},
        {13, "������ �ֽ�", 30, 2500},
        {14, "��� �ֽ�", 30, 2500},
        {15, "ź���", 40, 1500},
        {16, "������ �帵ũ", 50, 1500},
        {17, "����", 15, 2000},
        {18, "���ϸ� ��", 10, 1700},
        // ��
        {19, "����", 20, 3700},
        {20, "����", 20, 2500},
        {21, "����", 20, 5000},
        // ���� ����
        {22, "������ġ", 30, 3000},
        {23, "���", 40, 900},
        {24, "���ö�", 15, 4500},
        {25, "������", 20, 1500},
        {26, "���̽� ũ��", 60, 2100},
        // ��Ȱ ��ǰ
        {27, "���", 10, 5000},
        {28, "����", 50, 1000},
        {29, "ĩ��", 20, 2500},
        {30, "ġ��", 15, 3000},
        {31, "��Ǫ", 15, 5000},
        {32, "�ٵ� ����", 15, 5500},
        {33, "���", 30, 4500},
        {34, "��", 25, 2700},
        {35, "ȭ����", 30, 500},
        {36, "���� Ÿ��", 20, 2500},
        {37, "��Ƽ��", 40, 4000},
        {38, "����", 20, 10000},
        {39, "���� ������", 20, 6200},
        {40, "�鵵 ũ��", 15, 4000},
        {41, "�鵵��", 20, 5000},
        // �ķ�ǰ
        {42, "����", 30, 3000},
        {43, "�䱸��Ʈ", 40, 2000},
        {44, "�Ļ�", 15, 2000},
        {45, "���", 30, 2000},
        {46, "����", 20, 2000},
        {47, "Ŀ��", 20, 2000},
        {48, "Ŀ�� ����", 20, 1700},
        {49, "���", 10, 2000},
        {50, "�ٳ���", 10, 2000}
    };

    for (int i = 0; i < 50; i++) {
        ItemNode* new_node = (ItemNode*)malloc(sizeof(ItemNode));
        if (new_node == NULL) {
            fprintf(stderr, "�޸� �Ҵ� ����\n");
            exit(1);
        }
        new_node->item = items[i];
        new_node->next = itemList->head;
        itemList->head = new_node;
    }
}

// ��ǰ�� ������ ������ Ȯ���ϴ� �Լ�
ItemNode* find_item(ItemList* list, int id) {
    ItemNode* current = list->head;
    while (current != NULL) {
        if (current->item.id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// �� ������ ���� ���� Ȯ�� ����
void set_initial_probabilities(Customer* customer) {
    if (strcmp(customer->type, "�л�") == 0) {
        for (int i = 0; i < 50; i++) {
            if (i >= 5 && i <= 18) { 
                customer->item_prob[i] = rand() % 61 + 20;  // �ּ� 20% �̻�, �ִ� 80%
            }
            else {
                customer->item_prob[i] = rand() % 81;  // �ִ� 80%
            }
        }
    }
    else if (strcmp(customer->type, "������") == 0) {
        for (int i = 0; i < 50; i++) {
            if (i >= 0 && i <= 4) {  // �Ǿ�ǰ 
                customer->item_prob[i] = rand() % 66 + 15;  // �ּ� 15% �̻�, �ִ� 80%
            }
            else if (i >= 18 && i <= 20) {  // �� 
                customer->item_prob[i] = rand() % 66 + 15;  // �ּ� 15% �̻�, �ִ� 80%
            }
            else {
                customer->item_prob[i] = rand() % 81;  // �ִ� 80%
            }
        }
    }
    else if (strcmp(customer->type, "�ֺ�") == 0) {
        for (int i = 0; i < 50; i++) {
            if (i >= 26 && i <= 50) {  // ��Ȱ��ǰ�� �ķ�ǰ
                customer->item_prob[i] = rand() % 61 + 20;  // �ּ� 20% �̻�, �ִ� 80%
            }
            else {
                customer->item_prob[i] = rand() % 81;  // �ִ� 80%
            }
        }
    }
}

void initialize_customers(CustomerList* customerList, int num_customers) {
    srand((unsigned int)time(NULL));

    const char* types[] = { "�л�", "������", "�ֺ�" };
    int type_counts[] = { 17, 17, 16 };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < type_counts[i]; j++) {
            Customer customer;
            customer.visit_prob = rand() % 80 + 1;
            customer.buy_prob = rand() % 80 + 1;
            strcpy_s(customer.type, sizeof(customer.type), types[i]);
            set_initial_probabilities(&customer);

            // ���� ��带 �����Ͽ� ����Ʈ�� �߰�
            CustomerNode* new_node = (CustomerNode*)malloc(sizeof(CustomerNode));
            if (new_node == NULL) {
                fprintf(stderr, "�޸� �Ҵ� ����\n");
                exit(1); // �޸� �Ҵ� ���� �� ���α׷� ����
            }
            new_node->customer = customer;
            new_node->next = customerList->head;
            customerList->head = new_node;
        }
    }
}

// Ȯ�� ���� �Լ�
void update_probability(int* prob, int min_prob) {
    int change = (rand() % 41) - 20;
    *prob += change;
    if (*prob < min_prob) *prob = min_prob;
    if (*prob > 100) *prob = 80;
}

// �ܺ� ������ ���� Ȯ�� ���� �Լ�
void adjust_probabilities(CustomerList* customerList, const char* weather, const char* temperature) {
    CustomerNode* current = customerList->head;

    while (current != NULL) {
        Customer* customer = &current->customer;

        if (strcmp(weather, "rain") == 0) {
            customer->item_prob[18] += 20;  // �񰡿��� ��� ���� Ȯ�� +20%
            if (customer->item_prob[18] > 100) customer->item_prob[18] = 100;
        }

        if (strcmp(temperature, "hot") == 0) { 
            customer->item_prob[17] += 20;  // ������̸� ���̽�ũ�� ���� Ȯ�� +20%
            if (customer->item_prob[17] > 100) customer->item_prob[17] = 100;
        }
        else if (strcmp(temperature, "cold") == 0) {
            customer->item_prob[39] += 20;  // �߿���̸� Ŀ���� ���� Ȯ�� +20%
            if (customer->item_prob[39] > 100) customer->item_prob[39] = 100;
        }

        current = current->next;
    }
}

// ���� ��� ����Ʈ�� ������ �߰� �Լ�
void add_purchase_summary(PurchaseList* list, int item_id, const char* item_name, int quantity, float price) {
    PurchaseNode* current = list->head;
    while (current != NULL) {
        if (current->summary.item_id == item_id) {
            current->summary.sold_quantity += quantity;
            return;
        }
        current = current->next;
    }

    PurchaseNode* new_node = (PurchaseNode*)malloc(sizeof(PurchaseNode));
    if (new_node == NULL) {
        fprintf_s(stderr, "�޸� �Ҵ� ����\n");
        exit(1); // �޸� �Ҵ� ���� �� ���α׷� ����
    }
    new_node->summary.item_id = item_id;
    strcpy_s(new_node->summary.item_name, sizeof(new_node->summary.item_name), item_name);
    new_node->summary.sold_quantity = quantity;
    new_node->summary.price = price;
    new_node->next = list->head;
    list->head = new_node;
}

// ���� ��� ����Ʈ �ʱ�ȭ �Լ�
void initialize_purchase_summary(PurchaseList* list) {
    list->head = NULL;
}

// ���� ��� ����Ʈ ��� �Լ�
void show_purchase_summary(PurchaseList* list) {
    PurchaseNode* current = list->head;
    printf("�� ���� ���:\n");
    while (current != NULL) {
        printf("��ǰ ID: %d, �̸�: %s, �Ǹ� ����: %d\n",
            current->summary.item_id, current->summary.item_name, current->summary.sold_quantity);
        current = current->next;
    }

    float student_expense = 0, office_worker_expense = 0, housewife_expense = 0;
    current = list->head;
    while (current != NULL) {
        if (current->summary.item_id >= 6 && current->summary.item_id <= 18) {  // ���� �� �����
            student_expense += current->summary.sold_quantity * current->summary.price;
        }
        if ((current->summary.item_id >= 1 && current->summary.item_id <= 5) || (current->summary.item_id >= 19 && current->summary.item_id <= 21)) {  // �Ǿ�ǰ �� ��
            office_worker_expense += current->summary.sold_quantity * current->summary.price;
        }
        if ((current->summary.item_id >= 27 && current->summary.item_id <= 41) || (current->summary.item_id >= 42 && current->summary.item_id <= 50)) {  // ��Ȱ ��ǰ �� �ķ�ǰ
            housewife_expense += current->summary.sold_quantity * current->summary.price;
        }
        current = current->next;
    }
    printf("�л� ����: %.2f��, ������ ����: %.2f��, �ֺ� ����: %.2f��\n", student_expense, office_worker_expense, housewife_expense);
}

// ���� ��� ����Ʈ �ʱ�ȭ �Լ�
void clear_purchase_summary(PurchaseList* list) {
    PurchaseNode* current = list->head;
    while (current != NULL) {
        PurchaseNode* temp = current;
        current = current->next;
        free(temp);
    }
    list->head = NULL;
}

// �ùķ��̼� �Լ�
void run_simulation_time_period(CustomerList* customerList, ItemList* itemList, PurchaseList* purchaseList, int num_items, const char* weather, const char* temperature, const char* time_period) {
    int customers_visited = 0;
    float time_period_profit = 0;
    float student_expense = 0, office_worker_expense = 0, housewife_expense = 0;

    printf("\n���� �ð���: %s\n", time_period);  // ���� �ð��� ǥ��

    adjust_probabilities(customerList, weather, temperature);

    CustomerNode* customerNode = customerList->head;
    while (customerNode != NULL) {
        Customer* customer = &customerNode->customer;

        if ((rand() % 100 + 1) <= customer->visit_prob) {
            customers_visited++;
            if ((rand() % 100 + 1) <= customer->buy_prob) {
                for (int i = 0; i < num_items; i++) {
                    ItemNode* itemNode = find_item(itemList, i + 1);
                    if (itemNode != NULL && (rand() % 100 + 1) <= customer->item_prob[i]) {
                        if (itemNode->item.quantity > 0) {
                            itemNode->item.quantity--;
                            time_period_profit += itemNode->item.price;

                            
                            add_purchase_summary(purchaseList, itemNode->item.id, itemNode->item.name, 1, itemNode->item.price);
                        }
                    }
                }
            }
        }

        int min_prob = 1;
        if (strcmp(customer->type, "�л�") == 0) {
            for (int i = 0; i < num_items; i++) {
                min_prob = (i >= 5 && i <= 18) ? 20 : 1;
                update_probability(&customer->item_prob[i], min_prob);
            }
        }
        else if (strcmp(customer->type, "������") == 0) {
            for (int i = 0; i < num_items; i++) {
                min_prob = (i >= 0 && i <= 4) || (i >= 18 && i <= 20) ? 15 : 1;
                update_probability(&customer->item_prob[i], min_prob);
            }
        }
        else if (strcmp(customer->type, "�ֺ�") == 0) {
            for (int i = 0; i < num_items; i++) {
                min_prob = (i >= 26 && i <= 50) ? 20 : 1;
                update_probability(&customer->item_prob[i], min_prob);
            }
        }

        customerNode = customerNode->next;
    }

    daily_profit += time_period_profit;
    printf("%s �ð��� �湮�� �� ��: %d\n", time_period, customers_visited);
    printf("%s �ð��� ����: %.2f��\n", time_period, time_period_profit);
}
// ��� Ȯ�� �Լ�
void show_inventory(ItemList* itemList) {
    printf("��� ���:\n");

    // �Ǿ�ǰ
    printf("\n�Ǿ�ǰ:\n");
    ItemNode* current = itemList->head;
    while (current != NULL) {
        if (current->item.id >= 1 && current->item.id <= 5) {
            printf("ID: %d, �̸�: %s, ����: %d, ����: %.2f��\n",
                current->item.id, current->item.name, current->item.quantity, current->item.price);
        }
        current = current->next;
    }

    // ���� �� �����
    printf("\n���� �� �����:\n");
    current = itemList->head;
    while (current != NULL) {
        if (current->item.id >= 6 && current->item.id <= 18) {
            printf("ID: %d, �̸�: %s, ����: %d, ����: %.2f��\n",
                current->item.id, current->item.name, current->item.quantity, current->item.price);
        }
        current = current->next;
    }

    // ��
    printf("\n��:\n");
    current = itemList->head;
    while (current != NULL) {
        if (current->item.id >= 19 && current->item.id <= 21) {
            printf("ID: %d, �̸�: %s, ����: %d, ����: %.2f��\n",
                current->item.id, current->item.name, current->item.quantity, current->item.price);
        }
        current = current->next;
    }

    // ���� ����
    printf("\n���� ����:\n");
    current = itemList->head;
    while (current != NULL) {
        if (current->item.id >= 22 && current->item.id <= 26) {
            printf("ID: %d, �̸�: %s, ����: %d, ����: %.2f��\n",
                current->item.id, current->item.name, current->item.quantity, current->item.price);
        }
        current = current->next;
    }

    // ��Ȱ ��ǰ
    printf("\n��Ȱ ��ǰ:\n");
    current = itemList->head;
    while (current != NULL) {
        if (current->item.id >= 27 && current->item.id <= 41) {
            printf("ID: %d, �̸�: %s, ����: %d, ����: %.2f��\n",
                current->item.id, current->item.name, current->item.quantity, current->item.price);
        }
        current = current->next;
    }

    // �ķ�ǰ
    printf("\n�ķ�ǰ:\n");
    current = itemList->head;
    while (current != NULL) {
        if (current->item.id >= 42 && current->item.id <= 50) {
            printf("ID: %d, �̸�: %s, ����: %d, ����: %.2f��\n",
                current->item.id, current->item.name, current->item.quantity, current->item.price);
        }
        current = current->next;
    }
}

// �ܺ� ���� ���� �Լ�
void generate_random_weather(char* weather) {
    const char* weather_conditions[] = { "sunny", "rain", "snow" };
    int index = rand() % 3;
    strcpy_s(weather, 10, weather_conditions[index]);
}

void generate_random_temperature(char* temperature) {
    const char* temperature_conditions[] = { "hot", "cold", "mild" };
    int index = rand() % 3;
    strcpy_s(temperature, 10, temperature_conditions[index]);
}

// ���Ͽ� ���� ���� Ȯ�� ��ȭ
void adjust_probabilities_for_weekend(CustomerList* customerList, const char* day_of_week) {
    CustomerNode* current = customerList->head;
    while (current != NULL) {
        Customer* customer = &current->customer;

        if (strcmp(day_of_week, "�ݿ���") == 0 || strcmp(day_of_week, "�����") == 0) {
            customer->item_prob[18] += 20;  // ����
            customer->item_prob[19] += 20;  // ����
            customer->item_prob[20] += 20;  // ����

            if (customer->item_prob[18] > 100) customer->item_prob[18] = 100;
            if (customer->item_prob[19] > 100) customer->item_prob[19] = 100;
            if (customer->item_prob[20] > 100) customer->item_prob[20] = 100;
        }

        if (strcmp(day_of_week, "�����") == 0 || strcmp(day_of_week, "�Ͽ���") == 0) {
            for (int i = 5; i < 18; i++) { // ���ڿ� �����
                customer->item_prob[i] += 10;
                if (customer->item_prob[i] > 100) customer->item_prob[i] = 100;
            }
        }

        current = current->next;
    }
}


// ��� ���� �Լ�
void restock_inventory(ItemList* itemList, int item_id, int quantity) {
    ItemNode* itemNode = find_item(itemList, item_id);
    if (itemNode != NULL) {
        float cost = itemNode->item.price * quantity * 0.8f;  // ��ǰ ��� ����� �ǸŰ��� 80%
        if (capital >= cost) {
            itemNode->item.quantity += quantity;
            capital -= cost;
            printf("��� ���������� ����Ǿ����ϴ�. ���: %.2f��, ���� �ں�: %.2f��\n", cost, capital);
        }
        else {
            printf("�ں��� �����Ͽ� ��� ������ �� �����ϴ�.\n");
        }
    }
    else {
        printf("�ش� ID�� ��ǰ�� ã�� �� �����ϴ�.\n");
    }
}

// ���� �Լ�
int main() {
    srand((unsigned int)time(NULL)); // ���� �õ� ����

    ItemList itemList = { NULL };
    CustomerList customerList = { NULL };
    PurchaseList purchaseList;

    initialize_items(&itemList);
    initialize_customers(&customerList, 50);
    initialize_purchase_summary(&purchaseList);

    printf("������ � �ùķ��̼��� �����մϴ�.\n");
    printf("'S'�� ������ �����ϼ���: ");
    char command = _getch();

    if (command != 'S' && command != 's') {
        printf("\n��ȿ���� ���� ��ɾ��Դϴ�. ���α׷��� �����մϴ�.\n");
        return 0;
    }

    int time_period_index = 0;  // 0: ��ħ, 1: ����, 2: ����
    const char* days_of_week[] = { "������", "ȭ����", "������", "�����", "�ݿ���", "�����", "�Ͽ���" };
    int day_index = 0;


    for (int day = 1; day <= 10; day++) { 
        printf("\n--- %d���� (%s) ---\n", day, days_of_week[day_index]);

        char weather[10];
        char temperature[10];

        generate_random_weather(weather);
        generate_random_temperature(temperature);

        printf("����: %s, ���: %s\n", weather, temperature);

        adjust_probabilities_for_weekend(&customerList, days_of_week[day_index]);

        daily_profit = 0; // ���� ���� �ʱ�ȭ

        while (1) {
            printf("\n'L'�� ������ ��� Ȯ���ϼ���. 'R'�� ������ ��� �����ϼ���. 'N'�� ������ ���� �ð���� �����ϼ���. 'P'�� ������ ���� ����� Ȯ���ϼ���: ");
            command = getchar();
            getchar();

            if (command == 'L' || command == 'l') {
                show_inventory(&itemList);
            }
            else if (command == 'R' || command == 'r') {
                int item_id, quantity;
                printf("��� ������ ��ǰ�� ID�� ������ �Է��ϼ��� (��: 1 10): ");
                scanf_s("%d %d", &item_id, &quantity);
                getchar();
                restock_inventory(&itemList, item_id, quantity);
            }
            else if (command == 'N' || command == 'n') {
                const char* time_periods[] = { "��ħ", "����", "����" };
                run_simulation_time_period(&customerList, &itemList, &purchaseList, 50, weather, temperature, time_periods[time_period_index]);

                time_period_index = (time_period_index + 1) % 3;

                if (time_period_index == 0) {
                    total_profit += daily_profit;  // ���� ���Ϳ� ���� ���� �߰�
                    printf("������ �� ����: %.2f��\n", daily_profit);
                    daily_profit = 0;

                    day_index = (day_index + 1) % 7;
                    show_purchase_summary(&purchaseList);
                    clear_purchase_summary(&purchaseList);
                    break; 
                }
                else {
                    printf("���� �ں�: %.2f��\n", capital);
                }
            }
            else if (command == 'P' || command == 'p') {
                show_purchase_summary(&purchaseList);
            }
            else {
                printf("��ȿ���� ���� ��ɾ��Դϴ�. 'L', 'R', 'N' �Ǵ� 'P'�� �Է��ϼ���.\n");
            }
        }
    }
    printf("������ � �ùķ��̼��� ����Ǿ����ϴ�.\n");
    printf("���� ����: %.2f��\n", total_profit);  // ���� ���͸� ǥ��
    printf("���� �ں�: %.2f��\n", capital);

    return 0;
}

