//
//  main.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"

#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"

#define MAX_NODE        100


//board, food card, festival card configuration parameters
static int board_nr;
static int food_nr;
static int festival_nr;

//player constituent
static int player_nr;


typedef struct player {
	int energy;
	int position;
	char name[MAX_CHARNAME];
	int accumCredit;
	int experiment;
	int flag_graduate;
} player_t;


static player_t *cur_player;



//function prototypes
#if 0
int isGraduated(void); //check if any player is graduated
void generatePlayers(int n, int initEnergy); //generate a new player
void goForward(int player, int step); //make player go "step" steps on the board (check if player is graduated)
void printPlayerStatus(void); //print all player status at the beginning of each turn
float calcAverageGrade(int player); //calculate average grade of the player
smmGrade_e takeLecture(int player, char *lectureName, int credit); //take the lecture (insert a grade of the player)
void* findGrade(int player, char *lectureName); //find the grade from the player's grade history
void printGrades(int player); //print all the grade history of the player
#endif


int isGraduated(void)
{
    //플레이어 중 한 명이라도 졸업하면 게임 종료 깃발을 든다. 
	int i;
    int flag_end = 0;
    
	for(i=0; i<player_nr; i++)
	{
		if (cur_player[i].accumCredit >= GRADUATE_CREDIT)
		    cur_player[i].flag_graduate = 1;
		    flag_end = 1;
	}
	     
	return flag_end;
}


void generatePlayers(int n, int initEnergy) //generate a new player
{
	int i;
	
	for(i=0; i<n; i++)
	{
		//input name
		printf("Input player %i's name: ", i);
		scanf("%s", cur_player[i].name);
		fflush(stdin);
		
		
		//set position
		cur_player[i].position = 0;
		
		//set energy, accum credit, 졸업 상태, 실험 상태  
		cur_player[i].energy = initEnergy;
		cur_player[i].accumCredit = 0;
		cur_player[i].flag_graduate = 0;
		cur_player[i].experiment = 0;
	}
}

void goForward(int player, int step)
{
	//주사위의 결과 만큼 플레이어의 위치를 옮긴다 
	void *boardPtr;
	int i;
	
	for(i=0; i<step; i++)
	{
		cur_player[player].position ++;
	    cur_player[player].position = cur_player[player].position%board_nr;	            
	    boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position );
	    printf("  ->Jump to %s\n", smmObj_getNodeName(boardPtr));
	    
		if(cur_player[player].position = 0) //집을 거치면 에너지 충전 
	    {
	    	cur_player[player].energy += smmObj_getNodeEnergy(boardPtr);
		    printf("->returned to HOME! energy charged by %i (total : %i)\n", smmObj_getNodeEnergy(boardPtr), cur_player[player].energy);
		}
	}
		
}

void printPlayerStatus(void)
{
	//플레이어의 상태를 출력한다 
	int i;
	printf("______________________________PLAYER STATUS_______________________________\n");
	for (i=0; i<player_nr; i++)
	{
		printf("%s : credit %i, energy %i position %i\n", 
		             cur_player[i].name,
					 cur_player[i].accumCredit,
					 cur_player[i].energy,
					 cur_player[i].position);					 
	}
	printf("__________________________________________________________________________\n");
}

float calcAverageGrade(int player)
{
	//플레이어의 평균 성적을 계산한다 
	int i;
	int AllGrades;
	float averageGrade;
	void *gradePtr;
	float RealGrade;
	int GradeNumber = 0;
	
	for (i=0; i<smmdb_len(LISTNO_OFFSET_GRADE + player); i++)
	{
		gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
		RealGrade = 4.3-(0.3*smmObj_getNodeGrade(gradePtr));
		AllGrades = AllGrades + RealGrade; //모든 성적을 더한다 
		if(smmObj_getNodeGrade(gradePtr) != NULL)
		    GradeNumber++;
	}
	averageGrade =  AllGrades / GradeNumber; //모든 성적을 더한 값을 성적의 개수로 나눠 평균 성적을 구한다 
	
	return averageGrade;
}

