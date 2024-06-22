#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h> // <Enter> 입력게 하는

// 물품 정보를 저장하는 구조체 정의
typedef struct {
    int id;
    char name[50];
    int quantity;
    float price;
} Item;

// 물품 리스트 노드 정의
typedef struct ItemNode {
    Item item;
    struct ItemNode* next;
} ItemNode;

// 물품 리스트 정의
typedef struct {
    ItemNode* head;
} ItemList;

// 고객 정보를 저장하는 구조체 정의
typedef struct {
    int visit_prob;
    int buy_prob;
    int item_prob[50];
    char type[20];  // 고객 유형 (학생, 직장인, 주부)
} Customer;

// 고객 리스트 노드 정의
typedef struct CustomerNode {
    Customer customer;
    struct CustomerNode* next;
} CustomerNode;

// 고객 리스트 정의
typedef struct {
    CustomerNode* head;
} CustomerList;

float capital = 1000000;  // 초기 자본 100만원
float daily_profit = 0;  // 일일 수익
float total_profit = 0;  // 총 수익

typedef struct {
    int item_id;
    char item_name[50];
    int sold_quantity;
    float price;
} PurchaseSummary;

// 구매 요약 리스트 노드 정의
typedef struct PurchaseNode {
    PurchaseSummary summary;
    struct PurchaseNode* next;
} PurchaseNode;

// 구매 요약 리스트 정의
typedef struct {
    PurchaseNode* head;
} PurchaseList;

//초기 물품 리스트
void initialize_items(ItemList* itemList) {
    Item items[50] = {
        // 의약품
        {1, "진통제", 20, 3500},
        {2, "기침약", 20, 3500},
        {3, "반창고", 50, 1000},
        {4, "마스크", 50, 5000},
        {5, "손소독제", 30, 4000},
        // 과자 및 음료수
        {6, "새우깡", 40, 1500},
        {7, "포테이토 칩", 40, 1700},
        {8, "초콜릿", 50, 1800},
        {9, "사탕", 50, 500},
        {10, "껌", 50, 500},
        {11, "사이다", 30, 1500},
        {12, "콜라", 30, 1500},
        {13, "오렌지 주스", 30, 2500},
        {14, "사과 주스", 30, 2500},
        {15, "탄산수", 40, 1500},
        {16, "에너지 드링크", 50, 1500},
        {17, "비요뜨", 15, 2000},
        {18, "포켓몬 빵", 10, 1700},
        // 술
        {19, "맥주", 20, 3700},
        {20, "소주", 20, 2500},
        {21, "와인", 20, 5000},
        // 간편 음식
        {22, "샌드위치", 30, 3000},
        {23, "라면", 40, 900},
        {24, "도시락", 15, 4500},
        {25, "샐러드", 20, 1500},
        {26, "아이스 크림", 60, 2100},
        // 생활 용품
        {27, "우산", 10, 5000},
        {28, "생수", 50, 1000},
        {29, "칫솔", 20, 2500},
        {30, "치약", 15, 3000},
        {31, "샴푸", 15, 5000},
        {32, "바디 워시", 15, 5500},
        {33, "담배", 30, 4500},
        {34, "비누", 25, 2700},
        {35, "화장지", 30, 500},
        {36, "종이 타월", 20, 2500},
        {37, "물티슈", 40, 4000},
        {38, "세제", 20, 10000},
        {39, "섬유 유연제", 20, 6200},
        {40, "면도 크림", 15, 4000},
        {41, "면도기", 20, 5000},
        // 식료품
        {42, "우유", 30, 3000},
        {43, "요구르트", 40, 2000},
        {44, "식빵", 15, 2000},
        {45, "계란", 30, 2000},
        {46, "버터", 20, 2000},
        {47, "커피", 20, 2000},
        {48, "커피 우유", 20, 1700},
        {49, "사과", 10, 2000},
        {50, "바나나", 10, 2000}
    };

    for (int i = 0; i < 50; i++) {
        ItemNode* new_node = (ItemNode*)malloc(sizeof(ItemNode));
        if (new_node == NULL) {
            fprintf(stderr, "메모리 할당 실패\n");
            exit(1);
        }
        new_node->item = items[i];
        new_node->next = itemList->head;
        itemList->head = new_node;
    }
}

