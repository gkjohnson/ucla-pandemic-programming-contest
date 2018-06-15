//------------------------------------------------------------------
// AIBotExample.cpp : Replace code here!
//------------------------------------------------------------------	

#include "DllEntry.h"
#include <windows.h>
#include <time.h>
#include <algorithm>

void DrawAiPaths(const sWorldInfo &mWorldInfo,  void (*DrawLine)(vec2,vec2,vColor,float));
vec2 PathFinding(vec2 currPos,vec2 targetPos,const sWorldInfo &mWorldInfo);
void recPathing(int prevNode,int nextNode,const sWorldInfo &mWorldInfo, int &bestnum, int curTurns, int target,int initNode,int &bestNode,int visited[500]);

extern "C" __declspec(dllexport)
void dllmonsteraction(const float dt, 
					  sEntInfo &mEnt, 
					  const sWorldInfo &mWorldInfo, 
					  void (*DrawLine)(vec2,vec2,vColor,float))
{
	// Find a target that's still alive.
	const sOtherEnts *pTarget = 0;
	static int iCount=0;
	static vec2 dest=mEnt.pos;
	static vec2 target=vec2(528.369,632.6);

	//Current Desire	
	static eEntityTypes desire=TYPE_QUADDAMAGE_PICKUP;
	static float desiredist=10000;
	static bool carryTaken=true;
	static bool shotgunTaken=true;
	static bool grenadesTaken=true;
	static float distToBarrel=10000;

	/*if (mEnt.iHealth<50&&mEnt.iCarryObject==0){
		desire=TYPE_HEALTH_PICKUP;
	}else */if(mEnt.iCarryObject==0){
		desire=TYPE_CARRYOBJECT_PICKUP;	
	}else if(mEnt.iCarryObject>0){
		desire=TYPE_CARRYTARGET;
	}
	carryTaken=true;

	static float gunsDist=10000;
	for(int i = 0; i < mWorldInfo.iNumOtherEnts; ++i){
			if(mWorldInfo.pOtherEnts[i].type == TYPE_CARRYOBJECT_PICKUP){
				carryTaken=false;
			}
	}

		for(int i = 0; i < mWorldInfo.iNumOtherEnts; ++i)
	{
		/*
		if(mWorldInfo.pOtherEnts[i].type == TYPE_FIREBALL)
			iCount++;
		else if(mWorldInfo.pOtherEnts[i].type == TYPE_ROCKET)
			iCount++;
		else if(mWorldInfo.pOtherEnts[i].type == TYPE_GRENADE)
			iCount++;
		else if(mWorldInfo.pOtherEnts[i].type == TYPE_SHOTGUN_PICKUP)
			iCount++;
		else if(mWorldInfo.pOtherEnts[i].type == TYPE_CHAINGUN_PICKUP)
			iCount++;
		else if(mWorldInfo.pOtherEnts[i].type == TYPE_ROCKET_PICKUP)
			iCount++;
		else if(mWorldInfo.pOtherEnts[i].type == TYPE_RIFLE_PICKUP)
			iCount++;
		else if(mWorldInfo.pOtherEnts[i].type == TYPE_GRENADE_PICKUP)
			iCount++;
		else if(mWorldInfo.pOtherEnts[i].type == TYPE_HEALTH_PICKUP)
			iCount++;
		else if(mWorldInfo.pOtherEnts[i].type == TYPE_TELEPORT)
			iCount++;
		*/


		if(mWorldInfo.pOtherEnts[i].type==TYPE_MODEL){
			if(Length(mWorldInfo.pOtherEnts[i].pos-mEnt.pos)<distToBarrel){
				distToBarrel=Length(mWorldInfo.pOtherEnts[i].pos-mEnt.pos);
			}

		}

		if(mWorldInfo.pOtherEnts[i].type==TYPE_SHOTGUN_PICKUP){
			shotgunTaken=false;
		}else if(mWorldInfo.pOtherEnts[i].type==TYPE_SHOTGUN_PICKUP){
			grenadesTaken=false;
		}

		if(mWorldInfo.pOtherEnts[i].type == TYPE_ENEMY)
		{
			pTarget = &mWorldInfo.pOtherEnts[i];
			//break;
		}
		//if (mWorldInfo.pOtherEnts[i].type == TYPE_CARRYOBJECT_PICKUP){
		/*if(mWorldInfo.pOtherEnts[i].type==TYPE_CARRYOBJECT_PICKUP){
			carryTaken=false;
			std::cout<<"TEST"<<std::endl;
		}*/
		int lookDist=100;
		//std::cout<<carryTaken<<std::endl;
		if(carryTaken==false){
			lookDist=20;
		}else if(carryTaken==true&&mEnt.iCarryObject==0){
			lookDist=400;
			//std::cout<<"ENTERED"<<std::endl;
		}

		if(Length(mWorldInfo.pOtherEnts[i].pos-mEnt.pos)<lookDist&&carryTaken==true){
			if((mWorldInfo.pOtherEnts[i].type==TYPE_SHOTGUN_PICKUP)
				||(mWorldInfo.pOtherEnts[i].type==TYPE_CHAINGUN_PICKUP)
				||(mWorldInfo.pOtherEnts[i].type==TYPE_ROCKET_PICKUP)
				||(mWorldInfo.pOtherEnts[i].type==TYPE_GRENADE_PICKUP)
				||(mWorldInfo.pOtherEnts[i].type==TYPE_QUADDAMAGE_PICKUP)
				||(mWorldInfo.pOtherEnts[i].type==TYPE_CARRYOBJECT_PICKUP)
				){
					
					if(Length(mWorldInfo.pOtherEnts[i].pos-mEnt.pos)<gunsDist){
						target=mWorldInfo.pOtherEnts[i].pos;
						desire=mWorldInfo.pOtherEnts[i].type;
					}

					/*if(mWorldInfo.pOtherEnts[i].type==TYPE_SHOTGUN_PICKUP&&(desire!=TYPE_SHOTGUN_PICKUP)){
					desire=mWorldInfo.pOtherEnts[i].type;}
					else if(mWorldInfo.pOtherEnts[i].type==TYPE_GRENADE_PICKUP&&desire!=TYPE_SHOTGUN_PICKUP){
					desire=mWorldInfo.pOtherEnts[i].type;}
					else if(mWorldInfo.pOtherEnts[i].type==TYPE_ROCKET_PICKUP&&desire!=TYPE_SHOTGUN_PICKUP&&desire!=TYPE_GRENADE_PICKUP){
					desire=mWorldInfo.pOtherEnts[i].type;}
					else if(mWorldInfo.pOtherEnts[i].type==TYPE_CHAINGUN_PICKUP&&desire!=TYPE_SHOTGUN_PICKUP&&desire!=TYPE_GRENADE_PICKUP&&desire!=TYPE_ROCKET_PICKUP){
					desire=mWorldInfo.pOtherEnts[i].type;}
					else if(mWorldInfo.pOtherEnts[i].type==TYPE_CHAINGUN_PICKUP&&desire!=TYPE_SHOTGUN_PICKUP&&desire!=TYPE_GRENADE_PICKUP&&desire!=TYPE_ROCKET_PICKUP&&desire!=TYPE_QUADDAMAGE_PICKUP){
					desire=mWorldInfo.pOtherEnts[i].type;}*/

					//DrawLine(mEnt.pos,mWorldInfo.pOtherEnts[i].pos,WHITE,0.f);

			}
			if((Length(mWorldInfo.pOtherEnts[i].pos-mEnt.pos)<30)&&(mWorldInfo.pOtherEnts[i].type==TYPE_HEALTH_PICKUP)&&(mEnt.iHealth<70)){
					target=mWorldInfo.pOtherEnts[i].pos;
					desire=mWorldInfo.pOtherEnts[i].type;
			}else 	if((Length(mWorldInfo.pOtherEnts[i].pos-mEnt.pos)<70)&&(mWorldInfo.pOtherEnts[i].type==TYPE_HEALTH_PICKUP)&&(mEnt.iHealth<50)){
					target=mWorldInfo.pOtherEnts[i].pos;
					desire=mWorldInfo.pOtherEnts[i].type;
			}else 	if((Length(mWorldInfo.pOtherEnts[i].pos-mEnt.pos)<100)&&(mWorldInfo.pOtherEnts[i].type==TYPE_HEALTH_PICKUP)&&(mEnt.iHealth<30)){
					target=mWorldInfo.pOtherEnts[i].pos;
					desire=mWorldInfo.pOtherEnts[i].type;
			}
		}


			if(mEnt.iCarryObject>0){
				if(mEnt.iGrenades==0&&grenadesTaken==false){
					desire=TYPE_GRENADE_PICKUP;
				}
			}

		if (desire==mWorldInfo.pOtherEnts[i].type){
			if(Length(mWorldInfo.pOtherEnts[i].pos-mEnt.pos)<desiredist){
				desiredist=Length(mWorldInfo.pOtherEnts[i].pos-mEnt.pos);
				target=mWorldInfo.pOtherEnts[i].pos;
			}
			//std::cout<<"("<<mWorldInfo.pOtherEnts[i].pos.x<<","<<mWorldInfo.pOtherEnts[i].pos.y<<std::endl;
			//dest=mWorldInfo.pOtherEnts[i].pos;
		}

	}
		//std::cout<<carryTaken<<std::endl;
		/*else if (carryTaken==true&&mEnt.iCarryObject==0){
		//std::cout<<"ENTERED"<<std::endl;
		desire=TYPE_SHOTGUN_PICKUP;
	}*/
	carryTaken=true;
		desiredist=10000;

	// Move to the last click position
	// vec2(0,0) as mWorldInfo.vLastMouseClick is the initial value, so we can ignore that.

		// Build the move direction. This must be a normalized direction vector.
	//MOVEMENT//
	dest=mEnt.pos;
	if((Length(target-mEnt.pos)<90)&&desire==TYPE_CARRYOBJECT_PICKUP){
		dest=target;
	}else if((Length(target-mEnt.pos)<40)&&desire!=TYPE_SHOTGUN_PICKUP&&desire!=TYPE_HEALTH_PICKUP){
		dest=target;
	}else if(Length(dest-mEnt.pos)<5.0f){
		dest=PathFinding(dest,target,mWorldInfo);
	}
	
	static vec2 barrelVec=vec2(0,0);

	for(int i = 0; i < mWorldInfo.iNumOtherEnts; ++i)
	{
		if(mWorldInfo.pOtherEnts[i].type==TYPE_MODEL){
			if(Length(mWorldInfo.pOtherEnts[i].pos-mEnt.pos)<15){
				distToBarrel=Length(mWorldInfo.pOtherEnts[i].pos-mEnt.pos);
				vec2 barrelDir=(mWorldInfo.pOtherEnts[i].pos-mEnt.pos);
				//barrelDir.x*=Length(barrelDir);
				//barrelDir.y*=Length(barrelDir);
				//barrelDir.x=100/(barrelDir.x);
				//barrelDir.y=rand()/(barrelDir.y);
				barrelVec=barrelDir;
			}

		}
	}

	//DrawLine(mEnt.pos,(barrelVec)+mEnt.pos,BLUE,0.f);
	

		mEnt.moveDirection = Normalize(dest - mEnt.pos);



	
		//std::cout<<mWorldInfo.iNumPathNodes;

		// You can draw lines for debugging with the Drawline function pointer passed in.
		// Note that there are some predefined colors in DllEntry.h, or you can make your own.
		// DrawLine(start, end, color, duration)
		//DrawLine(mEnt.pos, mWorldInfo.vLastMouseClick, MAGENTA, 0.f);

	static vec2 click=vec2(0,0);
	//Getting World Info Values

	//DrawLine(mEnt.pos,mWorldInfo.pPathNodes[100].vPos,MAGENTA,0.f);
	//std::cout<<mWorldInfo.pPathNodes[100].connectingNodeIndex[0];

	static int i=0;
	//DrawLine(mEnt.pos,mWorldInfo.pPathNodes[i].vPos,MAGENTA,0.f);
	
/*	i++;
	if (i>mWorldInfo.iNumPathNodes){i=0;}
*/
	/*if(click!=mWorldInfo.vLastMouseClick){
	std::cout<<"("<<mWorldInfo.vLastMouseClick.x<<","<<mWorldInfo.vLastMouseClick.y<<")"<<std::endl;


	}
	
	if(mWorldInfo.vLastMouseClick.x){
		click = mWorldInfo.vLastMouseClick;
	}*/

	// Full speed ahead always.
		mEnt.fMoveSpeed = MAX_ENT_SPEED;


		
	//mEnt.fMoveSpeed = 0;
	if(pTarget)
	{
		//mEnt.nextCommand = OP_SHOOT_CHAINGUN;
		/*
		if(mEnt.iGrenades>0)
			mEnt.nextCommand = OP_SHOOT_GRENADE;
		else if(mEnt.iShotgunShells>0)
			mEnt.nextCommand = OP_SHOOT_SHOTGUN;
		else if(mEnt.iChaingunBullets>0)
			mEnt.nextCommand = OP_SHOOT_CHAINGUN;
		else if(mEnt.iRockets>0)
			mEnt.nextCommand = OP_SHOOT_ROCKET;
		else if(mEnt.iRifleBullets>0)
			mEnt.nextCommand = OP_SHOOT_RIFLE;
		else
			mEnt.nextCommand = OP_SHOOT_FIREBALL;
		*/
		// Always aim at the target.

		//// Aim ahead of target ////
		
		static int amSpeed=FIREBALL_SPEED;
		//std::cout<<distToBarrel<<std::endl;
		if(Length(pTarget->pos-mEnt.pos)<13.0){
			amSpeed=10000;
		}else if(Length(pTarget->pos-mEnt.pos)<40.0&&mEnt.iShotgunShells>0){
			amSpeed=10000;
		}else if(Length(pTarget->pos-mEnt.pos)<120&&mEnt.iChaingunBullets>0){
			amSpeed=10000;
		}else if(Length(pTarget->pos-mEnt.pos)<200.0&&mEnt.iRockets>0&&distToBarrel>4){
			amSpeed=ROCKET_SPEED;
		}else if(Length(pTarget->pos-mEnt.pos)<220&&mEnt.iGrenades>0&&distToBarrel>4){
			amSpeed=ROCKET_SPEED;
		}else{
			amSpeed=FIREBALL_SPEED;
		}
		
	

		vec2 pos = pTarget->pos; // pos is position of enemy
		

		float t = .8*Length(pos-mEnt.pos)/(amSpeed - Length(pTarget->vel)); // find t time

		//DrawLine(mEnt.pos,pos+(pTarget->vel)*t, GREEN,0.f);

		//pos += vec2(5, 0);
		mEnt.aimDirection = Normalize(pos - mEnt.pos+(pTarget->vel)*t);

		
		
	}
	else
	{
		mEnt.nextCommand = OP_ANIM_WIN;
	}
	
	DrawAiPaths(mWorldInfo, DrawLine);
	DrawLine(mEnt.pos,dest,WHITE,0.f);
	//DrawLine(mEnt.pos,dest,RED,0.f);

	//SHOOTING//
	if(pTarget){
	if(pTarget->bIsPenalty==false){
	if(Length(pTarget->pos-mEnt.pos)<13.0){
		mEnt.nextCommand = OP_ATTACK_FIST;
	}else if(Length(pTarget->pos-mEnt.pos)<100.0&&mEnt.iShotgunShells>0){
		mEnt.nextCommand = OP_SHOOT_SHOTGUN;
	}else if(Length(pTarget->pos-mEnt.pos)<170&&mEnt.iChaingunBullets>0){
		mEnt.nextCommand = OP_SHOOT_CHAINGUN;
	}else if(Length(pTarget->pos-mEnt.pos)<200.0&&mEnt.iRockets>0&&distToBarrel>4){
		mEnt.nextCommand = OP_SHOOT_ROCKET;
	}else if(Length(pTarget->pos-mEnt.pos)<220&&mEnt.iGrenades>0&&distToBarrel>10){
		mEnt.nextCommand = OP_SHOOT_GRENADE;
	}else{
		mEnt.nextCommand = OP_SHOOT_FIREBALL;
	}
	
	}
	}
	if(distToBarrel<15){
		mEnt.aimDirection=barrelVec;
		mEnt.nextCommand=OP_ATTACK_FIST;
		//DrawLine(mEnt.pos,barrelVec,BLUE,0.f);
	}

	/*if(Length(target-mEnt.pos)<30){
		DrawLine(mEnt.pos,target,WHITE,0.f);
	}else if (Length(target-mEnt.pos)<80){
		DrawLine(mEnt.pos,target,RED,0.f);
	}else if (Length(target-mEnt.pos)<150){
		DrawLine(mEnt.pos,target,GREEN,0.f);
	}else{
		DrawLine(mEnt.pos,target,BLUE,0.f);
	}*/
	
	desire=TYPE_CARRYOBJECT_PICKUP;
	//DrawLine(mEnt.pos,mWorldInfo.pPathNodes[261].vPos,GREEN,0.f);
distToBarrel=10000;
	barrelVec=vec2(0,0);
}




