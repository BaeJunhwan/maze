/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.
	
	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

	http://www.spout.zeal.co

    =========================================================================
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    =========================================================================

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isdfs = false;
	isbfs = false;
	isOpen = 0;

	dfsButton.set(70, 25, 45, 25);
	bfsButton.set(120, 25, 45, 25);
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth() - windowWidth) / 2, (ofGetScreenHeight() - windowHeight) / 2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist.
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked

	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true; // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS", false, false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup

//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
//
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if (title == "Open") {
		readFile();
	}
	if (title == "Exit") {
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if (title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen) {
			int start = 0;
			int target = maze_row * maze_col - 1;

			isbfs = false;
			isdfs = DFS(start, target);
			menu->SetPopupItem("Show DFS", true);
			menu->SetPopupItem("Show BFS", false);

			if (!isdfs) {
				cout << "DFS path not found" << endl;
			}

		} else
			cout << "you must open file first" << endl;
	}

	if (title == "Show BFS") {
		//doTopmost(bChecked); // Use the checked value directly
		if (isOpen) {
			int start = 0;
			int target = maze_row * maze_col - 1;
			isdfs = false;
			isbfs = BFS(start, target);
			menu->SetPopupItem("Show DFS", false);
			menu->SetPopupItem("Show BFS", true);

			if (!isbfs) {
				cout << "BFS path not found" << endl;
			} 

		} else
			cout << "you must open file first" << endl;
	}

	if (title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if (title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;

	drawToolbarButtons();
	// TO DO : DRAW MAZE;
	// 저장된 자료구조를 이용해 미로를 그린다.
	// add code here

	for (i = 0; i < mazeLines.size(); i++) {
		for (j = 0; j < mazeLines[i].length(); j++) {
			char ch = mazeLines[i][j];
			int x = startX + j * unit;
			int y = startY + i * unit;
			
			if (ch == '+') {
				ofDrawRectangle(x-thickness/2, y-thickness/2, thickness, thickness);
			}
			else if (ch == '-') {
				ofDrawRectangle(x-unit, y - thickness / 2, 2*unit, thickness);
			}
			else if (ch == '|') {
				ofDrawRectangle(x - thickness / 2, y-unit, thickness, 2*unit);
			}

		}
	}

	if (isdfs) {;
		bShowInfo = true;
		if (isOpen)
			dfsdraw();
		else
			cout << "You must open file first" << endl;
	}

	if (isbfs) {
		bShowInfo = true;
		if (isOpen)
			bfsdraw();
		else
			cout << "You must open file first" << endl;
	}

	if (bShowInfo) {
		// Show keyboard duplicates of menu functions
		ofSetColor(100);
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight() - 20);
	}

} // end Draw

void ofApp::doFullScreen(bool bFull) {
	// Enter full screen
	if (bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	} else {
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU));
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth() - ofGetWidth()) / 2, (ofGetScreenHeight() - ofGetHeight()) / 2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if (bTopmost) doTopmost(true);
	}

} // end doFullScreen

