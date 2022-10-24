//Eduardo José Borges - ADS/2

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ENQUEUE_OP 1
#define DEQUEUE_OP 2

//Define queue type.
typedef struct{
    int front, rear;
    void (*enqueue)(int);
    void (*dequeue)();
    int* elements;
}Queue;

Queue Data;

void NewQueue();
void enqueue(int Value);
void dequeue();

void NewQueue(){
    //Set front and rear;
    Data.front = -1;
    Data.rear = -1;
    //Link functions;
    Data.enqueue = &enqueue;
    Data.dequeue = &dequeue;
    //Set elements array initial length with dynamic memory allocation.
    Data.elements = calloc(1, sizeof(int));
}

void enqueue(int Value){
    Data.rear++;
    if(Data.front == -1){
        Data.front = 0;
    }
    if(Data.rear == 0){
        Data.elements[Data.rear] = Value;
    }
    else{
        Data.elements = realloc(Data.elements, (Data.rear + 1) * sizeof(int));
        Data.elements[Data.rear] = Value;
    }
}

void dequeue(){
    if(Data.front >= Data.rear + 1 || Data.front == -1){
        MessageBox(NULL, "Underflow!", "Underflow error", MB_ICONERROR);
        Data.front--;
    }
    Data.front++;
}

//Window Procedure Events.
LRESULT CALLBACK WindowProc(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND QueueElements, InputArea, QueueSize;

//Create Window Label ELements.
void CreateQueueInfos(HWND window);
//Create Window Buttons and Input areas.
void CreateWindowButtons(HWND window);

//Data display
char* ElementsString();
char* SizeOfQueue();

//App Utils.
typedef struct{
    int x;
    int y;
}Vector2;

Vector2 AlignCenter(Vector2 ObjectSize);
Vector2 Margin(int MarginX, int MarginY);
Vector2 ComponentSize(double R_X, double R_Y);

//Main Function of a WIN32 app.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow){
    NewQueue();

    const wchar_t ClassName[] = L"Queue App";
    
    //Create Window Class.
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = (LPCSTR)ClassName;
    wc.hIcon = (HICON)LoadImage(hInstance, "./QueueIcon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_SHARED);
    RegisterClass(&wc);

    //Create main window.
    int ScreenX = GetSystemMetrics(SM_CXSCREEN);
    int ScreenY = GetSystemMetrics(SM_CYSCREEN);
    HWND window = CreateWindowEx(0, (LPCSTR)ClassName, "Queue App", WS_OVERLAPPEDWINDOW, 0, 0, ScreenX, ScreenY, NULL, NULL, hInstance, NULL);
    //Error Handling
    if(window == NULL){
        exit(-1);
    }

    ShowWindow(window, SW_SHOWMAXIMIZED);
    
    //Run Message Loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch(uMsg){
        case WM_CREATE:
            CreateQueueInfos(window);
            CreateWindowButtons(window);
            break;
        //Handle the events what occurs inside window components.
        case WM_COMMAND:
            switch (wParam)
            {
            case ENQUEUE_OP:
                char Value[30];
                GetWindowText(InputArea, Value, 30);
                if(!(atoi(Value)) && strcmp(Value, "0")){
                    MessageBox(NULL, "Only integer numbers are accepted!", "Input Error!", MB_ICONERROR);
                }
                else{
                    Data.enqueue(atoi(Value));
                    SetWindowText(QueueSize, TEXT(SizeOfQueue()));
                    SetWindowText(QueueElements, TEXT(ElementsString()));
                }
                break;
            case DEQUEUE_OP:
                Data.dequeue();
                if(Data.front != -1){
                    SetWindowText(QueueSize, TEXT(SizeOfQueue()));
                    SetWindowText(QueueElements, TEXT(ElementsString()));
                }
                break;
            
            default:
                break;
            }
        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(window, &ps);
            EndPaint(window, &ps);
            return 0;
        case WM_CLOSE:
            if(MessageBox(window, (LPCSTR)"Você realmente quer deixar a aplicação?", (LPCSTR)"Close", MB_OKCANCEL) == IDOK){
                DestroyWindow(window);
                free(Data.elements);
            }
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(window, uMsg, wParam, lParam);
}

void CreateQueueInfos(HWND window){
    //Info Title view.
    Vector2 InfoTitleSize = ComponentSize(3.5, 35);
    Vector2 InfoTitlePosition = AlignCenter((Vector2){400, 20});
    Vector2 InfoTitleMargin = Margin(0, 3);
    InfoTitlePosition.y -= InfoTitleMargin.y;
    HWND InfoTitle = CreateWindowW(L"STATIC", L"Queue:", WS_CHILD | WS_VISIBLE, InfoTitlePosition.x, InfoTitlePosition.y, InfoTitleSize.x, InfoTitleSize.y, window, NULL, NULL, NULL);
    //Stack elements view.
    Vector2 QueueElementsSize = ComponentSize(3.5, 8);
    Vector2 QueueElementsPosition = AlignCenter((Vector2){400, 100});
    Vector2 QueueElementsMargin = Margin(0, 4);
    QueueElementsPosition.y -= QueueElementsMargin.y;
    QueueElements = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE, QueueElementsPosition.x, QueueElementsPosition.y, QueueElementsSize.x, QueueElementsSize.y, window, NULL, NULL, NULL);
    //Size Title.
    Vector2 SizeTitleSize = ComponentSize(3.5, 35);
    Vector2 SizeTitlePosition = AlignCenter((Vector2){400, 200});
    Vector2 SizeTitleMargin = Margin(0, 20);
    SizeTitlePosition.y -= SizeTitleMargin.y;
    HWND SizeTitle = CreateWindowW(L"STATIC", L"Queue size is:", WS_CHILD | WS_VISIBLE, SizeTitlePosition.x, SizeTitlePosition.y, SizeTitleSize.x, SizeTitleSize.y, window, NULL, NULL, NULL);
    //Size.
    Vector2 QueueSizeViewSize = ComponentSize(3.5, 35);
    Vector2 QueueSizePosition = AlignCenter((Vector2){400, 200});
    Vector2 QueueSizeMargin = Margin(0, 50);
    QueueSizePosition.y -= QueueSizeMargin.y;
    QueueSize = CreateWindowW(L"STATIC", L"0", WS_CHILD | WS_VISIBLE, QueueSizePosition.x, QueueSizePosition.y, QueueSizeViewSize.x, QueueSizeViewSize.y, window, NULL, NULL, NULL);
}

