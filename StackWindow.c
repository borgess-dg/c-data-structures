#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PUSH_OP 1
#define POP_OP 2
#define STACKSIZE 5

//Window Procedure Events.
LRESULT CALLBACK WindowProc(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND StackElements, InputArea, StackSize;

//Create Window Label ELements.
void CreateStackInfos(HWND window);
//Create Window Buttons and Input areas.
void CreateWindowButtons(HWND window);

//Data display
char* ElementsString();
char* SizeOfStack();

//App Utils.
typedef struct{
    int x;
    int y;
}Vector2;

Vector2 AlignCenter(Vector2 ObjectSize);
Vector2 Margin(int MarginX, int MarginY);

typedef struct{
    int top;
    void (*push)();
    void (*pop)();
    int elements[STACKSIZE];
}Stack;

//Prototype of Stack functions.
void newStack(int StackSize);
void push(Stack* Data, int Value);
void pop(Stack* Data);

void NewStack(Stack* tmp, int StackSize){
    //Link function to struct objects.
    tmp->top = -1;
    tmp->push = &push;
    tmp->pop = &pop;
    //Set stack elements quantity.
    //tmp->elements = calloc(StackSize, sizeof(int));
    //if(tmp->elements = NULL){
        //MessageBox(NULL, "Error!", "We have a memory allocation error!", MB_OK | MB_ICONERROR);
    //}
}

Stack GlobalStackData;

void push(Stack* Data, int Value){
    Data->top++;
    Data->elements[Data->top] = Value;
    if(Data->top > STACKSIZE - 1){
        MessageBox(NULL, "Há ocorrência de overflow nessa pilha!", "Overflow Error", MB_ICONERROR);
        Data->top--;
    }
}

void pop(Stack* Data){
    Data->top--;
}

//Main Function of a WIN32 app.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow){

    NewStack(&GlobalStackData, STACKSIZE);

    const wchar_t ClassName[] = L"Stack Window Class";
    
    //Create Window Class.
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = (LPCSTR)ClassName;
    wc.hIcon = (HICON)LoadImage(hInstance, "./StackIcon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_SHARED);
    RegisterClass(&wc);

    //Create main window.
    int ScreenX = GetSystemMetrics(SM_CXSCREEN);
    int ScreenY = GetSystemMetrics(SM_CYSCREEN);
    HWND window = CreateWindowEx(0, (LPCSTR)ClassName, "Stack Window", WS_OVERLAPPEDWINDOW, 0, 0, ScreenX, ScreenY, NULL, NULL, hInstance, NULL);
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
            CreateStackInfos(window);
            CreateWindowButtons(window);
            break;
        //Handle the events what occurs inside window components.
        case WM_COMMAND:
            switch (wParam)
            {
            case PUSH_OP:
                char Value[30];
                GetWindowText(InputArea, Value, 30);
                GlobalStackData.push(&GlobalStackData, atoi(Value));
                SetWindowText(StackSize, TEXT(SizeOfStack()));
                SetWindowText(StackElements, TEXT(ElementsString()));
                break;
            case POP_OP:
                char StackSizePop[20];
                GlobalStackData.pop(&GlobalStackData);
                sprintf(StackSizePop, "%d", GlobalStackData.top);
                SetWindowText(StackSize, TEXT(StackSizePop));
                SetWindowText(StackElements, TEXT(ElementsString()));
                break;
            
            default:
                break;
            }
        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(window, &ps);
            //SetBkMode(hdc, TRANSPARENT);
            EndPaint(window, &ps);
            return 0;
        case WM_CLOSE:
            if(MessageBox(window, (LPCSTR)"Você realmente quer deixar a aplicação?", (LPCSTR)"Close", MB_OKCANCEL) == IDOK){
                DestroyWindow(window);
            }
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(window, uMsg, wParam, lParam);
}

void CreateStackInfos(HWND window){
    //Info Title view.
    Vector2 InfoTitlePosition = AlignCenter((Vector2){400, 20});
    Vector2 InfoTitleMargin = Margin(0, 5);
    InfoTitlePosition.y -= InfoTitleMargin.y;
    HWND InfoTitle = CreateWindowW(L"STATIC", L"Stack:", WS_CHILD | WS_VISIBLE, InfoTitlePosition.x, InfoTitlePosition.y, 400, 20, window, NULL, NULL, NULL);
    //Stack elements view.
    Vector2 StackElementsPosition = AlignCenter((Vector2){400, 100});
    Vector2 StackElementsMargin = Margin(0, 10);
    StackElementsPosition.y -= StackElementsMargin.y;
    StackElements = CreateWindowW(L"STATIC", L"[ ]", WS_CHILD | WS_VISIBLE, StackElementsPosition.x, StackElementsPosition.y, 400, 100, window, NULL, NULL, NULL);
    //Size Title.
    Vector2 SizeTitlePosition = AlignCenter((Vector2){400, 200});
    Vector2 SizeTitleMargin = Margin(0, 10);
    SizeTitlePosition.y += SizeTitleMargin.y;
    HWND SizeTitle = CreateWindowW(L"STATIC", L"Stack size is:", WS_CHILD | WS_VISIBLE, SizeTitlePosition.x, SizeTitlePosition.y, 400, 20, window, NULL, NULL, NULL);
    //Size.
    Vector2 StackSizePosition = AlignCenter((Vector2){400, 200});
    Vector2 StackSizeMargin = Margin(0, 8);
    StackSizePosition.y += StackSizeMargin.y;
    StackSize = CreateWindowW(L"STATIC", L"0", WS_CHILD | WS_VISIBLE, StackSizePosition.x, StackSizePosition.y, 400, 20, window, NULL, NULL, NULL);
}

void CreateWindowButtons(HWND window){
    InputArea = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 0, 100, 100, 20, window, NULL, NULL, NULL);
    HWND PushButton = CreateWindowW(L"BUTTON", L"Push", WS_VISIBLE | WS_CHILD, 0, 150, 100, 40, window, (HMENU)PUSH_OP, NULL, NULL);
    HWND PopButton = CreateWindowW(L"BUTTON", L"Pop", WS_VISIBLE | WS_CHILD, 0, 200, 100, 40, window, (HMENU)POP_OP, NULL, NULL);
}

char* ElementsString(){
    static char StackView[512];
    strcpy(StackView, "\0");
    char* StackElementString = calloc(30, sizeof(char));
    for(int i = 0; i <= GlobalStackData.top; i++){
        if((i == 0)){
            strcat(StackView, "[ ");
            sprintf(StackElementString, "%d", GlobalStackData.elements[i]);
            strcat(StackView, StackElementString);
            strcat(StackView, " ]");
        }
        else if(i <= GlobalStackData.top - 1){
            StackView[strlen(StackView)] = '\0';
            StackView[strlen(StackView) - 1] = '\0';
            strcat(StackView, ", ");
            sprintf(StackElementString, "%d", GlobalStackData.elements[i]);
            strcat(StackView, StackElementString);
            strcat(StackView, " ]");
        }
    }

    return StackView;
}

char* SizeOfStack(){
    static char StackSizePush[20];
    strcpy(StackSizePush, "\0"); 
    sprintf(StackSizePush, "%d", GlobalStackData.top);
    return StackSizePush;
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