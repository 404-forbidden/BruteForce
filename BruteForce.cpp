// ASCII
// 48~57 : 0~9
// 65~90 : A~Z
// 97~122 : a~z
// printable ASCII : 33~126
#include <iostream>
#include <future>
#include <time.h>
#include <stdlib.h>
#include <conio.h>

#define MAX 8
#define NUMBER_OF_PASSWORD 20
#define EXECUTE_TIME 3600

#define ASCII_START 48
#define ASCII_END 122

#define DIGIT_START 48
#define DIGIT_END 57

#define UPPER_START 65
#define UPPER_END 90

#define LOWER_START 97
#define LOWER_END 122

using namespace std;

int getCommand(); // 키보드 입력 이벤트 : 입력 키 return
void calcCarry(int arr[], int i, int startCode, int endCode); // 올림 재귀 계산
void createPassword(int password[], int digit, int passwordType); // 무작위 비밀번호 생성
bool isEqual(int password[][MAX], int compare[], int digit); // 비밀번호 목록에서 비교
void attackBruteForce(int password[][MAX], int digit); // 브루트포스 : 소요 시간 return

int main() {
    srand(time(NULL));

    int passwordType;
    int password[5][NUMBER_OF_PASSWORD][MAX];
    double totalDuration = 0;

    cout << "비밀번호 유형을 선택하세요(0: 숫자, 1: 숫자+알파벳): ";
    cin >> passwordType;

    clock_t startTime = clock();

    cout << endl << "생성된 비밀번호" << endl;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < NUMBER_OF_PASSWORD; j++) {
            createPassword(password[i][j], MAX - i, passwordType);
        }
        cout << endl;
    }
    cout << endl;
    cout << "진행 상황을 보려면 아무 키나 누르세요" << endl;

    future<void> f4Digits = async(launch::async, attackBruteForce, password[4], 4);
    future<void> f5Digits = async(launch::async, attackBruteForce, password[3], 5);
    future<void> f6Digits = async(launch::async, attackBruteForce, password[2], 6);
    future<void> f7Digits = async(launch::async, attackBruteForce, password[1], 7);
    future<void> f8Digits = async(launch::async, attackBruteForce, password[0], 8);

    return 0;
}

int getCommand() { // 키보드 입력 이벤트 : 입력 키 return
	if (_kbhit()) {
		return _getch();
	}
	 
	return 0;
}

void calcCarry(int arr[], int i, int startCode, int endCode) { // 올림 재귀 계산
    if (arr[i - 1] == endCode) {
        arr[i] = startCode;
        arr[--i] = startCode;
        calcCarry(arr, i, startCode, endCode);
    }
    else {
        if (arr[i - 1] == DIGIT_END) { // 숫자 -> 대문자
            arr[i - 1] = UPPER_START;
        }
        else if (arr[i - 1] == UPPER_END) { // 대문자 -> 소문자
            arr[i - 1] = LOWER_START;
        }
        else {
            arr[i - 1]++;
        }
        arr[i] = startCode;
    }
}

bool isEqual(int password[][MAX], int compare[], int digit) { // 비밀번호 목록에서 비교
    int digitIndex;

    switch (digit) {
        case 8:
            digitIndex = 0;
            break;
        case 7:
            digitIndex = 1;
            break;
        case 6:
            digitIndex = 2;
            break;
        case 5:
            digitIndex = 3;
            break;
        case 4:
            digitIndex = 4;
            break;
        default:
            break;
    }

    for (int i = 0; i < NUMBER_OF_PASSWORD; i++) {
        if (equal(compare, compare + digit, password[i])) {
            cout << "찾은 비밀번호:";
            for (int j = 0; j < digit; j++) {
                cout << (char)password[i][j] << "(" << password[i][j] << ") ";
            }
            cout << endl;
            return true;
        }
    }
    return false;
}

void createPassword(int password[], int digit, int passwordType) { // 무작위 비밀번호 생성
    int startCode, n;

    switch (passwordType) {
        case 0: // 숫자
            startCode = DIGIT_START;
            n = 10;
            break;
        
        case 1: // 숫자+알파벳
            startCode = DIGIT_START;
            n = 75;

        default:
            startCode = ASCII_START;
            n = 75;
            break;
    }

    for (int i = 0; i < digit; i++) {
        do {
            password[i] = rand() % n + startCode;
        } while( (password[i] > DIGIT_END && password[i] < UPPER_START)
        || (password[i] > UPPER_END && password[i] < LOWER_START) );

        cout << (char)password[i];
    }

    if (digit == 8) {
        cout << " ";
    }
    else {
        cout << "\t";
    }
}

void attackBruteForce(int password[][MAX], int digit) { // 브루트포스 : 소요 시간 return
    int arrayEnd = digit - 1;
    int equal = 0;
    int bruteCompare[MAX];
    clock_t startTime, finishTime;
    double duration;

    fill_n(bruteCompare, MAX, ASCII_START);
    startTime = clock();

    while (equal != NUMBER_OF_PASSWORD) {
        if (isEqual(password, bruteCompare, digit)) {
            equal++;
        }

        if (bruteCompare[arrayEnd] == ASCII_END) {
            calcCarry(bruteCompare, arrayEnd, ASCII_START, ASCII_END);
            continue;
        }
        else if (bruteCompare[arrayEnd] == DIGIT_END) { // 숫자 -> 대문자
            bruteCompare[arrayEnd] = UPPER_START;
        }
        else if (bruteCompare[arrayEnd] == UPPER_END) { // 대문자 -> 소문자
            bruteCompare[arrayEnd] = LOWER_START;
        }
        else {
            bruteCompare[arrayEnd]++;
        }

        if(getCommand()) {
            for(int j = 0; j < digit; j++) {
                cout << (char)bruteCompare[j] << "(" << bruteCompare[j] << ") ";
            }
            cout << (double)(clock() - startTime) / CLOCKS_PER_SEC << "초 경과" << endl;
        }
    }

    finishTime = clock();
    duration = (double)(finishTime - startTime);

    cout << digit << "자리 소요시간 : " << duration / CLOCKS_PER_SEC << "초, 평균 소요시간 : "
    << duration / NUMBER_OF_PASSWORD / CLOCKS_PER_SEC << "초" << endl;

    return;
}