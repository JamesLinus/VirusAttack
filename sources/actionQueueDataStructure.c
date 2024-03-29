//
// sputFunctions.c
// Created by bclarke on 2014/09/11
// tags: stack, abstraction, struct pointers
//
/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*---------- Custom Headers	-----------*/

#include "../includes/sput.h"
#include "../includes/actionQueueDataStructure.h"

/*---------- Data Types -----------*/

struct queueNode {
	
	commandType command; //command for execution
	upgradeStat option; //stat to change
	int target;	//target of command
    struct queueNode *nextNode;

};

struct actionQueueStructure  {
	
    struct queueNode *start;
    struct queueNode *current;
    int nItems;

};


/*---------- Functions ----------*/

/*int main()	{
	//testingGameStructure();
	testingActionQueue();
}*/
void testingActionQueue()	{

	sput_start_testing();
	
	sput_set_output_stream(NULL);

	sput_enter_suite("testCheckGold(): gold check");
    sput_run_test(testcheckGold);
    sput_leave_suite();
	
	//sput_enter_suite("testPopFromQueue(): Popping from queue");
	//sput_run_test(testPopFromQueue);
	//sput_leave_suite();

	sput_enter_suite("testPushToQueue(): Pushing to queue");
	sput_run_test(testPushToQueue);
	sput_leave_suite();

	sput_finish_testing();
}

ActionQueueStructure createActionQueue()	{

	ActionQueueStructure newActionQueue = (ActionQueueStructure) malloc(sizeof(*newActionQueue));
	newActionQueue->nItems = 0;
	newActionQueue->start = newActionQueue->current = NULL;
	getQueue(newActionQueue);

	return newActionQueue;
}

/*
 *puts node to back of queue
 */
int createNode(ActionQueueStructure queue)	{
	
	if(queue->start == NULL )	{
		queue->start = queue->current = (QueueNode) malloc(sizeof(*(queue->start)));
		queue->start->nextNode = NULL;
	} else {
		queue->current->nextNode = (QueueNode) malloc(sizeof(*(queue->current->nextNode)));
		queue->current = queue->current->nextNode;
		queue->current->nextNode = NULL;
	}

	return 1;

}

ActionQueueStructure getQueue(ActionQueueStructure queue)	{

	static ActionQueueStructure stQueue;

	if(queue != NULL)	{
		stQueue = queue;	
	}
	
	return stQueue;

}
/*
 * Pushes values to newly created node at back of queue
 */
int pushToQueue(ActionQueueStructure queue, commandType command, upgradeStat option, int target)	{
		createNode(queue);
		queue->current->command = command;
		queue->current->option = option;
		queue->current->target = target;
		(queue->nItems)++;
		return (queue->nItems);
}

void testPushToQueue()	{
	
    //commandType nCommand;
    //commandType nStat;
    //int tar = 1;

    commandType nCommand_1=upgrade;
    upgradeStat nStat_1=power;
    int tar_1 = 1;

    commandType nCommand_2=execute;
    upgradeStat nStat_2=range;
    int tar_2 = 2;

    //commandType nCommand_3=set;
    //upgradeStat nStat_3=speed;
    //int tar_3 = 3;
	
	
    GameProperties newGame = createGame();
    ActionQueueStructure newQueue = createActionQueue();

	sput_fail_unless(pushToQueue(newQueue,nCommand_1,nStat_1,tar_1) == 1,"Valid: 1 Queue Item");
	sput_fail_unless(pushToQueue(newQueue,nCommand_2,nStat_2,tar_2) == 2,"Valid: 2 Queue Items");
	sput_fail_unless(getFirstCommand(newQueue) == upgrade,"Valid: Top of Queue Command");
	sput_fail_unless(getFirstOption(newQueue) == power,"Valid: Top of Queue Option");
	sput_fail_unless(getLastCommand(newQueue) == execute,"Valid: Last of Queue Command");
	sput_fail_unless(getLastOption(newQueue) == range,"Valid: Last of Queue Option");
	addGold(10);
	//popFromQueue(newQueue,&nCommand,&nStat,tar,newGame,10);

}

commandType getFirstCommand(ActionQueueStructure queue)	{

	return queue->start->command;
}

commandType getLastCommand(ActionQueueStructure queue)	{

	return queue->current->command;
}

upgradeStat getFirstOption(ActionQueueStructure queue)	{

	return queue->start->option;

}

upgradeStat getLastOption(ActionQueueStructure queue)	{

	return queue->current->option;

}

int getFirstTarget()	{

	return (getQueue(NULL))->start->target;

}

int getLastTarget()	{

	return (getQueue(NULL))->current->target;

}

int calulateCosts(commandType cmd, upgradeStat stat, int target)    {

    switch(cmd) {
        case upgrade:
            return (getTowerLevel(target)*getCurrentStat(stat,target));
            break;
        case mktwr:
            return getCostOfNewTower();
            break;
        default:

            break;
    }

    return 0;
}