smmObjGrade_e takeLecture(int player, char *lectureName, int credit)
{
    void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
       
    cur_player[player].accumCredit += smmObj_getNodeCredit(boardPtr);
    cur_player[player].energy -= smmObj_getNodeEnergy(boardPtr);
    
    int randGrade = (rand()%MAX_GRADE);
    smmObjGrade_e GradeResult = (smmObjGrade_e)randGrade;
                      
    //grade generation
    void *gradePtr = smmObj_genObject(lectureName, smmObjType_grade , 0, credit, 0, randGrade);
	smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
	
	printf("a successfully takes the lecture %s with grade %smmObjGrade_e (average : %f), reminded energy : %i\n", smmObj_getNodeName( boardPtr ),randGrade, calcAverageGrade(player), cur_player[player].energy);
}

void* findGrade(int player, char *lectureName)
{
	int i;
	void *gradePtr;
	int flag = 0;
	
	for (i=0; i<smmdb_len(LISTNO_OFFSET_GRADE + player); i++)
	{
		gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
	    if(smmObj_getNodeName(gradePtr) == lectureName)
	        flag = 1;
	    
	}	
	return flag;   	
}

void printGrades(int player)
{
	//플레이어의 성적을 출력한다 
	int i;
	void *gradePtr;
	
	printf("______________________________PLAYER GRADES_______________________________\n");
	for (i=0; i<smmdb_len(LISTNO_OFFSET_GRADE + player); i++)
	{
		gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
		printf("%s : credit %i, grade: %i \n", smmObj_getNodeName(gradePtr), smmObj_getNodeCredit(gradePtr), smmObj_getNodeGrade(gradePtr));
	}
	printf("__________________________________________________________________________\n");
}



int rolldie(int player)
{
    char c;
    printf(" Press any key to roll a die (press g to see grade): ");  
    c = getchar();
    fflush(stdin);
    
#if 1
    if (c == 'g') //g면 플레이어의 성적 출력 
        printGrades(player);
#endif
    
    return (rand()%MAX_DIE + 1); //g가 아니면 주사위의 결과 출력 
}


