#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_ACTIONS 1000
#define MAX_STACK 10

typedef enum{
  ACT_CLICK,
  ACT_RIGHT_CLICK,
  ACT_DOUBLE_CLICK,
  ACT_WAIT,
  ACT_LOOP_START,
  ACT_LOOP_END
} ActionType;

typedef struct{
  ActionType type;
  int x;
  int y;
  int time;
  int loop_count;
} Action;

typedef struct{
  int start_index;
  int remaining;
} Loop;

void click(int x, int y, DWORD down_flag, DWORD up_flag, int times){
  INPUT in = {0};
	
	SetCursorPos(x, y);
	Sleep(20);
	
	int i;
	for(i = 0; i < times; i++){
		in.type = INPUT_MOUSE;
		in.mi.dwFlags = down_flag;
		
		SendInput(1, &in, sizeof(INPUT));
		
		Sleep(20);
		
		in.type = INPUT_MOUSE;
		in.mi.dwFlags = up_flag;
		
		SendInput(1, &in, sizeof(INPUT));
	}
}

void fatal_error(){
	printf("Press Enter to exit...\n");
	getchar();
	getchar();
	exit(1);
}

void parse_line(char *line, Action actions[], int *counter, int max_x, int max_y, int number_line){
    
  //ignore comments
  if(line[0] == '#')
    return;
    
	if(*counter >= MAX_ACTIONS){
	    printf("Too many actions!\n");
	    fatal_error();
	}
	
	int x, y, t, l;

    //CLICK
    if(sscanf(line, "click=%d,%d", &x, &y) == 2){
    	
    	if(x < 0 || x >= max_x || y < 0 || y >= max_y){
    		printf("Error at line %d: coordinates out of screen bounds (%d,%d)\n", number_line, x, y);
    		fatal_error();
		}
    	
    	actions[*counter].type = ACT_CLICK;
    	actions[*counter].x = x;
    	actions[*counter].y = y;
	}
	
	//RIGHT CLICK
	else if(sscanf(line, "rclick=%d,%d", &x, &y) == 2){
		
		if(x < 0 || x >= max_x || y < 0 || y >= max_y){
    		printf("Error at line %d: coordinates out of screen bounds (%d,%d)\n", number_line, x, y);
    		fatal_error();
		}
		
		actions[*counter].type = ACT_RIGHT_CLICK;
    actions[*counter].x = x;
    actions[*counter].y = y;
	}
	
	else if(sscanf(line, "dbclick=%d,%d", &x, &y) == 2){
		
		if(x < 0 || x >= max_x || y < 0 || y >= max_y){
    		printf("Error at line %d: coordinates out of screen bounds (%d,%d)\n", number_line, x, y);
    		fatal_error();
		}
		
		actions[*counter].type = ACT_DOUBLE_CLICK;
    actions[*counter].x = x;
    actions[*counter].y = y;
	}
	
  //WAIT
  else if(sscanf(line, "wait=%d", &t) == 1){
    
    if(t < 0){
      printf("Error at line %d: wait must be >= 0, got %d\n", number_line, t);
      fatal_error();
  }
    
    actions[*counter].type = ACT_WAIT;
    actions[*counter].time = t;
	}
  //LOOP
  else if(sscanf(line, "loop=%d", &l) == 1){
  
  if(l <= 0){
        printf("Error at line %d: loop must be > 0, got %d\n", number_line, l);
        fatal_error();
    }
    
    actions[*counter].type = ACT_LOOP_START;
    actions[*counter].loop_count = l;
	}

	//END LOOP
    else if(strcmp(line, "endloop") == 0){
    	actions[*counter].type = ACT_LOOP_END;	
	}
    
    else{
		printf("Unknown command at line %d: %s\n", number_line, line);
		fatal_error();
	}
    
    (*counter)++;
}

void execute_actions(Action actions[], int actions_count){
	
	Loop stack[MAX_STACK];
	int loop_level = -1;
	
	int i = 0;
	
	while(i < actions_count){
		
		switch(actions[i].type){
			
			case ACT_CLICK:
				click(actions[i].x, actions[i].y, MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP, 1);
				i++;
				break;
			
			case ACT_RIGHT_CLICK:
				click(actions[i].x, actions[i].y, MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_RIGHTUP, 1);
				i++;
				break;
				
			case ACT_DOUBLE_CLICK:
				click(actions[i].x, actions[i].y, MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP, 2);
				i++;
				break;
			
			case ACT_WAIT:
				Sleep(actions[i].time);
				
				i++;
				break;
			
			case ACT_LOOP_START:
				
				if(loop_level >= MAX_STACK - 1){
                    printf("Too many nested loops!\n");
                    fatal_error();
                }
                
				if(actions[i].loop_count <= 0){
					i++;
					break;
				}
                
				loop_level++;
				stack[loop_level].start_index = i + 1;
				stack[loop_level].remaining = actions[i].loop_count;
				
				i++;
				break;
				
			case ACT_LOOP_END:
				
				if(loop_level < 0){
                    printf("Error: endloop without loop!\n", i);
                    fatal_error();
                }
                
				if(stack[loop_level].remaining > 1){
					stack[loop_level].remaining--;
					i = stack[loop_level].start_index;
				}
				else{
					loop_level--;
					i++;	
				}
				
				break;
		}
	}
}

int main(){
  BOOL (WINAPI *SetDPIAware)(void);
  
  SetDPIAware = (BOOL (WINAPI *)(void))
      GetProcAddress(GetModuleHandle("user32.dll"), "SetProcessDPIAware");
  
  if(SetDPIAware){
      SetDPIAware();
  }
  
  int max_x = GetSystemMetrics(SM_CXSCREEN);
  int max_y = GetSystemMetrics(SM_CYSCREEN);
  
  Action actions[MAX_ACTIONS];
  int action_counter = 0;

  FILE *file = fopen("macro.ini", "r");
  char line[256];

  if(file == NULL){
      printf("Error: cannot open 'macro.ini'. File not found or inaccessible.\n");
      fatal_error();
  }

  int number_line = 0;

  while(fgets(line, sizeof(line), file)){
  
  if(line[0] == ' ' || line[0] == '\t'){
      printf("Error: indentation not allowed: %s\n", line);
      fatal_error();
  }
  
  number_line++;
  
      //remove newline
      line[strcspn(line, "\n")] = 0;

      //skip empty lines
      if(strlen(line) == 0)
        continue;
  
      parse_line(line, actions, &action_counter, max_x, max_y, number_line);
  }

  fclose(file);
  execute_actions(actions, action_counter);
  
  printf("Execution completed successfully.\n");
	printf("Press Enter to exit...\n");
	getchar();
	getchar();
	
	return 0;
}