int getCurrentStat(upgradeStat stat,int target)	{

	switch(stat)	{
		case power:
				return getTowerDamage(target);
				break;	
		case range:
				return getTowerRange(target);
				break;	
		case speed:
				return getTowerSpeed(target);	
				break;	
		case AOErange:
				return getTowerAOErange(target);
				break;	
		case AOEpower:
				return getTowerAOEpower(target);
				break;	
		case level:
				return getTowerLevel(target);
				break;	
		default:
				return 0;
				break;
	}
}

upgradeStat upgradeTowerStat(upgradeStat stat, int target)  {

    switch(stat)    {
        case power:
        {
            if(upgradeDmg(target))  {
                return power;
            }
        }
        case range:
        {
            if(upgradeRange(target))    {
                return range;
            }
        }
        case speed:
        {
            if(upgradeSpeed(target))    {
                return speed;
            }
        }
        case AOErange:
        {
            if(upgradeAOErange(target)) {
                return AOErange;
            }
        }
        case AOEpower:
        {
            if(upgradeAOEpower(target)) {
                return AOEpower;
            }
        }
        default:
            fprintf(stderr,"upgradeTowerStat tower.c: unrecognised stat\n");
            return statError;

    }
}

int popToTower()	{
	ActionQueueStructure queue = getQueue(NULL);
	int needed = calulateCosts(queue->start->command,queue->start->option,queue->start->target);
	GameProperties Game = getGame(NULL);
	
	if((queue->start != NULL) && (checkQueue(queue, Game,needed))){
		ActionQueueStructure queue = getQueue(NULL);
		int needed = calulateCosts(queue->start->command,queue->start->option,queue->start->target);
		GameProperties Game = getGame(NULL);
		upgradeTowerStat(queue->start->option,queue->start->target);
		takeGold(Game, needed);
		QueueNode tempStart = queue->start;
        queue->start = queue->start->nextNode;
		free(tempStart);
		setlastAction(Game);
		--(queue->nItems);
		return 1;	
	}
		return 0;
}


/*
 *Pops from front of Queue.
 */
int popFromQueue(ActionQueueStructure queue, commandType *cmd, upgradeStat *stat, int *target)	{ 
    GameProperties Game = getGame(NULL);
    int needed = calulateCosts(*cmd,*stat,*target);
	if((queue->start != NULL) && (checkQueue(queue,Game, needed)))	{ //!	testing target, available gold, cooldown time 
		*cmd = queue->start->command;
		*stat = queue->start->option;
		*target = queue->start->target;
		QueueNode tempStart = queue->start;
		queue->start = queue->start->nextNode;	
		free(tempStart);	
		--(queue->nItems);
		takeGold(Game, needed);	//remove gold
		setlastAction(Game);	//activate cooldown timer.
		return 1;
	}
		return 0;
}

/*
 * Checks Top of queue to ensure target and time is correct
 */

int checkQueue(ActionQueueStructure queue, GameProperties Game, int needed)	{
	if((checkGold(needed, Game)) && (lastAction(Game)))	{
			return 1;		
	} 
	return 0;	
}

//void testPopFromQueue()	{
//
//	commandType nCommand=upgrade;
//	upgradeStat nStat=power;
//	int tar = 1;	
//	ActionQueueStructure newQueue = createActionQueue();
//	addGold(10);
//	pushToQueue(newQueue,nCommand,nStat,tar);
//	sput_fail_unless(popFromQueue(newQueue,&nCommand,&nStat,&tar)==0, "Invalid: cooldown not ready when trying to pop");
//	delayGame(ACTIONCOOLDOWN);
//	sput_fail_unless(popFromQueue(newQueue,&nCommand,&nStat,&tar)==1, "Valid: cooldown ready when trying to pop");
//	sput_fail_unless(popFromQueue(newQueue,&nCommand,&nStat,&tar)==0, "Invalid: Nothing in queue");
//	pushToQueue(newQueue,nCommand,nStat,tar);
//	delayGame(ACTIONCOOLDOWN);
//	sput_fail_unless(popFromQueue(newQueue,&nCommand,&nStat,&tar)==0, "Invalid: Not enough Gold");
//	addGold(10);
//	sput_fail_unless(popFromQueue(newQueue,&nCommand,&nStat,&tar)==1, "Valid: Have enough gold");
//	addGold(10);
//	pushToQueue(newQueue,nCommand,nStat,tar);
//	sput_fail_unless(popFromQueue(newQueue,&nCommand,&nStat,&tar)==0, "Invalid: Cooldown not ready");
//	
//	free(newQueue);
//}

/*
 *Checks required gold is less than stored gold
 */

int checkGold(int needed, GameProperties Game)	{
	if(needed > getGold(Game)) {
		return 0;
	}
		return 1;
}

void testcheckGold()	{

	GameProperties testGame;
    testGame = createGame();
	addGold(10);
	sput_fail_unless(checkGold(10,testGame) == 1,"boundary Testing enough gold");
	takeGold(testGame,10);
	sput_fail_unless(checkGold(50,testGame) == 0,"Testing not enough gold");
	addGold(100);
	sput_fail_unless(checkGold(10,testGame) == 1,"Testing enough gold");

	free(testGame);
}