//action code when a player stays at a node
void actionNode(int player)
{
    void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
    int type = smmObj_getNodeType( boardPtr );
    char *name = smmObj_getNodeName( boardPtr );
    void *gradePtr;
    void *festcard;   
    void *foodcard;
    char c;
    int threshold;
    int experiment_result;
    char reply[SMM_LECTURE_REPLY];
    
	switch(type)
    {
        //case lecture:
        case SMMNODE_TYPE_LECTURE:       	  
			    
	        while (1)
		    {
			    printf("Lecture %s (credit %i, energy %i) starts! are you going to join? or drop? :", smmObj_getNodeName( boardPtr ), smmObj_getNodeCredit(boardPtr), smmObj_getNodeEnergy(boardPtr));
			    scanf("%s", reply);
		   
			    if(reply == "join")
		        {
		            if(findGrade(player, smmObj_getNodeName( boardPtr )) == 0)
		            {
		            	if(cur_player[player].energy >= smmObj_getNodeEnergy(boardPtr))
		        	    {
		        		    
			                takeLecture(player, name, smmObj_getNodeCredit(boardPtr));
                            break;
					    }
					
	    				else
		    			    printf("%s is too hungry to take the lecture %s (remained : %i, required : %i)\n", cur_player[player].name, smmObj_getNodeName( boardPtr ), cur_player[player].energy, smmObj_getNodeEnergy(boardPtr));
					   
			    		    break;
				    }
				
	    			else
		    		    printf("%s has already taken the lecture %s!!\n", cur_player[player].name, smmObj_getNodeName( boardPtr ));                   
        	     				
	            }
				
		        else if(reply == "drop")
		        {
		            printf("Player %s drops the lecture %s!\n", cur_player[player].name, smmObj_getNodeName( boardPtr ));
			        break;	
		    	}
			    								
		    	else
		            printf("invalid input! input 'drop' or 'join'!\n");
	    	} 	       	
			
			break;
			
			
		//case Restaurant:
		case SMMNODE_TYPE_RESTAURANT:
		    
			cur_player[player].energy += smmObj_getNodeEnergy(boardPtr); //플레이어의 에너지가 노드의 에너지만큼 충전 
		    printf("->Let's eat in %s and charge %i energies (remained energy : %i)\n", name, smmObj_getNodeEnergy(boardPtr), cur_player[player].energy);
		   		   
		    break;
		   
		case SMMNODE_TYPE_LABORATORY:
		    
		    
			if (cur_player[player].experiment == 0) //실험 중이 아니므로 지나감 
			   printf("This is not experiment time. You can go through this lab.\n");
			   
			else //실험 중 
			{
				threshold = (rand()%MAX_DIE + 1);
			    printf("->Experimant time! Let's see if you can satisfy professer (threshold : %i)\n", threshold);
			
			    cur_player[player].energy -= smmObj_getNodeEnergy(boardPtr);
			    experiment_result = rolldie(player);
			
			    if(experiment_result >= threshold) //실험 성공 
			    {
				    cur_player[player].experiment = 0; //실험 중 상태에서 일반 상태로 전환 
			        printf("->Experimant result : %i, success! %s can exit this lab!\n", experiment_result, cur_player[player].name);
			    }
			
			    else //실험 실패 
			        printf("->Experimant result : %i, fail ToT. %s need more experiment.....\n", experiment_result, cur_player[player].name);
			} 
						    
			break;   	
			
		case SMMNODE_TYPE_HOME:	
			
			//goForward 함수에 에너지 충전이 포함되어있으므로 공란  
			
		    break;
		    
		case SMMNODE_TYPE_GOTOLAB:
			
			cur_player[player].experiment = 1; //실험 중 상태로 전환 
			printf("OMG! This is experiment time! Player %s goes to the lab.\n", cur_player[player].name);
			
			while(type != SMMNODE_TYPE_LABORATORY) //실험실로 이동
			{
				cur_player[player].position++;
				cur_player[player].position = cur_player[player].position%board_nr; 
			}
			 
									
			break;
			
		case SMMNODE_TYPE_FOODCHANCE:
		    	    		    
			printf("->%s gets a food chance! press any key to pick a food card: ", cur_player[player].name);  
		    c = getchar();
		    fflush(stdin);
		    
			foodcard = smmdb_getData(LISTNO_FOODCARD, (rand()%food_nr + 1)); //음식 카드를 랜덤으로 가져옴 
			cur_player[player].energy += smmObj_getFoodEnergy(foodcard); //음식 카드의 에너지만큼 충전 
		    printf("->%s gets %s and charges %i (remained energy : %i)\n", cur_player[player].name, smmObj_getFoodName(foodcard), smmObj_getFoodEnergy(foodcard), cur_player[player].energy);
			break;
		    
		case SMMNODE_TYPE_FESTIVAL:	
			
			printf("->%s participates to Snow Festival! press any key to pick a festival card: ", cur_player[player].name);	
			c = getchar();
		    fflush(stdin);
				
			festcard = smmdb_getData(LISTNO_FESTCARD, (rand()%festival_nr + 1)); //축제 카드를 랜덤으로 가져옴 
			printf("MISSION : %s", smmObj_getFestCard(festcard)); //미션 출력 
			printf("Press any key when mission is ended.\n");
			c = getchar();
		    fflush(stdin);
						
			break;
				
		default:
            break;
    }
}