void CreateWindowButtons(HWND window){
    //Input Area format.
    Vector2 InputSize = ComponentSize(3.5, 20);
    Vector2 InputPosition = AlignCenter((Vector2){400, 40});
    Vector2 InputMargin = Margin(0, 11);
    InputPosition.y -= InputMargin.y;
    InputArea = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, InputPosition.x, InputPosition.y, InputSize.x, InputSize.y, window, NULL, NULL, NULL);
    //Push Button format.
    Vector2 EnqueueButtonSize = ComponentSize(3.5, 20);
    Vector2 EnqueueButtonPosition = AlignCenter((Vector2){400, 40});
    Vector2 EnqueueButtonMargin = Margin(0, 40);
    EnqueueButtonPosition.y -= EnqueueButtonMargin.y;
    HWND EnqueueButton = CreateWindowW(L"BUTTON", L"Enqueue", WS_VISIBLE | WS_CHILD, EnqueueButtonPosition.x, EnqueueButtonPosition.y, EnqueueButtonSize.x, EnqueueButtonSize.y, window, (HMENU)ENQUEUE_OP, NULL, NULL);
    //Pop Button format.
    Vector2 DequeueButtonSize = ComponentSize(3.5, 20);
    Vector2 DequeueButtonPosition = AlignCenter((Vector2){400, 40});
    Vector2 DequeueButtonMargin = Margin(0, 25);
    DequeueButtonPosition.y += DequeueButtonMargin.y;
    HWND DequeueButton = CreateWindowW(L"BUTTON", L"Dequeue", WS_VISIBLE | WS_CHILD, DequeueButtonPosition.x, DequeueButtonPosition.y, DequeueButtonSize.x, DequeueButtonSize.y, window, (HMENU)DEQUEUE_OP, NULL, NULL);
}

char* ElementsString(){
    static char QueueView[512];
    strcpy(QueueView, "\0");
    char* QueueElementString = calloc(30, sizeof(char));
    for(int i = Data.front; i <= Data.rear; i++){
        if((i == Data.front)){
            strcat(QueueView, "[ ");
            sprintf(QueueElementString, "%d", Data.elements[i]);
            strcat(QueueView, QueueElementString);
            strcat(QueueView, " ]");
        }
        else if(i <= Data.rear){
            QueueView[strlen(QueueView)] = '\0';
            QueueView[strlen(QueueView) - 1] = '\0';
            strcat(QueueView, ", ");
            sprintf(QueueElementString, "%d", Data.elements[i]);
            strcat(QueueView, QueueElementString);
            strcat(QueueView, " ]");
        }
    }

    return QueueView;
}

char* SizeOfQueue(){
    static char QueueSizePush[20];
    strcpy(QueueSizePush, "\0");
    if(Data.front == -1){
        sprintf(QueueSizePush, "%d", 0);
    }
    else{
        sprintf(QueueSizePush, "%d", ((Data.rear + 1) - (Data.front + 1)) + 1);
    }
    return QueueSizePush;
}

Vector2 AlignCenter(Vector2 ObjectSize){
    Vector2 tmp;
    tmp.x = floor(GetSystemMetrics(SM_CXSCREEN)/2) - floor(ObjectSize.x/2);
    tmp.y = floor(GetSystemMetrics(SM_CYSCREEN)/2) - floor(ObjectSize.y/2);
    return tmp;
}

Vector2 Margin(int MarginX, int MarginY){
    Vector2 tmp;
    if(MarginX) tmp.x = floor(GetSystemMetrics(SM_CXSCREEN)/MarginX);
    else tmp.x = 0;
    if(MarginY) tmp.y = floor(GetSystemMetrics(SM_CYSCREEN)/MarginY);
    else tmp.y = 0;
    return tmp;
}

Vector2 ComponentSize(double R_X, double R_Y){
    Vector2 tmp;
    tmp.x = floor(GetSystemMetrics(SM_CXSCREEN)/R_X);
    tmp.y = floor(GetSystemMetrics(SM_CYSCREEN)/R_Y);
    return tmp;
}