// functions
void DrawAiPaths(const sWorldInfo &mWorldInfo,  void (*DrawLine)(vec2,vec2,vColor,float))
{
	//first find best fit
	vec2 vMin = vec2( 10000,  10000);
	vec2 vMax = vec2(-10000, -10000);

	for(int i=0; i<mWorldInfo.iNumPathNodes; i++)
	{
		sPathNode *pathEnt1 = &mWorldInfo.pPathNodes[i];
		if(pathEnt1->vPos.x < vMin.x)
			vMin.x = pathEnt1->vPos.x;
		if(pathEnt1->vPos.x > vMax.x)
			vMax.x = pathEnt1->vPos.x;

		if(pathEnt1->vPos.y < vMin.y)
			vMin.y = pathEnt1->vPos.y;
		if(pathEnt1->vPos.y > vMax.y)
			vMax.y = pathEnt1->vPos.y;
	}

	float fGridSize = 8.0f;
	//int numX = (vMax.x - vMin.x)/fGridSize;
	//int numY = (vMax.y - vMin.y)/fGridSize;


	//now fill in 2d array of map
	const int XGRID_MAX=70;
	const int YGRID_MAX=70;
	bool pathGrid[XGRID_MAX][YGRID_MAX];
	memset(&pathGrid, 0, sizeof(bool)*XGRID_MAX*YGRID_MAX);
	for(int i=0; i<mWorldInfo.iNumPathNodes; i++)
	{
		sPathNode *pathEnt1 = &mWorldInfo.pPathNodes[i];

		if(pathEnt1->vPos.x>=vMin.x && pathEnt1->vPos.x<=vMax.y &&
			pathEnt1->vPos.y>=vMin.y && pathEnt1->vPos.y<=vMax.y)
		{
			int xPos = (pathEnt1->vPos.x - vMin.x) / fGridSize;
			int yPos = (pathEnt1->vPos.y - vMin.y) / fGridSize;

			pathGrid[xPos][yPos] = true;
		}
	}

	//draw lines along y axis
	for(int x=0; x<XGRID_MAX; x++)
	{
		bool bInLine = false;
		vec2 vLineStart;
		for(int y=0; y<YGRID_MAX; y++)
		{
			if(pathGrid[x][y] && !bInLine)
			{
				vLineStart.x = vMin.x + x*fGridSize;
				vLineStart.y = vMin.y + y*fGridSize;
				bInLine = true;
			}

			if(!pathGrid[x][y] && bInLine)
			{
				vec2 vLineEnd;
				vLineEnd.x = vMin.x + x*fGridSize;
				vLineEnd.y = vMin.y + (y-1)*fGridSize;
				DrawLine(vLineStart, vLineEnd, GREEN, 0.0f);
				bInLine = false;
			}
		}
	}

	//draw lines along x axis
	for(int y=0; y<YGRID_MAX; y++)
	{
		bool bInLine = false;
		vec2 vLineStart;
		for(int x=0; x<XGRID_MAX; x++)
		{
			if(pathGrid[x][y] && !bInLine)
			{
				vLineStart.x = vMin.x + x*fGridSize;
				vLineStart.y = vMin.y + y*fGridSize;
				bInLine = true;
			}

			if(!pathGrid[x][y] && bInLine)
			{
				vec2 vLineEnd;
				vLineEnd.x = vMin.x + (x-1)*fGridSize;
				vLineEnd.y = vMin.y + y*fGridSize;
				DrawLine(vLineStart, vLineEnd, GREEN, 0.0f);
				bInLine = false;
			}
		}
	}

	//look for nodes with no connections
	for(int i=0; i<mWorldInfo.iNumPathNodes; i++)
	{
		sPathNode *pathEnt1 = &mWorldInfo.pPathNodes[i];
		bool bFoundConnection = false;
		for(int j=0; j<4; j++)
		{
			if(pathEnt1->connectingNodeIndex[j]!=-1)
			{
				bFoundConnection = true;
				break;
			}
		}

		if(bFoundConnection)
			continue;

		vec2 vTop = pathEnt1->vPos + vec2(0,4);
		vec2 vBot = pathEnt1->vPos + vec2(0,-4);
		vec2 vRight = pathEnt1->vPos + vec2(4,0);
		vec2 vLeft = pathEnt1->vPos+ vec2(-4,0);

		DrawLine(vTop, vBot, GREEN, 0.0f);
		DrawLine(vRight, vLeft, GREEN, 0.0f);
	}


	/*
	//old slow way
	for(int i=0; i<mWorldInfo.iNumPathNodes; i++)
	{
		sPathNode *pathEnt1 = &mWorldInfo.pPathNodes[i];
		for(int j=0; j<4; j++)
		{
			if(pathEnt1->connectingNodeIndex[j]!=-1)
			{
				sPathNode *pathEnt2 = &mWorldInfo.pPathNodes[pathEnt1->connectingNodeIndex[j]];
				//DrawLine(pathEnt1->vPos, pathEnt2->vPos, GREEN, 0.0f); 
			}
		}
	}
	*/
}

