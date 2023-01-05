/**/
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define TRUE 1
#define FALSE 0

typedef struct _Process {
    int type;
    int process_id;                   // ���μ��� ���̵�
    int priority;                       // ���μ��� �켱����
    int computing_time;         // ���μ��� ���� Ÿ��
    int turn_around_time;       // ���μ����� ���ð�, ���μ����� �Էµ� �ð����� ������� ������ �ð�
} Process;

typedef struct _HRRN_Process {
    Process* prc;                    //�Է¹��� ���μ��� ����

    int inTime;                        // ���� �ð�
    float R;                                 //HRRN �� �ʿ��� ���μ����� �������    
    int W;                                 //HRRN �� �ʿ��� ���μ��� ��� �ð�

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
    
    //�Լ� ������������ �Է¹޴� �ð��� ���� �ߴ�.
    s = clock();
    exeProcess(&start);
    e = clock();

    res1 = (float)(e - s) / CLOCKS_PER_SEC;
    printf(" �Լ� �ҿ�� �ð� : %.3f \n", res1);



    return 0;
}

void make_LinkedList(HRRN_Process** start) {
    HRRN_Process* left = NULL;
    HRRN_Process* cur = NULL;
    char temp;   //enter�� �ֱ����� �ӽ� �����
    int scnftmp;  //scanf �������� ���� ���������
    int time = 1;
    int type = 0, process_id = 0, priority = 0, computing_time = 0;

    printf("�Է�- \n");
    while (TRUE) {
        scnftmp = scanf("%d", &type);
        if (type == -1) break;
        scnftmp = scanf("%d %d %d", &process_id, &priority, &computing_time);
   
        if (type == 0) {
            if (!*start) {             // start�� �ƹ��͵� ������(ó������� ���μ�����)
                *start = make_HRRN_process(NULL, type, process_id, priority, computing_time, time);
                left = *start;
            }
            else {
                cur = make_HRRN_process(left, type, process_id, priority, computing_time, time);
                left = cur;
            }
        }
        else if (type == 1) {
            //type == 1 �� ������ �� ���� �������Ƿ� ���μ����� �������� �ʰ� �ð��� ���� �Ѵ�.
            time++;
        }
        else {
            printf("�߸� �Է���");
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

    // �Է½ð��� ���� ���μ������� 
    while (cur!= NULL && cur ->inTime <= cur_time) {
        cur->R = (float)((cur_time - cur->inTime) - (cur->prc->computing_time)) / cur->prc->computing_time;
        cur->prc->turn_around_time = cur_time - cur->inTime;
        cur = cur->right;
    }
}

HRRN_Process* choiceProcessF(HRRN_Process** start, int cur_time) {

    //R�� ��� ����
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

