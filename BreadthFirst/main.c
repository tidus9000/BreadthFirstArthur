/*
 * File:   main.cpp
 * Author: j4-smith
 *N-Queens example in C
 * First Created on 25 April 2014, 18:42
 * Modified in light of student progress 
 
 * Rewritten Oct 2015 to use more generic solution structures 
 * and functions with NQueens-specific names that call the functions that modify the
 */


#include <stdio.h>
#include <stdlib.h>
#include "StructureDefinitions.h"
#include "SolutionListOperations.h"
#include "NQueensChecker.h"

#pragma warning (disable: 4996)

//some #defined values
#define NUMPEGS 4
#define LEFTOUTER 0
#define LEFTMID 1
#define RIGHTMID 2
#define RIGHTOUTER 3
#define TRUE 1
#define FALSE 0

//a way of getting the number of whitre/balck pegs from the int score
#define WHITEPEGS(x) ( (x) % NUMPEGS)
#define BLACKPEGS(x) ((x)/NUMPEGS)

void SetNewChallenge(char *challenge);
int ScoreCandidateSolution(char *attempt, char *target);
void CopyworkingCandidateTocolours(int colours[], candidateSolution workingCandidate);
candidateSolution CopycoloursToworkingCandidate(int colours[]);

char GetValidGuessForPeg(void);

candidateSolution workingCandidate;//this one will hold the soltion we arecurrently considering
candidateList  currentListOfCandidates; // this list will store all the soltion we;ve created but not examined yet
candidateList listOfExaminedCandidates; // this is where we will store all the ones we're done with



//************************ don't edit anything above this line***********************//


int main(int argc, const char * argv[])
{
    
    /* the next set of variables get used within our main function
     * but aren't declared globally becuase we don't want the other functions to change them inadvertently */
    int numberOfCompleteSolutionsFound = 0; //simple flag to let us know whether we have stopped
    int numberOfSolutionsExamined = 0; //simple counter
    int indexOfSolutionWeAreLookingAt; //index in list of current solution being examined
    int newLength, valueToAdd; // used when we extend the working candidate
	
	char target[NUMPEGS], candidate[NUMPEGS];

	char userResponse;
	int attempts = 1, scoreForAttempt = 0, peg = 0;
	int colours[NUMPEGS];
	int listIndex = 0, listSize = 0;
	int duplicateInList = FALSE;

	for (int i = 0; i < NUMPEGS; i++)
	{
		colours[i] = 0;
	}

	// begin by setting a new goal
	SetNewChallenge(target);

	scoreForAttempt = 0;


    //start off by emptying the lists of candidate solutions
    CleanListsOfSolutionsToStart();
	CleanWorkingCandidate();
    
    /* So now let's start by creating our first solution
     * which we do by filling the values into the variable we declared as the workingCandidate
     * We'll begin by putting the first queen into column ), or specified from the command line
   */
   valueToAdd = 0;
   if(argc==2)
     {
       valueToAdd = atoi(argv[1]);
       if (valueToAdd >=N || valueToAdd <0)
           PrintThisMessageAndExit("Invalid Input");
     }

   for (int i = 0; i < NUMPEGS; i++)
   {
	   workingCandidate.variableValues[i] = colours[i];
	   workingCandidate.numberOfDefinedValues++;
   }

   AddWorkingCandidateToCurrentList();
   listSize++;

   // now lets do the main goal loop
   while (BLACKPEGS(scoreForAttempt) < NUMPEGS)
   {
	   CopySolutionFromCurrentListIntoWorkingCandidate(listIndex);
	   //RemoveSolutionFromCurrentList(listIndex);
	   //listSize--;

	   CopyworkingCandidateTocolours(colours, workingCandidate);
	   CleanWorkingCandidate();

	   //loop over each peg position
	   for (peg = 0; peg < NUMPEGS; peg++)
	   {
		//convert numerical solution into char array for checking solution
		   char rgby[4] = { 'r', 'g', 'b', 'y' };
		   candidate[peg] = rgby[colours[peg]];
	   }
		   attempts++;
		   scoreForAttempt = ScoreCandidateSolution(candidate, target);


		   //populating search tree
		   for (int i = 0; i < NUMPEGS; i++)
		   {
			   //creating an array that can be modified and placed into the list
			   int tempColours[NUMPEGS];
			   for (int j = 0; j < NUMPEGS; j++)
			   {
				   //give that array the same values as our candidate
				   tempColours[j] = colours[j];
			   }

			   for (int j = 0; j < 4; j++)
			   {
				   for (int k = 0; k < NUMPEGS; k++)
				   {
					   tempColours[k] = colours[k];
				   }

				   //loop and if statement to change the value of one of the array positions and put the change in the list
				   if (colours[i] != j)
				   {
					   tempColours[i] = j;

					   for (int l = 0; l <= listSize; l++)
					   {
						   int coloursCheck[NUMPEGS];
						   CopySolutionFromCurrentListIntoWorkingCandidate(l);
						   for (int m = 0; m < NUMPEGS; m++)
						   {
							   coloursCheck[m] = workingCandidate.variableValues[m];
						   }
						   CleanWorkingCandidate();

						   //check that this change is not already in the list to avoid overpopulating the list and going down duplicate routes
						   if (coloursCheck[0] == tempColours[0] && coloursCheck[1] == tempColours[1] && coloursCheck[2] == tempColours[2] && coloursCheck[3] == tempColours[3])
						   {
							   duplicateInList = TRUE;
						   }

					   }

					   //if this sequence is not already in the list, then add it to the list
					   if (duplicateInList == FALSE)
					   {
						   for (int k = 0; k < NUMPEGS; k++)
						   {
							   workingCandidate.variableValues[k] = tempColours[k];
							   workingCandidate.numberOfDefinedValues++;
						   }
						   AddWorkingCandidateToCurrentList();
						   listSize++;
						   CleanWorkingCandidate();
					   }

					   duplicateInList = FALSE;
				   }
			   }

		   }

		   //AddWorkingCandidateToCurrentList();

		  listIndex++;
	   /* now the attempt is complete we can call the function to score it
	   * that will set the values of the variables  numberOfRightPieces  and numberOfRightColoursInWrongPlace
	   * It would be friendly to print out a message saying what the score was...
	   */

	   if (BLACKPEGS(scoreForAttempt) == NUMPEGS)
	   {
		   printf("Well done you have solved the puzzle in %d attempts!\n", attempts);

	   }
   } //end of while loop
	   
	//workingCandidate.variableValues[0] = 1;
	//workingCandidate.numberOfDefinedValues++;
	////AddQueenToNextRowInColumn(valueToAdd);
 //   //CalculateNumberOfVulnerableQueensForWorkingCandidate(); //should return zero because there is only one queen:)
 //   numberOfSolutionsExamined=1;
 //   
 //   //and we can put this as our first item in the list to start the process
 //   AddWorkingCandidateToCurrentList();
	//CopySolutionFromCurrentListIntoWorkingCandidate(0);
	//printf("%i", workingCandidate.variableValues[0]);
	//AddSolutionPram1_ToListParam2(&workingCandidate, &currentListOfCandidates);
 //   
    
    //Now we will go into a loop examining solutions until we find one that is full and has no vulnerable queens
    
   // end of  statement dealing with search
    
    
    
    
    return 0;
}