// 물품의 갯수나 정보를 확인하는 함수
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

// 고객 유형에 따른 구매 확률 설정
void set_initial_probabilities(Customer* customer) {
    if (strcmp(customer->type, "학생") == 0) {
        for (int i = 0; i < 50; i++) {
            if (i >= 5 && i <= 18) { 
                customer->item_prob[i] = rand() % 61 + 20;  // 최소 20% 이상, 최대 80%
            }
            else {
                customer->item_prob[i] = rand() % 81;  // 최대 80%
            }
        }
    }
    else if (strcmp(customer->type, "직장인") == 0) {
        for (int i = 0; i < 50; i++) {
            if (i >= 0 && i <= 4) {  // 의약품 
                customer->item_prob[i] = rand() % 66 + 15;  // 최소 15% 이상, 최대 80%
            }
            else if (i >= 18 && i <= 20) {  // 술 
                customer->item_prob[i] = rand() % 66 + 15;  // 최소 15% 이상, 최대 80%
            }
            else {
                customer->item_prob[i] = rand() % 81;  // 최대 80%
            }
        }
    }
    else if (strcmp(customer->type, "주부") == 0) {
        for (int i = 0; i < 50; i++) {
            if (i >= 26 && i <= 50) {  // 생활용품과 식료품
                customer->item_prob[i] = rand() % 61 + 20;  // 최소 20% 이상, 최대 80%
            }
            else {
                customer->item_prob[i] = rand() % 81;  // 최대 80%
            }
        }
    }
}

void initialize_customers(CustomerList* customerList, int num_customers) {
    srand((unsigned int)time(NULL));

    const char* types[] = { "학생", "직장인", "주부" };
    int type_counts[] = { 17, 17, 16 };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < type_counts[i]; j++) {
            Customer customer;
            customer.visit_prob = rand() % 80 + 1;
            customer.buy_prob = rand() % 80 + 1;
            strcpy_s(customer.type, sizeof(customer.type), types[i]);
            set_initial_probabilities(&customer);

            // 직접 노드를 생성하여 리스트에 추가
            CustomerNode* new_node = (CustomerNode*)malloc(sizeof(CustomerNode));
            if (new_node == NULL) {
                fprintf(stderr, "메모리 할당 실패\n");
                exit(1); // 메모리 할당 실패 시 프로그램 종료
            }
            new_node->customer = customer;
            new_node->next = customerList->head;
            customerList->head = new_node;
        }
    }
}

// 확률 갱신 함수
void update_probability(int* prob, int min_prob) {
    int change = (rand() % 41) - 20;
    *prob += change;
    if (*prob < min_prob) *prob = min_prob;
    if (*prob > 100) *prob = 80;
}

// 외부 변수에 따른 확률 조정 함수
void adjust_probabilities(CustomerList* customerList, const char* weather, const char* temperature) {
    CustomerNode* current = customerList->head;

    while (current != NULL) {
        Customer* customer = &current->customer;

        if (strcmp(weather, "rain") == 0) {
            customer->item_prob[18] += 20;  // 비가오면 우산 구매 확률 +20%
            if (customer->item_prob[18] > 100) customer->item_prob[18] = 100;
        }

        if (strcmp(temperature, "hot") == 0) { 
            customer->item_prob[17] += 20;  // 더운날씨이면 아이스크림 구매 확률 +20%
            if (customer->item_prob[17] > 100) customer->item_prob[17] = 100;
        }
        else if (strcmp(temperature, "cold") == 0) {
            customer->item_prob[39] += 20;  // 추운날씨이면 커피의 구매 확률 +20%
            if (customer->item_prob[39] > 100) customer->item_prob[39] = 100;
        }

        current = current->next;
    }
}

