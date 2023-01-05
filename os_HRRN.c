/**/
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define TRUE 1
#define FALSE 0

typedef struct _Process {
    int type;
    int process_id;                   // 프로세스 아이디
    int priority;                       // 프로세스 우선순위
    int computing_time;         // 프로세스 서비스 타임
    int turn_around_time;       // 프로세스의 대기시간, 프로세스가 입력된 시간부터 현재까지 지나온 시간
} Process;

typedef struct _HRRN_Process {
    Process* prc;                    //입력받은 프로세스 정보

    int inTime;                        // 들어온 시간
    float R;                                 //HRRN 에 필요한 프로세스의 응답비율    
    int W;                                 //HRRN 에 필요한 프로세스 대기 시간

    struct _HRRN_Process* left;
    struct _HRRN_Process* right;

} HRRN_Process;

void make_LinkedList(HRRN_Process** start);

Process* make_process(int type, int process_id, int priority, int computing_time);

HRRN_Process* make_HRRN_process(HRRN_Process* left, int type, int process_id, int priority, int computing_time, int time);

void calcR(HRRN_Process** start, int cur_time);

HRRN_Process* choiceProcessF(HRRN_Process** start, int cur_time);

void exeProcess(HRRN_Process** start);



int main() {
    HRRN_Process* start = NULL;
    clock_t s, e;
    float res1;

    make_LinkedList(&start);
    
    //함수 성능측정에서 입력받는 시간은 제외 했다.
    s = clock();
    exeProcess(&start);
    e = clock();

    res1 = (float)(e - s) / CLOCKS_PER_SEC;
    printf(" 함수 소요된 시간 : %.3f \n", res1);



    return 0;
}

void make_LinkedList(HRRN_Process** start) {
    HRRN_Process* left = NULL;
    HRRN_Process* cur = NULL;
    char temp;   //enter를 넣기위한 임시 저장소
    int scnftmp;  //scanf 에러나서 넣은 더미저장소
    int time = 1;
    int type = 0, process_id = 0, priority = 0, computing_time = 0;

    printf("입력- \n");
    while (TRUE) {
        scnftmp = scanf("%d", &type);
        if (type == -1) break;
        scnftmp = scanf("%d %d %d", &process_id, &priority, &computing_time);
   
        if (type == 0) {
            if (!*start) {             // start에 아무것도 없으면(처음만드는 프로세스면)
                *start = make_HRRN_process(NULL, type, process_id, priority, computing_time, time);
                left = *start;
            }
            else {
                cur = make_HRRN_process(left, type, process_id, priority, computing_time, time);
                left = cur;
            }
        }
        else if (type == 1) {
            //type == 1 이 들어오면 한 텀이 지났으므로 프로세스는 생성하지 않고 시간을 증가 한다.
            time++;
        }
        else {
            printf("잘못 입력함");
        }
    }
}

Process* make_process(int type, int process_id, int priority, int computing_time) {
    Process* _new = (Process*)malloc(sizeof(Process) * 1);
    _new->type = type;
    _new->process_id = process_id;
    _new->priority = priority;
    _new->computing_time = computing_time;

    return _new;
}

HRRN_Process* make_HRRN_process(HRRN_Process* left, int type, int process_id, int priority, int computing_time, int time) {
    Process* _prcNew = make_process(type, process_id, priority, computing_time);
    HRRN_Process* _hrrnNew = (HRRN_Process*)malloc(sizeof(Process) * 1);

    _hrrnNew->inTime = time;
    _hrrnNew->prc = _prcNew;
    _hrrnNew->left = left;
    _hrrnNew->right = NULL;

    if (left != NULL) {
        left->right = _hrrnNew;
    }



    return _hrrnNew;
}

void calcR(HRRN_Process** start, int cur_time) {
    HRRN_Process* cur = *start;

    // 입력시간이 적은 프로세스부터 
    while (cur!= NULL && cur ->inTime <= cur_time) {
        cur->R = (float)((cur_time - cur->inTime) - (cur->prc->computing_time)) / cur->prc->computing_time;
        cur->prc->turn_around_time = cur_time - cur->inTime;
        cur = cur->right;
    }
}

HRRN_Process* choiceProcessF(HRRN_Process** start, int cur_time) {

    //R을 모두 갱신
    calcR(start, cur_time);
    HRRN_Process* choiceProcess = NULL;
    HRRN_Process* cur = NULL;


    choiceProcess = *start;
    
    if (choiceProcess == NULL)
        return NULL;

    cur = choiceProcess->right;
    
    
    while (cur && cur ->inTime <= cur_time) {
        if (cur->R > choiceProcess->R) {
            choiceProcess = cur;
        }
        cur = cur->right;
    }

    if (choiceProcess == *start) {
        if (choiceProcess->right != NULL) {
            choiceProcess->right->left = NULL;
            *start = choiceProcess->right;
        }
        else {
            *start = NULL;
        }
       
    }
    else {
        choiceProcess->left->right = choiceProcess->right;
        if (choiceProcess->right != NULL) {
            choiceProcess->right->left = choiceProcess->left;
        }
    }

    return choiceProcess;

}
void exeProcess(HRRN_Process** start) {
    int Process_id, priority, computing_time, turn_around_time;

    int cur_time = 1;

    HRRN_Process* cur = *start;
    HRRN_Process* choiceProcess;

    printf("Process_id \t priority \t computing_time \t turn_around time\n");



    while (*start) {
        choiceProcess = choiceProcessF(start, cur_time);

        Process_id = choiceProcess->prc->process_id;
        priority = choiceProcess->prc->priority;
        computing_time = choiceProcess->prc->computing_time;
        turn_around_time = choiceProcess->prc->turn_around_time;
        
        printf("%d\t\t   %d\t\t   %d\t\t\t   %d\n", Process_id, priority, computing_time, turn_around_time);
        cur_time += computing_time;
     }
}

