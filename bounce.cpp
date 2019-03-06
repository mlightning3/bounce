/*
 * Bounce
 *
 * A simple Boing like demo
 *
 * Matt Kohls
 * 2019
 *
 *
 */

// Amiga includes
#include <proto/intuition.h>
#include <proto/gadtools.h>
#include <proto/graphics.h>
#include <proto/layers.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <intuition/intuition.h>

#include <stdint.h>
#include <stdlib.h>

// Constants
const uint16_t SCREEN_WIDTH = 100;
const uint16_t SCREEN_HEIGHT = 100;
const uint16_t POSX = 10; 
const uint16_t POSY = 10;

/**
 * WindowPos
 * Keeps track of window position
 */
struct WindowPos {
	uint16_t posx;
	uint16_t posy;
};

/**
 * WindowSize
 * Keeps track of window size
 */
struct WindowSize {
	uint16_t width;
	uint16_t height;
};

/**
 * BallPos
 * Keeps track of ball position
 */
struct BallPos {
	uint16_t posx;
	uint16_t posy;
};

/**
 * AmigaWindow
 * Wrapper class for Amgia functions
 *
 */
class AmigaWindow {
	private:
		struct BallPos ballpos;
		int bounceDirX;
		int bounceDirY;

		struct WindowPos winposition;
		struct WindowSize winsize;
		struct Window *window;
		struct RastPort *rp;
		struct IntuiMessage *msg;
		ULONG msgClass;
		bool bad;

		/**
		 * Initialize the window
		 */
		void init() {
			bad = false;
			window = OpenWindowTags(NULL,
					WA_Left, winposition.posx, WA_Top, winposition.posy,
					WA_Width, winsize.width, WA_Height, winsize.height,
					WA_IDCMP, IDCMP_CLOSEWINDOW | IDCMP_REFRESHWINDOW,
					WA_Flags, WFLG_SIZEGADGET | WFLG_DRAGBAR | WFLG_DEPTHGADGET | 
					WFLG_CLOSEGADGET | WFLG_ACTIVATE | WFLG_SMART_REFRESH,
					WA_Title, "Bounce", WA_PubScreenName, "Workbench", TAG_DONE);
			if(window == nullptr) {
				bad = true;
			} else {
				rp = window->RPort;
				if(rp == nullptr) {
					bad = true;
				}
			}
		}

		/**
		 * Draws the bouncing ball on screen
		 */
		void drawBounce() {
			BallPos oldPos = ballpos;
			if(ballpos.posx == 10) {
				bounceDirX = 1;
			} else if(ballpos.posx == winsize.width - 10) {
				bounceDirX = -1;
			}
			if(ballpos.posy == 10) {
				bounceDirY = 1;
			} else if(ballpos.posy == winsize.height - 10) {
				bounceDirY = -1;
			}

			ballpos.posx += bounceDirX;
			ballpos.posy += bounceDirY;

			SetAPen(rp, (ULONG) 0);
			DrawEllipse(rp, oldPos.posx + 1, oldPos.posy + 1, 5, 5);
			DrawEllipse(rp, oldPos.posx, oldPos.posy, 5, 5);

			SetAPen(rp, (ULONG) 1);
			DrawEllipse(rp, ballpos.posx + 1, ballpos.posy + 1, 5, 5);

			SetAPen(rp, (ULONG) 2);
			DrawEllipse(rp, ballpos.posx, ballpos.posy, 5, 5);
		
		}

	public:
		/**
		 * Creates an empty window of a default size and position
		 */
		AmigaWindow() {
			winposition = {POSX, POSY};
			winsize = {SCREEN_WIDTH, SCREEN_HEIGHT};
			init();
		}

		/**
		 * Creates a new window of specific size and position
		 * @param pos Position of window
		 * @param size Size of window
		 */
		AmigaWindow(const WindowPos& pos, const WindowSize& size) {
			winposition = pos;
			winsize = size;
			init();
		}

		/**
		 * Cleans up window
		 */
		~AmigaWindow() {
			if(window != nullptr) {
				CloseWindow(window);
				window = nullptr;
			}
		}

		/**
		 * Window loop for program
		 */
		void runLoop() {
			ballpos = {15, 20}; // Temp for now
			bounceDirX = 1; // Temp for now
			bounceDirY = -1; // Temp for now
			
			drawBounce();
			bool stop = false;
			while(!bad && !stop) {
				// If there is a message in the queue, handle it
				if(msg = GT_GetIMsg(window->UserPort)) {
					msgClass = msg->Class;
					GT_ReplyIMsg(msg);
					if(msgClass == IDCMP_CLOSEWINDOW) {
						CloseWindow(window);
						stop = true;
						window = nullptr;
					} else if(msgClass == IDCMP_REFRESHWINDOW) {
						RefreshWindowFrame(window);
					}
				}
				drawBounce();
				Delay(10);
			}
		}

		/**
		 * Tells us if there is an error with the window
		 * @return True if there is an error, False if not
		 */
		bool isBad() const { return bad; }
};

int main(int argc, char** argv) {
	AmigaWindow amigaWindow;
	if(amigaWindow.isBad()) { return -1; }
	amigaWindow.runLoop();
	if(amigaWindow.isBad()) { return -1; }
	return 0;
}