// 구매 요약 리스트에 아이템 추가 함수
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
        fprintf_s(stderr, "메모리 할당 실패\n");
        exit(1); // 메모리 할당 실패 시 프로그램 종료
    }
    new_node->summary.item_id = item_id;
    strcpy_s(new_node->summary.item_name, sizeof(new_node->summary.item_name), item_name);
    new_node->summary.sold_quantity = quantity;
    new_node->summary.price = price;
    new_node->next = list->head;
    list->head = new_node;
}

// 구매 요약 리스트 초기화 함수
void initialize_purchase_summary(PurchaseList* list) {
    list->head = NULL;
}

// 구매 요약 리스트 출력 함수
void show_purchase_summary(PurchaseList* list) {
    PurchaseNode* current = list->head;
    printf("고객 구매 요약:\n");
    while (current != NULL) {
        printf("물품 ID: %d, 이름: %s, 판매 수량: %d\n",
            current->summary.item_id, current->summary.item_name, current->summary.sold_quantity);
        current = current->next;
    }

    float student_expense = 0, office_worker_expense = 0, housewife_expense = 0;
    current = list->head;
    while (current != NULL) {
        if (current->summary.item_id >= 6 && current->summary.item_id <= 18) {  // 과자 및 음료수
            student_expense += current->summary.sold_quantity * current->summary.price;
        }
        if ((current->summary.item_id >= 1 && current->summary.item_id <= 5) || (current->summary.item_id >= 19 && current->summary.item_id <= 21)) {  // 의약품 및 술
            office_worker_expense += current->summary.sold_quantity * current->summary.price;
        }
        if ((current->summary.item_id >= 27 && current->summary.item_id <= 41) || (current->summary.item_id >= 42 && current->summary.item_id <= 50)) {  // 생활 용품 및 식료품
            housewife_expense += current->summary.sold_quantity * current->summary.price;
        }
        current = current->next;
    }
    printf("학생 지출: %.2f원, 직장인 지출: %.2f원, 주부 지출: %.2f원\n", student_expense, office_worker_expense, housewife_expense);
}

// 구매 요약 리스트 초기화 함수
void clear_purchase_summary(PurchaseList* list) {
    PurchaseNode* current = list->head;
    while (current != NULL) {
        PurchaseNode* temp = current;
        current = current->next;
        free(temp);
    }
    list->head = NULL;
}