void CopyworkingCandidateTocolours(int colours[], candidateSolution workingCandidate)
{
	for (int i = 0; i < NUMPEGS; i++)
	{
		colours[i] = workingCandidate.variableValues[i];
	}
}

candidateSolution CopycoloursToworkingCandidate(int colours[])
{
	candidateSolution workingCandidate;
	for (int i = 0; i < NUMPEGS; i++)
	{
		workingCandidate.variableValues[i] = colours[i];
		workingCandidate.numberOfDefinedValues++;
	}
	return workingCandidate;
}


char GetValidGuessForPeg(void)
{
	char guess = '-', userResponse;
	int  readOK = 0;
	while (readOK == 0)
	{
		printf("\n\tPlease enter one of r, g, b or y followed by <return>:");
		// read two chars: first is the value they input (stored in UserResponse),
		// second is the <return which we discard using %*c
		scanf("%c%*c", &userResponse);
		switch (userResponse)
		{
		case 'r': //fall through to 4th ok case
		case 'y':
		case 'g':
		case 'b': readOK = 1;
			guess = userResponse;
			break;
		default: readOK = 0;
		}
	}

	return guess;

}


/////DONT CHANGE ANYTHING BELOW THIS LINE


/* void SetNewGoal(void)
* simple function that creates 4 randon colours for the target
* doesn;t need any inputs or outputs
*/
void SetNewChallenge(char *challenge)
{

	int colourAsInt, peg;
	char colour;
	//initialise the rando, number generator from the time in seconds
	//loop over each position in the target
	srand((unsigned int)time(NULL));

	for (peg = 0; peg<NUMPEGS; peg++)
	{
		//generate a number between 0 and 3
		colourAsInt = rand() % 4;
		//convert this to a char for thre colour
		if (colourAsInt == 0)
			colour = 'r';
		else if (colourAsInt == 1)
			colour = 'y';
		else if (colourAsInt == 2)
			colour = 'b';
		else
			colour = 'g';
		//set the value
		challenge[peg] = colour;
	}


	//printf("Target sequence is:");
	for (peg = 0; peg<NUMPEGS; peg++)
	{
		//printf(" %c", challenge[peg]);
	}
	//printf("\n");
}


/* void ScoreThisTry(void);
* function that change the values of the counter of correct/mispalced counters to score this attempt
* no inout or output
*/
int ScoreCandidateSolution(char *attempt, char *target)
{

	//start off by zero-ing the counters
	int numberOfRightPieces = 0, numberOfRightColoursInWrongPlace = 0;

	//keep track of how many times we have counted each of the goal colours
	int peg, peg2, pegUsedInScore[NUMPEGS];
	for (peg = 0; peg<NUMPEGS; peg++)
	{
		pegUsedInScore[peg] = 0;
	}


	//look for utterly correct pieces first
	for (peg = 0; peg<NUMPEGS; peg++)
	{
		if (target[peg] == attempt[peg])
		{
			pegUsedInScore[peg] = 1;
			numberOfRightPieces++;
		}
	}

	//if not solved, we need to count partial matches
	if (numberOfRightPieces <NUMPEGS)
	{

		//consider each peg in turn
		for (peg = 0; peg < NUMPEGS; peg++)
		{
			//if peg was not a match in the right place
			if (attempt[peg] != target[peg])
			{
				//look elsewhere along the target  for a possible match
				for (peg2 = 0; peg2 < NUMPEGS; peg2++)
				{
					//only consider target pegs that have not already been matched already
					if ((pegUsedInScore[peg2] == 0) && (attempt[peg] == target[peg2]))
					{
						//increase count of white pegs
						numberOfRightColoursInWrongPlace++;
						//mark target peg as no longer available for matching
						pegUsedInScore[peg2] = 1;
						//stop looking at this peg in the attempt
						break;
					}
				}

			}
		}

	}
	return (NUMPEGS* numberOfRightPieces + numberOfRightColoursInWrongPlace);

}