vec2 PathFinding(vec2 currPos,vec2 targetPos,const sWorldInfo &mWorldInfo){
	
	int currNode;
	int targetNode;
	int bestNum=80;
	int bestNode=-2;
	int visited[500];
	//std::cout<<"BestNode:"<<bestNode<<std::endl;
	//std::cout<<mWorldInfo.iNumPathNodes;
	{	
		float distC=100;
		float distT=100;
		for (int i=0; i<mWorldInfo.iNumPathNodes;i++){
			visited[i]=bestNum+1;
			float newDistC=Length(mWorldInfo.pPathNodes[i].vPos-currPos);
			float newDistT=Length(mWorldInfo.pPathNodes[i].vPos-targetPos);
			if(newDistC<distC){
				distC=newDistC;
				currNode=i;
			}
			if(newDistT<distT){
				distT=newDistT;
				targetNode=i;
			}
		}
	}
	if(targetNode==currNode){
		bestNode=targetNode;
		return mWorldInfo.pPathNodes[bestNode].vPos;
	}
	for (int i=0;i<4;i++){
		/*bool barrelNode=false;
		for(int j=0;j<8;j++){
			if(mWorldInfo.pOtherEnts[j].type==TYPE_MODEL){
				if(abs(Length(mWorldInfo.pOtherEnts[j].pos-mWorldInfo.pPathNodes[mWorldInfo.pPathNodes[currNode].connectingNodeIndex[i]].vPos))<4){
					barrelNode=true;
					break;
				}
			}
		}
		if(barrelNode==true){
			barrelNode=false;
			continue;
		}
		*/
		if((mWorldInfo.pPathNodes[currNode].connectingNodeIndex[i]==targetNode)){
			//return mWorldInfo.pPathNodes[currNode].connectingNodeIndex[targetNode];
			bestNode=targetNode;
			return mWorldInfo.pPathNodes[bestNode].vPos;
		}
		if(mWorldInfo.pPathNodes[currNode].connectingNodeIndex[i]!=-1){
			visited[mWorldInfo.pPathNodes[currNode].connectingNodeIndex[i]]=1;
			//std::cout<<"ENTERED"<<std::endl;
			recPathing(currNode,mWorldInfo.pPathNodes[currNode].connectingNodeIndex[i],mWorldInfo,bestNum,1,targetNode,mWorldInfo.pPathNodes[currNode].connectingNodeIndex[i],bestNode,visited);
		}	
	}
	//std::cout<<"Best Node:"<<bestNode<<std::endl;
	//std::cout<<"BestNode2:"<<bestNode<<std::endl;
	if(bestNode==-2){
		bestNode=currNode;
	}
	return mWorldInfo.pPathNodes[bestNode].vPos;
	
}

