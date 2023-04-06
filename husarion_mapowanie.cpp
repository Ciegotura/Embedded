 
#include <hFramework.h>
#include <DistanceSensor.h>
#include <Lego_Touch.h>

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <string>

//-----------------------------------------------------------------------
//Projekt na mechatronikę ver.0.1.panda3 
//
//todo:
//kalibracja sensor - enkoder (żeby mniej więcej było wiadomo co ile sczytujemy)
//program nadrzędny? 
//
//-----------------------------------------------------------------------
//program for robot
void motionProgram(int time);

//master servo func
void turnRightMaster();
void turnLeftMaster();
void goBackMaster();
void goFwdMaster();

//slave servo func
void turnLeftSlave();
void turnRightSlave();
void goFwdSlave();
void goBackSlave(); 

//frameworks 
void slaveFramework();
void masterFramework();
void prompt();
void sensors();
void bumper();
void encoder();

//general motion commands
void turnLeft();
void turnRight();
void goFwd(int reps);
void goBack(int reps); 

//control logic
bool runState();
bool rotLogic();
bool checkSensors();

//math functions 
int biedaCosinus(int angle);
int biedaSinus(int angle);

//direction typedef
enum dir {forward, backward, left, right, none};

//power and servo parameters 
int power1 = 1000; //650
int power2 = 1000; //700
int ticksLine = 111; //about 10 mm map resolution (print)
int ticksTurn = 115; //90 deg rotation
int repsLine = 1;
int repsTurn = 10;
 
//sensor values
int sensorFront = 999;
int sensorRight = 999;
int sensorLeft = 999;
int sensorRange = 60;
int sensorMax = 999;

//motion parameters and control
dir moveMode = none;
bool moveDone;
bool mot2ready;
bool mot1ready = true;
int servoTick = 15;
int moveInterval = 20;

//control parameters
bool prevState = false;
bool run = false;
bool prevState2 = false;

//logic parameters
int thresholdFront = 25;
int thresholdCorner = 40;
int thresholdSide = 15;
bool bumperState;

//map characters
char charTrack = 'H';
char charOrigin = 'O';
char charObstacle = 'X';
char charPosition = 'C';
char charEmpty = '.';

//testing
unsigned long servPosition1prev = 0;
unsigned long servPosition2prev = 0;

//mapping and recording objects

struct coords //pozycja robota (jako wejście)
{              
    int x = 0;
    int y = 0;
    int angle = 0;
};

struct obstacle //kontener na polozenie przeszkody
{        
    int x = 0;              
    int y = 0;
};

class obstacle_list //lista wykrytych przeszkod
{       
public:
    std::vector<int> x;
    std::vector<int> y; 

    void add(obstacle obs){

        bool flag = false;

        for(int i = 0; i < int(x.size());i++)
        {
            flag = flag || (obs.x == x[i] && obs.y == y[i]);           
        }
        
        if(!flag)
        {
            x.push_back(obs.x);
            y.push_back(obs.y);
        }

    } 

};

class robotpos_list //lista położeń robota
{       
public:
    std::vector<int> x;
    std::vector<int> y; 

    void add(coords robotpos){

        bool flag = false;

        for(int i = 0; i < int(x.size());i++)
        {
            flag = flag || (robotpos.x == x[i] && robotpos.y == y[i]);           
        }

        if(!flag)
        {
            x.push_back(robotpos.x);
            y.push_back(robotpos.y);
        }

    } 

};

//robot position tracking declarations
robotpos_list trackLine;
coords currentPos;
coords roboTracker(int len, int angle);

//obstacle tracking declarations
obstacle_list obstacles;

obstacle detector(dir direction, int sensRead, coords robotpos);

void mapCreate(robotpos_list robotpos, obstacle_list obstacles);

//-----------------------------------------------------------------------
//Main clause

void hMain()
{
    hMot1.setEncoderPolarity(Polarity::Reversed); //changing encoder polarity (Polarity::Normal is default)
	hMot2.setEncoderPolarity(Polarity::Reversed);
    Serial.printf("START"); 

    sys.taskCreate(sensors);
    // sys.taskCreate(encoder);

    //create servo framework tasks
    sys.taskCreate(slaveFramework);
    sys.taskCreate(masterFramework);

    //create runstate check task;
    sys.taskCreate(bumper);


    while (true)
    {
        while(runState()){
            motionProgram(moveInterval);
            trackLine.add(currentPos);
            hLED3.toggle();
            //Serial.printf("x = %d y = %d angle = %d\n", currentPos.x, currentPos.y, currentPos.angle);
	    }
        prompt();
        sys.delay(moveInterval);
    }
    

}

