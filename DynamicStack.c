//Eduardo José Borges - ADS/2

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define PUSH_OP 1
#define POP_OP 2

typedef struct{
    int top;
    void (*push)(int);
    int (*pop)();
    int* elements;
}Stack;

Stack Data;

void NewStack();
void push(int Value);
int pop();

void NewStack(){
    //Set top;
    Data.top = -1;
    //Link functions
    Data.push = &push;
    Data.pop = &pop;
    //Set elements array initial length with dynamic memory allocation.
    Data.elements = calloc(1, sizeof(int));
}

void push(int Value){
    Data.top++;
    if(Data.top == 0){
        Data.elements[Data.top] = Value;
    }
    else{
        Data.elements = realloc(Data.elements, (Data.top + 1) * sizeof(int));
        Data.elements[Data.top] = Value;
    }
    printf("\n%d", Data.elements[Data.top]);
}

int pop(){
    int tmp = Data.elements[Data.top];
    Data.top--;
    if(Data.top < -1){
        MessageBox(NULL, "Underflow!", "Underflow error", MB_ICONERROR);
        Data.top++;
    }
    return tmp;
}

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
Vector2 ComponentSize(double R_X, double R_Y);

//Main Function of a WIN32 app.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow){
    NewStack();

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
                if(!(atoi(Value)) && strcmp(Value, "0")){
                    MessageBox(NULL, "Only integer numbers are accepted!", "Input Error!", MB_ICONERROR);
                }
                else{
                    Data.push(atoi(Value));
                    SetWindowText(StackSize, TEXT(SizeOfStack()));
                    SetWindowText(StackElements, TEXT(ElementsString()));
                }
                break;
            case POP_OP:
                char StackSizePop[20];
                Data.pop();
                sprintf(StackSizePop, "%d", Data.top + 1);
                SetWindowText(StackSize, TEXT(SizeOfStack()));
                SetWindowText(StackElements, TEXT(ElementsString()));
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

void CreateStackInfos(HWND window){
    //Info Title view.
    Vector2 InfoTitleSize = ComponentSize(3.5, 35);
    Vector2 InfoTitlePosition = AlignCenter((Vector2){400, 20});
    Vector2 InfoTitleMargin = Margin(0, 3);
    InfoTitlePosition.y -= InfoTitleMargin.y;
    HWND InfoTitle = CreateWindowW(L"STATIC", L"Stack:", WS_CHILD | WS_VISIBLE, InfoTitlePosition.x, InfoTitlePosition.y, InfoTitleSize.x, InfoTitleSize.y, window, NULL, NULL, NULL);
    //Stack elements view.
    Vector2 StackElementsSize = ComponentSize(3.5, 8);
    Vector2 StackElementsPosition = AlignCenter((Vector2){400, 100});
    Vector2 StackElementsMargin = Margin(0, 4);
    StackElementsPosition.y -= StackElementsMargin.y;
    StackElements = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE, StackElementsPosition.x, StackElementsPosition.y, StackElementsSize.x, StackElementsSize.y, window, NULL, NULL, NULL);
    //Size Title.
    Vector2 SizeTitleSize = ComponentSize(3.5, 35);
    Vector2 SizeTitlePosition = AlignCenter((Vector2){400, 200});
    Vector2 SizeTitleMargin = Margin(0, 20);
    SizeTitlePosition.y -= SizeTitleMargin.y;
    HWND SizeTitle = CreateWindowW(L"STATIC", L"Stack size is:", WS_CHILD | WS_VISIBLE, SizeTitlePosition.x, SizeTitlePosition.y, SizeTitleSize.x, SizeTitleSize.y, window, NULL, NULL, NULL);
    //Size.
    Vector2 StackSizeViewSize = ComponentSize(3.5, 35);
    Vector2 StackSizePosition = AlignCenter((Vector2){400, 200});
    Vector2 StackSizeMargin = Margin(0, 50);
    StackSizePosition.y -= StackSizeMargin.y;
    StackSize = CreateWindowW(L"STATIC", L"0", WS_CHILD | WS_VISIBLE, StackSizePosition.x, StackSizePosition.y, StackSizeViewSize.x, StackSizeViewSize.y, window, NULL, NULL, NULL);
}

void CreateWindowButtons(HWND window){
    //Input Area format.
    Vector2 InputSize = ComponentSize(3.5, 20);
    Vector2 InputPosition = AlignCenter((Vector2){400, 40});
    Vector2 InputMargin = Margin(0, 11);
    InputPosition.y -= InputMargin.y;
    InputArea = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, InputPosition.x, InputPosition.y, InputSize.x, InputSize.y, window, NULL, NULL, NULL);
    //Push Button format.
    Vector2 PushButtonSize = ComponentSize(3.5, 20);
    Vector2 PushButtonPosition = AlignCenter((Vector2){400, 40});
    Vector2 PushButtonMargin = Margin(0, 40);
    PushButtonPosition.y -= PushButtonMargin.y;
    HWND PushButton = CreateWindowW(L"BUTTON", L"Push", WS_VISIBLE | WS_CHILD, PushButtonPosition.x, PushButtonPosition.y, PushButtonSize.x, PushButtonSize.y, window, (HMENU)PUSH_OP, NULL, NULL);
    //Pop Button format.
    Vector2 PopButtonSize = ComponentSize(3.5, 20);
    Vector2 PopButtonPosition = AlignCenter((Vector2){400, 40});
    Vector2 PopButtonMargin = Margin(0, 25);
    PopButtonPosition.y += PopButtonMargin.y;
    HWND PopButton = CreateWindowW(L"BUTTON", L"Pop", WS_VISIBLE | WS_CHILD, PopButtonPosition.x, PopButtonPosition.y, PopButtonSize.x, PopButtonSize.y, window, (HMENU)POP_OP, NULL, NULL);
}

char* ElementsString(){
    static char StackView[512];
    strcpy(StackView, "\0");
    char* StackElementString = calloc(30, sizeof(char));
    for(int i = 0; i <= Data.top; i++){
        if((i == 0)){
            strcat(StackView, "[ ");
            sprintf(StackElementString, "%d", Data.elements[i]);
            strcat(StackView, StackElementString);
            strcat(StackView, " ]");
        }
        else if(i <= Data.top){
            StackView[strlen(StackView)] = '\0';
            StackView[strlen(StackView) - 1] = '\0';
            strcat(StackView, ", ");
            sprintf(StackElementString, "%d", Data.elements[i]);
            strcat(StackView, StackElementString);
            strcat(StackView, " ]");
        }
    }

    return StackView;
}

char* SizeOfStack(){
    static char StackSizePush[20];
    strcpy(StackSizePush, "\0"); 
    sprintf(StackSizePush, "%d", Data.top + 1);
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

Vector2 ComponentSize(double R_X, double R_Y){
    Vector2 tmp;
    tmp.x = floor(GetSystemMetrics(SM_CXSCREEN)/R_X);
    tmp.y = floor(GetSystemMetrics(SM_CYSCREEN)/R_Y);
    return tmp;
}