int main(int argc, const char * argv[]) {
    
    FILE* fp;
    char name[MAX_CHARNAME];
	int type;
	int credit;
	int energy;
	smmObjType_e Objtype;
	smmObjGrade_e grade;
	int initEnergy;
	int i;
	
	char food_name[MAX_CHARNAME];
	int food_energy;
	char Fest_card[MAX_CHARNAME];
    
    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;
    
	int turn = 0;
    int die_result;
	     
    srand(time(NULL));
    
    printf("-------------------------SM marble game starts!-------------------------");
    
    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig 
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }
    
    printf("Reading board component......\n");
    while (fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4) //read a node parameter set
    {
        //store the parameter set
        void *boardObj = smmObj_genObject(name, Objtype, type, credit, energy, grade);
        smmdb_addTail(LISTNO_NODE, boardObj);

        if (type == SMMNODE_TYPE_HOME)
           initEnergy = energy;
        board_nr++;
    }
    fclose(fp);
    printf("Total number of board nodes : %i\n", board_nr);
    //노드 정보 출력 
     for (i = 0;i<board_nr;i++)
    {
        void *boardObj = smmdb_getData(LISTNO_NODE, i);
        printf("node %i : %s, %i(%s), credit %i, energy %i\n", 
                i, smmObj_getNodeName(boardObj), 
                smmObj_getNodeType(boardObj), smmObj_getTypeName(smmObj_getNodeType(boardObj)),
                smmObj_getNodeCredit(boardObj), smmObj_getNodeEnergy(boardObj));
    }
  
    
    //2. food card config 
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }
    
    printf("\n\nReading food card component......\n");
    while (fscanf(fp, "%s %i", food_name, &food_energy) == 2) //read a food parameter set
    {
        //store the parameter set
        void *foodObj = smmObj_foodCard(food_name, food_energy);
        smmdb_addTail(LISTNO_FOODCARD, foodObj);
        food_nr++;
    }
    fclose(fp);
    printf("Total number of food cards : %i\n", food_nr);
    //음식 카드 정보 출력 
     for (i = 0;i<food_nr;i++)
    {
        void *foodObj = smmdb_getData(LISTNO_FOODCARD, i);
        printf(" Card %i : %s, energy %i \n", i, smmObj_getFoodName(foodObj), smmObj_getFoodEnergy(foodObj));
    }
    
    //3. festival card config 
    if ((fp = fopen(FESTFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }
    
    printf("\n\nReading festival card component......\n");
    while (fscanf(fp, "%s", Fest_card) == 1) //read a festival card string
    {
        //store the parameter set
        void *FestObj = smmObj_FestCard(Fest_card);
        smmdb_addTail(LISTNO_FESTCARD, FestObj);
        festival_nr++;
    }
    fclose(fp);
    printf("Total number of festival cards : %i\n", festival_nr);
    //축제 카드 정보 출력 
     for (i = 0;i<festival_nr;i++)
    {
        void *FestObj = smmdb_getData(LISTNO_FESTCARD, i);
        printf(" Card %i : %s \n", i, smmObj_getFestCard(FestObj));
    }
    
    
    //2. Player configuration ---------------------------------------------------------------------------------
    
    do
    {
        //input player number to player_nr
        printf("input player no. : \n");
        scanf("%d", &player_nr);
        fflush(stdin);
    }
    while (player_nr < 0 || player_nr >  MAX_PLAYER);
    
    cur_player = (player_t*)malloc(player_nr*sizeof(player_t));
    
    generatePlayers(player_nr, initEnergy); //플레이어 생성 
     
    
    
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (isGraduated != 1) //is anybody graduated?
    {
               
        //4-1. initial printing           
		printPlayerStatus();
        
        printf("This is %s's turn ::: \n", cur_player[turn].name);
        
       
        if(cur_player[turn].experiment == 0) //(if not in experiment)
		{
			//4-2. die rolling 
			die_result = rolldie(turn);
			printf("-->result : %i \n", die_result);
			 
			//4-3. go forward
            goForward(turn, die_result);
        }   		     
        
		//4-4. take action at the destination node of the board
        actionNode(turn);
        
        //4-5. next turn
        turn = (turn +1)%player_nr;
    }
    
    //4. SM Marble game ends -----------------------------------------------------------------------------------
    
    //졸업한 플레이어를 집으로 보내고 정보 출력 
    for(i=0; i<player_nr; i++)
    {
    	if (cur_player[i].flag_graduate == 1)
    	{
    		printf("Congratulation! Player %s graduated! \n", cur_player[i].name);
			cur_player[i].position = 0;
    	    printf("Player %s returns to HOME! \n", cur_player[i].name);
    	    printGrades(i);  	    
    	}   
     
	}
	
    printf("--------------------------SM marble game ends!--------------------------\n");
    
    free(cur_player);
    system("PAUSE");
    return 0;
}