//-----------------------------------------------------------------------
//Program & logic
//Program for robot

void motionProgram(int time)
{ 
    if (checkSensors())
    { 
        goFwd(1);
        //encoder();

        if(bumperState)
        {
            goBack(24);
            
            if (rotLogic())
            {
                turnRight();
            }
            else
            {
                turnLeft();
            }
        }
    }
    else 
    {
        if (rotLogic())
        {
            turnRight();
        }
        else
        {
            turnLeft();
        }
     }
}

//Program logic

bool runState() //run/stop @ button 1 press
{
    bool state = hBtn1.isPressed();
    if(state && state == !prevState){
        run = !run;
        hLED1.toggle();
    }
    prevState = state;
    sys.delay(100);
    return run;
}

void prompt() //special action @ button 2 press
{
    bool state2 = hBtn2.isPressed();
    if(state2 && state2 == !prevState2){
        hLED2.toggle();
    }
    prevState2 = state2;
    sys.delay(100);

    if(state2){
        mapCreate(trackLine, obstacles);
        hLED2.toggle();
    }
    
}

bool rotLogic() //rotation direction logic 
{
    return sensorLeft < sensorRight;
}

bool checkSensors() //rotation trigger clause
{
    bool chkFront = false; 
    bool chkLeft = false;
    bool chkRight = false;

    chkFront = (sensorFront > thresholdFront);
    chkLeft = !((sensorLeft < thresholdSide) && (sensorFront < thresholdCorner));
    chkRight = !((sensorRight  < thresholdSide) && (sensorFront < thresholdCorner));

    return chkFront && chkRight && chkLeft;
}

//-----------------------------------------------------------------------
//subtasks & frameworks

// frameworks

void slaveFramework() //slave servo framework
{
    while(true){
    switch (moveMode){
        case forward: //forward
            goFwdSlave();
            break;

        case left: //turnleft
	        turnLeftSlave();
            break;

        case right: //turnright
        	turnRightSlave();
            break;

        case backward: //back
        	goBackSlave();
            break;

        default:
            sys.delay(servoTick);
            break;
        }
    }
}

void masterFramework() //master servo framework
{
    while(true){
    switch (moveMode){ //slave motor
        case forward: //forward
            goFwdMaster();
			moveMode = none;
            break;

        case left: //turnleft
	        turnLeftMaster();
			moveMode = none;
            break;

        case right: //turnright
        	turnRightMaster();
			moveMode = none;
            break;

        case backward: //back
        	goBackMaster();
			moveMode = none;
            break;

        default:
            sys.delay(servoTick);
            break;
        }
    }
}

// sensor tasks

void bumper() //bumper framework
{
    hLegoSensor_simple ls(hSens6);
    hSensors::Lego_Touch sensor(ls);
    
    for(;;)
    {
        bumperState = !sensor.isPressed();
        sys.delay(10);
    }
}

void sensors() //sensor framework
{
    hModules::DistanceSensor sensFront(hSens1);
    hModules::DistanceSensor sensRight(hSens2);
    hModules::DistanceSensor sensLeft(hSens3);
    
    obstacle obsLeft;
    obstacle obsFront;
    obstacle obsRight;

    for(;;)
    {

        if (moveDone)
        {
            sensorFront = 9 + sensFront.getDistance();
            sensorRight = 7 + sensRight.getDistance();
            sensorLeft = 7 + sensLeft.getDistance();

            if(sensorFront > sensorRange || sensorFront < 0)
            {
                sensorFront = sensorMax;
            }
            else
            {
                //obsFront = detector(forward, sensorFront, currentPos);
                //obstacles.add(obsFront);
            }

            if(sensorLeft > sensorRange || sensorLeft < 0)
            {
                sensorLeft = sensorMax;
            }
            else
            {
                obsLeft = detector(left, sensorLeft, currentPos);
                obstacles.add(obsLeft);
            }

            if(sensorRight > sensorRange || sensorRight < 0)
            {
                sensorRight = sensorMax;
            }
            else
            {
                obsRight = detector(right, sensorRight, currentPos);
                obstacles.add(obsRight);
            }

            //  Serial.printf("front = %d ",sensorFront);
            //  Serial.printf("left = %d ",sensorLeft);
            //  Serial.printf("right = %d\n ",sensorRight);
        }

    sys.delay(moveInterval);

    }
}

