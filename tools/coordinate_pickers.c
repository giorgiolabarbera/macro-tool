#include <stdio.h>
#include <windows.h>

int main(){
    POINT p;

    printf("[F8]  click\n");
	printf("[F9]  right click\n");
	printf("[F10] double click\n");
	printf("[ESC] exit\n\n");
    
	while(1){
    	
        //ESC -> exit
        if(GetAsyncKeyState(VK_ESCAPE) & 1){
        	printf("ESC pressed. Exiting...\n");
            break;
        }

        //F8 -> left click
        if(GetAsyncKeyState(VK_F8) & 1){
            GetCursorPos(&p);

            printf("click=%d,%d\n", p.x, p.y);

            Sleep(200);
        }
        
        //F9 -> right click
        if(GetAsyncKeyState(VK_F9) & 1){
            GetCursorPos(&p);

            printf("rclick=%d,%d\n", p.x, p.y);

            Sleep(200);
        }
        
        //F10 -> double click
        if(GetAsyncKeyState(VK_F10) & 1){
            GetCursorPos(&p);

            printf("dbclick=%d,%d\n", p.x, p.y);

            Sleep(200);
        }

        Sleep(10);
    }

    return 0;
}