// 시뮬레이션 함수
void run_simulation_time_period(CustomerList* customerList, ItemList* itemList, PurchaseList* purchaseList, int num_items, const char* weather, const char* temperature, const char* time_period) {
    int customers_visited = 0;
    float time_period_profit = 0;
    float student_expense = 0, office_worker_expense = 0, housewife_expense = 0;

    printf("\n현재 시간대: %s\n", time_period);  // 현재 시간대 표시

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
        if (strcmp(customer->type, "학생") == 0) {
            for (int i = 0; i < num_items; i++) {
                min_prob = (i >= 5 && i <= 18) ? 20 : 1;
                update_probability(&customer->item_prob[i], min_prob);
            }
        }
        else if (strcmp(customer->type, "직장인") == 0) {
            for (int i = 0; i < num_items; i++) {
                min_prob = (i >= 0 && i <= 4) || (i >= 18 && i <= 20) ? 15 : 1;
                update_probability(&customer->item_prob[i], min_prob);
            }
        }
        else if (strcmp(customer->type, "주부") == 0) {
            for (int i = 0; i < num_items; i++) {
                min_prob = (i >= 26 && i <= 50) ? 20 : 1;
                update_probability(&customer->item_prob[i], min_prob);
            }
        }

        customerNode = customerNode->next;
    }

    daily_profit += time_period_profit;
    printf("%s 시간대 방문한 고객 수: %d\n", time_period, customers_visited);
    printf("%s 시간대 수익: %.2f원\n", time_period, time_period_profit);
}
// 재고 확인 함수
void show_inventory(ItemList* itemList) {
    printf("재고 목록:\n");

    // 의약품
    printf("\n의약품:\n");
    ItemNode* current = itemList->head;
    while (current != NULL) {
        if (current->item.id >= 1 && current->item.id <= 5) {
            printf("ID: %d, 이름: %s, 수량: %d, 가격: %.2f원\n",
                current->item.id, current->item.name, current->item.quantity, current->item.price);
        }
        current = current->next;
    }

    // 과자 및 음료수
    printf("\n과자 및 음료수:\n");
    current = itemList->head;
    while (current != NULL) {
        if (current->item.id >= 6 && current->item.id <= 18) {
            printf("ID: %d, 이름: %s, 수량: %d, 가격: %.2f원\n",
                current->item.id, current->item.name, current->item.quantity, current->item.price);
        }
        current = current->next;
    }

    // 술
    printf("\n술:\n");
    current = itemList->head;
    while (current != NULL) {
        if (current->item.id >= 19 && current->item.id <= 21) {
            printf("ID: %d, 이름: %s, 수량: %d, 가격: %.2f원\n",
                current->item.id, current->item.name, current->item.quantity, current->item.price);
        }
        current = current->next;
    }

    // 간편 음식
    printf("\n간편 음식:\n");
    current = itemList->head;
    while (current != NULL) {
        if (current->item.id >= 22 && current->item.id <= 26) {
            printf("ID: %d, 이름: %s, 수량: %d, 가격: %.2f원\n",
                current->item.id, current->item.name, current->item.quantity, current->item.price);
        }
        current = current->next;
    }

    // 생활 용품
    printf("\n생활 용품:\n");
    current = itemList->head;
    while (current != NULL) {
        if (current->item.id >= 27 && current->item.id <= 41) {
            printf("ID: %d, 이름: %s, 수량: %d, 가격: %.2f원\n",
                current->item.id, current->item.name, current->item.quantity, current->item.price);
        }
        current = current->next;
    }

    // 식료품
    printf("\n식료품:\n");
    current = itemList->head;
    while (current != NULL) {
        if (current->item.id >= 42 && current->item.id <= 50) {
            printf("ID: %d, 이름: %s, 수량: %d, 가격: %.2f원\n",
                current->item.id, current->item.name, current->item.quantity, current->item.price);
        }
        current = current->next;
    }
}

// 외부 변수 생성 함수
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

// 요일에 따른 구매 확률 변화
void adjust_probabilities_for_weekend(CustomerList* customerList, const char* day_of_week) {
    CustomerNode* current = customerList->head;
    while (current != NULL) {
        Customer* customer = &current->customer;

        if (strcmp(day_of_week, "금요일") == 0 || strcmp(day_of_week, "토요일") == 0) {
            customer->item_prob[18] += 20;  // 맥주
            customer->item_prob[19] += 20;  // 소주
            customer->item_prob[20] += 20;  // 와인

            if (customer->item_prob[18] > 100) customer->item_prob[18] = 100;
            if (customer->item_prob[19] > 100) customer->item_prob[19] = 100;
            if (customer->item_prob[20] > 100) customer->item_prob[20] = 100;
        }

        if (strcmp(day_of_week, "토요일") == 0 || strcmp(day_of_week, "일요일") == 0) {
            for (int i = 5; i < 18; i++) { // 과자와 음료수
                customer->item_prob[i] += 10;
                if (customer->item_prob[i] > 100) customer->item_prob[i] = 100;
            }
        }

        current = current->next;
    }
}