void encoder() //encoder framework (unused)
{
    unsigned long servPosition1;
    unsigned long servPosition2;

        
        servPosition1 = hMot1.getEncoderCnt();
        servPosition2 = hMot2.getEncoderCnt();
        Serial.printf("pos_mot_1: %d\r\n", servPosition1 - servPosition1prev); //Serwo wydrukować aktualną pozycję silnika 1 (ilość taktów enkodera)
        Serial.printf("pos_mot_2: %d\r\n", servPosition2 - servPosition2prev); //Serwo wydrukować aktualną pozycję silnika 2 (ilość taktów enkodera)
        servPosition1prev = servPosition1;
        servPosition2prev = servPosition2;
}

//-----------------------------------------------------------------------
//Servo commands - individual & general

//Master function definitions

void turnRightMaster()
{
        for(int i = 0; i<repsTurn; i++){
        		mot2ready = true; 
        		hMot2.rotRel(-ticksTurn,power2,1);
        		mot2ready = false;
        		sys.delay(servoTick);
        	}
        moveDone = true;
}

void turnLeftMaster()
{
        for(int i = 0; i<repsTurn; i++){
        		mot2ready = true; 
        		hMot2.rotRel(ticksTurn,power2,1);
        		mot2ready = false;
        		sys.delay(servoTick);
        	}
        moveDone = true;
}

void goBackMaster()
{
        for(int i = 0; i<repsLine; i++){
        		mot2ready = true; 
        		hMot2.rotRel(ticksLine,power2,1);
        		mot2ready = false;
        		sys.delay(servoTick);
        	}
        moveDone = true;         
}

void goFwdMaster()
{
        for(int i = 0; i<repsLine; i++){
        		mot2ready = true; 
        		hMot2.rotRel(-ticksLine,power2,1);
        		mot2ready = false;
        		sys.delay(servoTick);
        	}
        moveDone = true;
}

//Slave function definitions

void turnLeftSlave()
{	
        while(!moveDone){
	        if (mot2ready){
	        	sys.delay(servoTick);
       			hMot1.rotRel(-ticksTurn,power1,1);
       		}
       	}
}

void turnRightSlave()
{	
        while(!moveDone){
	        if (mot2ready){
	        	sys.delay(servoTick);
       			hMot1.rotRel(ticksTurn,power1,1);
       		}
       	}
}

void goFwdSlave()
{	
        while(!moveDone){
	        if (mot2ready){
	        	sys.delay(servoTick);
       			hMot1.rotRel(-ticksLine,power1,1);
       		}
       	}
}

void goBackSlave()
{	
        while(!moveDone){
	        if (mot2ready){
	        	sys.delay(servoTick);
       			hMot1.rotRel(ticksLine,power1,1);
       		}
        }
}
// motion commands definition

void turnLeft()
{
    moveDone = false;
    moveMode = left;
    currentPos = roboTracker(0,90);
    while(!moveDone)
    {
        sys.delay(servoTick);
    }
    sys.delay(2*moveInterval);
}

void turnRight()
{
    moveDone = false;
    moveMode = right;
    currentPos = roboTracker(0,-90);
    while(!moveDone)
    {
        sys.delay(servoTick);
    }
    sys.delay(2*moveInterval);
}

void goFwd(int reps)
{    
    moveDone = false;
    moveMode = forward;
    repsLine = reps;
    currentPos = roboTracker(reps,0);
    while(!moveDone)
    {
        sys.delay(servoTick);
    }
}

void goBack(int reps)
{
    moveDone = false;
    moveMode = backward;
    repsLine = reps;
    currentPos = roboTracker(-reps,0);
    while(!moveDone)
    {
        sys.delay(servoTick);
    }
}

//-----------------------------------------------------------------------
//Mapping 

coords roboTracker(int len, int angle) //Robot position tracker
{
    coords nextPos;
    nextPos.x = currentPos.x + biedaCosinus(currentPos.angle)*len;
    nextPos.y = currentPos.y - biedaSinus(currentPos.angle)*len;
    nextPos.angle = currentPos.angle + angle;
    if (nextPos.angle >= 360)
    {
        nextPos.angle = nextPos.angle - 360;
    } 
    
    if (nextPos.angle < 0)
    {
        nextPos.angle = nextPos.angle + 360;
    } 
    return nextPos;
}