void recPathing(int prevNode,int nextNode,const sWorldInfo &mWorldInfo, int &bestnum, int curTurns, int target,int initNode,int &bestNode,int visited[500]){
	int turnNum=curTurns;
	turnNum++;
	//std::cout<<prevNode<<std::endl;
	//std::cout<<nextNode<<std::endl;

	if((turnNum>=bestnum))
    {return;}
	
	//std::cout<<"BestNum:"<<bestnum<<std::endl;

	

	for (int i=0; i<4;i++){
		
		//DrawLine(mWorldInfo.pPathNodes[nextNode].vPos,mWorldInfo.pPathNodes[mWorldInfo.pPathNodes[nextNode].connectingNodeIndex[i]].vPos,WHITE,0.f);

		if(mWorldInfo.pPathNodes[nextNode].connectingNodeIndex[i]==target){
			bestnum=turnNum;
			bestNode=initNode;
			//std::cout<<"FOUND!!!"<<bestnum<<std::endl;
			break;
		}
		
		/*		bool barrelNode=false;
		for(int j=0;j<8;j++){
			if(mWorldInfo.pOtherEnts[j].type==TYPE_MODEL){
				if(abs(Length(mWorldInfo.pOtherEnts[j].pos-mWorldInfo.pPathNodes[mWorldInfo.pPathNodes[nextNode].connectingNodeIndex[i]].vPos))<4){
					barrelNode=true;
					break;
				}
			}
		}
		if(barrelNode==true){
			barrelNode=false;
			continue;
		}*/

		/*if(bestNode>-1){
			return;
		}*/
	if((mWorldInfo.pPathNodes[nextNode].connectingNodeIndex[i]!=-1)
		&&visited[mWorldInfo.pPathNodes[nextNode].connectingNodeIndex[i]]>turnNum){
		//	std::cout<<"ENTERED"<<turnNum<<std::endl;
		//std::cout<<"PREV:"<<prevNode<<std::endl;
		//std::cout<<"NEXT:"<<mWorldInfo.pPathNodes[nextNode].connectingNodeIndex[i]<<std::endl;
		visited[mWorldInfo.pPathNodes[nextNode].connectingNodeIndex[i]]=turnNum;
		recPathing(nextNode,mWorldInfo.pPathNodes[nextNode].connectingNodeIndex[i],mWorldInfo,bestnum,turnNum,target,initNode,bestNode,visited);
	}
	}
	return;
}