// 재고 보충 함수
void restock_inventory(ItemList* itemList, int item_id, int quantity) {
    ItemNode* itemNode = find_item(itemList, item_id);
    if (itemNode != NULL) {
        float cost = itemNode->item.price * quantity * 0.8f;  // 물품 재고 비용은 판매가의 80%
        if (capital >= cost) {
            itemNode->item.quantity += quantity;
            capital -= cost;
            printf("재고가 성공적으로 보충되었습니다. 비용: %.2f원, 남은 자본: %.2f원\n", cost, capital);
        }
        else {
            printf("자본이 부족하여 재고를 보충할 수 없습니다.\n");
        }
    }
    else {
        printf("해당 ID의 물품을 찾을 수 없습니다.\n");
    }
}

// 메인 함수
int main() {
    srand((unsigned int)time(NULL)); // 랜덤 시드 설정

    ItemList itemList = { NULL };
    CustomerList customerList = { NULL };
    PurchaseList purchaseList;

    initialize_items(&itemList);
    initialize_customers(&customerList, 50);
    initialize_purchase_summary(&purchaseList);

    printf("편의점 운영 시뮬레이션을 시작합니다.\n");
    printf("'S'를 눌러서 시작하세요: ");
    char command = _getch();

    if (command != 'S' && command != 's') {
        printf("\n유효하지 않은 명령어입니다. 프로그램을 종료합니다.\n");
        return 0;
    }

    int time_period_index = 0;  // 0: 아침, 1: 점심, 2: 저녁
    const char* days_of_week[] = { "월요일", "화요일", "수요일", "목요일", "금요일", "토요일", "일요일" };
    int day_index = 0;


    for (int day = 1; day <= 10; day++) { 
        printf("\n--- %d일차 (%s) ---\n", day, days_of_week[day_index]);

        char weather[10];
        char temperature[10];

        generate_random_weather(weather);
        generate_random_temperature(temperature);

        printf("날씨: %s, 기온: %s\n", weather, temperature);

        adjust_probabilities_for_weekend(&customerList, days_of_week[day_index]);

        daily_profit = 0; // 일일 수익 초기화

        while (1) {
            printf("\n'L'을 눌러서 재고를 확인하세요. 'R'을 눌러서 재고를 보충하세요. 'N'을 눌러서 다음 시간대로 진행하세요. 'P'를 눌러서 구매 요약을 확인하세요: ");
            command = getchar();
            getchar();

            if (command == 'L' || command == 'l') {
                show_inventory(&itemList);
            }
            else if (command == 'R' || command == 'r') {
                int item_id, quantity;
                printf("재고를 보충할 물품의 ID와 수량을 입력하세요 (예: 1 10): ");
                scanf_s("%d %d", &item_id, &quantity);
                getchar();
                restock_inventory(&itemList, item_id, quantity);
            }
            else if (command == 'N' || command == 'n') {
                const char* time_periods[] = { "아침", "점심", "저녁" };
                run_simulation_time_period(&customerList, &itemList, &purchaseList, 50, weather, temperature, time_periods[time_period_index]);

                time_period_index = (time_period_index + 1) % 3;

                if (time_period_index == 0) {
                    total_profit += daily_profit;  // 최종 수익에 일일 수익 추가
                    printf("오늘의 총 수익: %.2f원\n", daily_profit);
                    daily_profit = 0;

                    day_index = (day_index + 1) % 7;
                    show_purchase_summary(&purchaseList);
                    clear_purchase_summary(&purchaseList);
                    break; 
                }
                else {
                    printf("현재 자본: %.2f원\n", capital);
                }
            }
            else if (command == 'P' || command == 'p') {
                show_purchase_summary(&purchaseList);
            }
            else {
                printf("유효하지 않은 명령어입니다. 'L', 'R', 'N' 또는 'P'를 입력하세요.\n");
            }
        }
    }
    printf("편의점 운영 시뮬레이션이 종료되었습니다.\n");
    printf("최종 수익: %.2f원\n", total_profit);  // 최종 수익만 표시
    printf("최종 자본: %.2f원\n", capital);

    return 0;
}

