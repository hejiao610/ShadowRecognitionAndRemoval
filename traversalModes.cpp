#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "opencv2/opencv_modules.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "stackContours.cpp"

using namespace cv;
using namespace std;

class traversalModes
{
	public:	
	vector <vector<Point>> contours;
	vector <Vec4i> hierarchy;
	int sz=0,maxLevel=0;
	//array storing each contour level
	int *Clevel;
	Stack *s;
	
	traversalModes(vector <vector<Point>> &C,vector <Vec4i> &H,int n)
	{
		contours = C;
		hierarchy = H;
		Clevel = (int *)malloc(n*sizeof(int));
		sz = n;
		memset(Clevel,-1,sz*4);
		s = new Stack(sz);
	}
	
	void contourLevels(void)
	{
		int i = 0, j = 0,level = 0;
		//by default in TREE mode contour 0 is of level 0
		Clevel[0] = 0;
		cout << "Contour : " << i << " Level : " << level << endl;
		while(1)
		{
			//check for a child contour
			//if a child contour exists
			if(hierarchy[i][2] != -1 && Clevel[hierarchy[i][2]] == -1)
			{
				//update the level
				Clevel[hierarchy[i][2]] = ++level;
				//store the maximum level for future use
				if(level > maxLevel)
					maxLevel = level;
				//point to the child contour
				i = hierarchy[i][2];
				cout << "Contour : " << i << " Level : " << level << endl;
				//continue
				continue;
			}
			//if not check for a sibling contour
			if(hierarchy[i][0] != -1 && Clevel[hierarchy[i][0]] == -1)
			{
				//add same level to sibling
				Clevel[hierarchy[i][0]] = level;
				//point to the sibling contour
				i = hierarchy[i][0];
				cout << "Contour : " << i << " Level : " << level << endl;
				//continue
				continue;
			}
			//if there is parent contour
			else if(hierarchy[i][3] != -1)
			{
				//point to the parent contour till there is a sibling contour, update level
				while(hierarchy[i][3] != -1)
				{
					i = hierarchy[i][3];level--;
					if(hierarchy[i][0] != -1 && Clevel[hierarchy[i][0]] == -1)
					{
						//add same level to sibling
						Clevel[hierarchy[i][0]] = level;
						//point to sibling contour
						i = hierarchy[i][0];
						cout << "Contour : " << i << " Level : " << level << endl;
						//continue
						break;
					}
				}
			}
			//if no child, no sibling and no parent
			else
				break;
		}
		cout << " Max Level : " << maxLevel << endl;
	}
	
	//Validate if all the contours are visitied
	void validateVisits(void)
	{
		int i = 0;
		for(i = 0; i < sz;i++)
			if(Clevel[i] == -1)
				cout << "Missed a contour " << endl;
		if(i == sz)
			cout << "Identified levels of all contours successfully" << endl;
	}
	
	//Breadth First Search
	void BFS(void)
	{
		//variables for drawContours
		Mat canvas(650, 1160, CV_8UC3,Scalar(0,0,0));	
		int temp = 0,level = 0,i=0,j=0;
		while(level <= maxLevel)
		{
			for(int i = 0; i < sz;i++)
			{
				if(Clevel[i] == level)
				{
					drawContours(canvas,contours,i,Scalar(255),2,8,hierarchy,0,Point());
					namedWindow("contours", WINDOW_AUTOSIZE);
					imshow("contours",canvas);
					while(1)
					{
						char k = waitKey(33);
						if(k == 27)
							break;
					}
				}
					
			}
			level++;
			
		}
		
	}
	//Depth First Search
	void DFS(void)
	{
		//variables for drawContours
		Mat canvas(650, 1160, CV_8UC3,Scalar(0,0,0));
		int i = 0, j = 0,temp=0;
		while(1)
		{
			if(hierarchy[i][0] != -1)
					s->push(hierarchy[i][0]);
			//check for a child contour
			//if a child contour exists
			if(hierarchy[i][2] != -1)
			{
				//point to the child contour
				i = hierarchy[i][2];
				//Store the next contour in stack 
				if(hierarchy[i][0] != -1)
					s->push(hierarchy[i][0]);
				drawContours(canvas,contours,i,Scalar(255),2,8,hierarchy,0,Point());
				namedWindow("contours", WINDOW_AUTOSIZE);
				imshow("contours",canvas);
				while(1)
				{
					char k = waitKey(33);
					if(k == 27)
						break;
				}
				continue;
			}
			//if not check for a sibling contour
			if(hierarchy[i][0] != -1)
			{
				i = hierarchy[i][0];
				drawContours(canvas,contours,i,Scalar(255),2,8,hierarchy,0,Point());
				namedWindow("contours", WINDOW_AUTOSIZE);
				imshow("contours",canvas);
				while(1)
				{
					char k = waitKey(33);
					if(k == 27)
						break;
				}
				//continue
				continue;
			}
			//if there is parent contour
			else if(!s->empty)
			{
				//point to the sibling contour
				i = s->pop();	
				drawContours(canvas,contours,i,Scalar(255),2,8,hierarchy,0,Point());
				namedWindow("contours", WINDOW_AUTOSIZE);
				imshow("contours",canvas);
				while(1)
				{
					char k = waitKey(33);
					if(k == 27)
						break;
				}
				//continue
				continue;
				//else
					//break;
			}
			//if no child, no sibling and no parent
			else
				break;
		}
		cout << " Max Level : " << maxLevel << endl;
		
	}
	void childToParentDFS(void)
	{
		//variables for drawContours
		Mat canvas(650, 1160, CV_8UC3,Scalar(0,0,0));	
		int temp = 0,level = 0,i=0,j=0;
	}
	void childToParentBFS(void)
	{
		//variables for drawContours
		Mat canvas(650, 1160, CV_8UC3,Scalar(0,0,0));	
		int temp = 0,level = maxLevel,i=0,j=0;
		while(level >= 0)
		{
			for(int i = 0; i < sz;i++)
			{
				if(Clevel[i] == level)
				{
					drawContours(canvas,contours,i,Scalar(255),2,8,hierarchy,0,Point());
					namedWindow("contours", WINDOW_AUTOSIZE);
					imshow("contours",canvas);
					while(1)
					{
						char k = waitKey(33);
						if(k == 27)
							break;
					}
				}
					
			}
			level--;
			
		}
	}
	
};