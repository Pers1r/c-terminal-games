//
// Created by Marek on 11.12.2025.
//

#ifndef GAME_INPUTHADLER_H
#define GAME_INPUTHADLER_H

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#endif


class InputHandler {
public:
	InputHandler() {
#ifndef _WIN32
		// Linux/Mac: Turn off canonical mode to read input immediately
		struct termios t;
		tcgetattr(STDIN_FILENO, &t);
		t.c_lflag &= ~ICANON; // Disable buffering (wait for Enter)
		t.c_lflag &= ~ECHO;   // Disable echoing input to screen
		tcsetattr(STDIN_FILENO, TCSANOW, &t);

		// Set non-blocking
		int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
		fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
#endif
	}

	~InputHandler() {
#ifndef _WIN32
		// Restore terminal settings on exit
		struct termios t;
		tcgetattr(STDIN_FILENO, &t);
		t.c_lflag |= ICANON;
		t.c_lflag |= ECHO;
		tcsetattr(STDIN_FILENO, TCSANOW, &t);
#endif
	}

	bool kbhit() {
#ifdef _WIN32
		return _kbhit();
#else
		struct timeval tv = { 0L, 0L };
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(STDIN_FILENO, &fds);
		return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
#endif
	}

	char getch() {
#ifdef _WIN32
		return _getch();
#else
		char c;
		if (read(STDIN_FILENO, &c, 1) < 0) return 0;
		return c;
#endif
	}
};

#endif //GAME_INPUTHADLER_H