void ofApp::doTopmost(bool bTop) {
	if (bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
	} else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if (GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
} // end doTopmost

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	// Escape key exit has been disabled but it can be checked here
	if (key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if (bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		} else {
			ofExit();
		}
	}

	// Remove or show screen info
	if (key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if (key == 'f') {
		bFullscreen = !bFullscreen;
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	if (dfsButton.inside(x, y)) {
		if (isOpen) {
			int start = 0;
			int target = maze_row * maze_col - 1;

			isbfs = false;
			isdfs = DFS(start, target);

			menu->SetPopupItem("Show DFS", isdfs);
			menu->SetPopupItem("Show BFS", false);

			if (!isdfs) {
				cout << "DFS path not found" << endl;
			}
		} else {
			cout << "you must open file first" << endl;
		}
	}

	if (bfsButton.inside(x, y)) {
		if (isOpen) {
			int start = 0;
			int target = maze_row * maze_col - 1;

			isdfs = false;
			isbfs = BFS(start, target);

			menu->SetPopupItem("Show DFS", false);
			menu->SetPopupItem("Show BFS", isbfs);

			if (!isbfs) {
				cout << "BFS path not found" << endl;
			}
		} else {
			cout << "you must open file first" << endl;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
}
bool ofApp::readFile() {
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		//string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		
		filePath = openFileResult.getPath();
		cout << "file name is " << filePath << endl;
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(filePath);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			} else {
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			mazeLines.clear();
			ofBuffer buffer(file);

			// Input_flag is a variable for indication the type of input.
			// If input_flag is zero, then work of line input is progress.
			// If input_flag is one, then work of dot input is progress.
			int input_flag = 0;

			// Idx is a variable for index of array.
			int idx = 0;

			// Read file line by line
			int cnt = 0;

			// TO DO
			// .maz 파일을 input으로 받아서 적절히 자료구조에 넣는다

			for (auto line : buffer.getLines()) {

				mazeLines.push_back(line);
			}

			maze_row = (mazeLines.size() - 1) / 2;
			maze_col = (mazeLines[0].length() - 1) / 2;

			adjList.clear();
			adjList.resize(maze_row * maze_col);
			visited = new int[maze_row * maze_col];
			parent = new int[maze_row * maze_col];

			for (int i = 0; i < maze_row; i++) {
				for (int j = 0; j < maze_col; j++) {
					idx = i * maze_col + j;
					int textR = 2 * i + 1;
					int textC = 2 * j + 1;

					if (j + 1 < maze_col && mazeLines[textR][textC + 1] == ' ') {
						adjList[idx].push_back(idx + 1);
						adjList[idx + 1].push_back(idx);
					}

					if (i + 1 < maze_row && mazeLines[textR + 1][textC] == ' ') {
						adjList[idx].push_back(idx + maze_col);
						adjList[idx + maze_col].push_back(idx);

					}
				}
			}
			return true;
		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
	return false;
}
void ofApp::freeMemory() {

	//TO DO
	// malloc한 memory를 free해주는 함수

	mazeLines.clear();
	adjList.clear();
	finalPath.clear();

	if (visited != nullptr) {
		delete[] visited;
		visited = nullptr;
	}

	if (parent != nullptr) {
		delete[] parent;
		parent = nullptr;
	}


	maze_row = 0;
	maze_col = 0;
	isOpen = 0;

	
}

bool ofApp::DFS(int v, int target) //DFS탐색을 하는 함수
{
	//TO DO
	//DFS탐색을 하는 함수 ( 3주차)
	stack<int> S;

	finalPath.clear();
	visitedEdge.clear();

	for (int i = 0; i < maze_row * maze_col; i++) {
		visited[i] = 0;
		parent[i] = -1;
	}

	S.push(v);
	visited[v] = true;
	parent[v] = -1;


	while (!S.empty()) {
		int current = S.top();
		visited[current] = true;
		if (current == target) {
			int x = target;
			while (x != -1) {
				finalPath.push_back(x);
				x = parent[x];
			}
			reverse(finalPath.begin(), finalPath.end());
			return true;
		}	

		bool move = false;

		for (int j = 0; j < adjList[current].size(); j++) {
			int next = adjList[current][j];

			if (!visited[next]) {
				S.push(next);
				visited[next] = true;
				move = true;
				visitedEdge.push_back({ current, next });
				
				parent[next] = current;
				break;
			}
		}

		if (!move) {
			S.pop();
		}
	}

	return false;
	
}
void ofApp::dfsdraw() {
	//TO DO
	//DFS를 수행한 결과를 그린다. (3주차 내용)
	ofSetColor(180, 180, 180);
	ofSetLineWidth(4);
	drawEdge(visitedEdge);

	ofSetColor(255, 0, 0);
	ofSetLineWidth(6);
	drawPath(finalPath);

	drawStartEnd();

}

bool ofApp::BFS(int v, int target) {
	queue<int> Q;

	finalPath.clear();
	visitedEdge.clear();

	for (int i = 0; i < maze_row * maze_col; i++) {
		visited[i] = 0;
		parent[i] = -1;
	}

	Q.push(v);
	visited[v] = true;
	parent[v] = -1;

	if (v == target) {
		finalPath.push_back(v);
		return true;
	}

	while (!Q.empty()) {
		int current = Q.front();
		Q.pop();

		for (int j = 0; j < adjList[current].size(); j++) {
			int next = adjList[current][j];

			if (!visited[next]) {
				visited[next] = true;
				parent[next] = current;

				visitedEdge.push_back({ current, next });

				if (next == target) {
					int x = target;

					while (x != -1) {
						finalPath.push_back(x);
						x = parent[x];
					}

					reverse(finalPath.begin(), finalPath.end());
					return true;
				}

				Q.push(next);
			}
		}
	}

	return false;
}


void ofApp::bfsdraw() {
	//TO DO
	//BFS를 수행한 결과를 그린다. (3주차 내용)
	ofSetColor(180, 180, 180);
	ofSetLineWidth(4);
	drawEdge(visitedEdge);

	ofSetColor(255, 0, 0);
	ofSetLineWidth(6);
	drawPath(finalPath);

	drawStartEnd();

}

void ofApp::drawPath(vector<int>& path) {
	for (int i = 0; i+1 < path.size(); i++) {
		int a = path[i];
		int b = path[i + 1];

		int r1 = a / maze_col;
		int c1 = a % maze_col;

		int r2 = b / maze_col;
		int c2 = b % maze_col;

		float x1 = startX + (2 * c1 + 1) * unit;
		float y1 = startY + (2 * r1 + 1 ) * unit;
		float x2 = startX + (2 * c2 + 1) * unit;
		float y2 = startY + (2 * r2 + 1) * unit;

		ofDrawLine(x1, y1, x2, y2);

		
	}
}

void ofApp::drawEdge(vector<pair<int, int>>& edge) {
	for (int i = 0; i < edge.size(); i++) {
		int a = edge[i].first;
		int b = edge[i].second;

		int r1 = a / maze_col;
		int c1 = a % maze_col;

		int r2 = b / maze_col;
		int c2 = b % maze_col;

		float x1 = startX + (2 * c1 + 1) * unit;
		float y1 = startY + (2 * r1 + 1) * unit;

		float x2 = startX + (2 * c2 + 1) * unit;
		float y2 = startY + (2 * r2 + 1) * unit;

		ofDrawLine(x1, y1, x2, y2);
	}
}

void ofApp::drawStartEnd() {
	int start = 0;
	int target = maze_row * maze_col - 1;

	int sr = start / maze_col;
	int sc = start % maze_col;

	int tr = target / maze_col;
	int tc = target % maze_col;

	float sx = startX + (2 * sc + 1) * unit;
	float sy = startY + (2 * sr + 1) * unit;

	float tx = startX + (2 * tc + 1) * unit;
	float ty = startY + (2 * tr + 1) * unit;

	float boxSize = unit * 0.8;

	ofSetColor(0, 255, 0);
	ofDrawRectangle(sx - boxSize / 2, sy - boxSize / 2, boxSize, boxSize);

	ofSetColor(0, 0, 255);
	ofDrawRectangle(tx - boxSize / 2, ty - boxSize / 2, boxSize, boxSize);
}


void ofApp::drawToolbarButtons() {

	if (isdfs) {
		ofSetColor(180, 220, 255);
	} else {
		ofSetColor(230);
	}
	ofDrawRectangle(dfsButton);

	ofSetColor(0);
	myFont.drawString("DFS", dfsButton.x + 10, dfsButton.y + 17);

	if (isbfs) {
		ofSetColor(180, 220, 255);
	} else {
		ofSetColor(230);
	}
	ofDrawRectangle(bfsButton);

	ofSetColor(0);
	myFont.drawString("BFS", bfsButton.x + 10, bfsButton.y + 17);
}