obstacle detector(dir direction, int sensRead, coords robotpos) //obstacle detector :D
{
    obstacle result;

    switch(direction){
        case left:
            result.x = robotpos.x - biedaSinus(robotpos.angle)*sensRead;
            result.y = robotpos.y - biedaCosinus(robotpos.angle)*sensRead;
            break;

        case right:
            result.x = robotpos.x + biedaSinus(robotpos.angle)*sensRead;
            result.y = robotpos.y + biedaCosinus(robotpos.angle)*sensRead;
            break;

        case forward:
            result.x = robotpos.x + biedaCosinus(robotpos.angle)*sensRead;
            result.y = robotpos.y - biedaSinus(robotpos.angle)*sensRead;
            break;
        
    }

    return result;

}

void mapCreate(robotpos_list robotpos, obstacle_list obstacles) //Map creator
{
    int height = 0;
    int width = 0;

    int *x_max_r = &*std::max_element( std::begin(robotpos.x), std::end(robotpos.x) );
    int *x_min_r = &*std::min_element( std::begin(robotpos.x), std::end(robotpos.x) );
    int *y_max_r = &*std::max_element( std::begin(robotpos.y), std::end(robotpos.y) );
    int *y_min_r = &*std::min_element( std::begin(robotpos.y), std::end(robotpos.y) );

    int *x_max_o = &*std::max_element( std::begin(obstacles.x), std::end(obstacles.x) );
    int *x_min_o = &*std::min_element( std::begin(obstacles.x), std::end(obstacles.x) );
    int *y_max_o = &*std::max_element( std::begin(obstacles.y), std::end(obstacles.y) );
    int *y_min_o = &*std::min_element( std::begin(obstacles.y), std::end(obstacles.y) );

    int x_max = max(*x_max_r, *x_max_o);
    int x_min = min(*x_min_r, *x_min_o);
    int y_max = max(*y_max_o, *y_max_r);
    int y_min = min(*y_min_o, *y_min_r);
    
    width = x_max - x_min;
    height = y_max - y_min;


    Serial.printf("szer = %d, wys = %d\n", width, height);

    Serial.printf("xmax = %d, xmin = %d\n", x_max, x_min);

    Serial.printf("ymax = %d, ymin = %d\n", y_max, y_min);

    for(int i = 0; i < int(obstacles.x.size()); i++){
        //Serial.printf("x = %d, y = %d\n", obstacles.x[i], obstacles.y[i]);
    }

    bool flag1 = false;
    bool flag2 = false;
    // Serial.printf("map insert ok\n");

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            for(int k=0; k < int(robotpos.x.size()); k++)
            {
            
                flag1 = flag1 || (i == abs(x_min) + robotpos.x[k] && j == abs(y_min) + robotpos.y[k]);   
            
            }

            for(int k=0; k < int(obstacles.x.size()); k++)
            {
                flag2 = flag2 || (i == abs(x_min) + obstacles.x[k] && j == abs(y_min) + obstacles.y[k]);
            }
            
            if(flag1)
            {
                Serial.printf("%c", charTrack); //trasa robota
            }
            else if (flag2)
            {
                Serial.printf("%c", charObstacle); //przeszkoda
            }
            else if (i == abs(x_min) && j == abs(y_min))
            {
                Serial.printf("%c", charOrigin);
            }
            else if (i == abs(x_min) + *robotpos.x.end() && j == abs(y_min) + *robotpos.y.end())
            {
                Serial.printf("%c", charPosition);
            }
            else
            {
                Serial.printf("%c", charEmpty); //puste miejsce
            }
            flag1 = false;
            flag2 = false;
        }
        Serial.printf("\n");
    }    

    
    // Serial.printf("map print ok\n");
}

//math functions for mapping
int biedaSinus(int angle) //baaardzo biedny sinus XD
{  
    int result;
    switch (angle){
        case 0:
            result = 0;
            break;

        case 90:
            result = 1;
            break;
        
        case 180:
            result = 0;
            break;

        case 270:
            result = -1;
            break;
    }
    return result;
}

int biedaCosinus(int angle) //równie biedny cosinus
{    
    int result;
    switch (angle){
        case 0:
            result = 1;
            break;

        case 90:
            result = 0;
            break;
        
        case 180:
            result = -1;
            break;

        case 270:
            result = 0;
            break;
    }
    return result;
}

