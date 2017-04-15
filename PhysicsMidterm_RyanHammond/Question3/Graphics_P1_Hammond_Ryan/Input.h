#ifndef _Input_HG_
#define _Input_HG_

#include "global.h"

namespace nInput
{
	namespace IsKeyPressed
	{
		inline bool A() { return glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS; }
		inline bool B() { return glfwGetKey(gWindow, GLFW_KEY_B) == GLFW_PRESS; }
		inline bool C() { return glfwGetKey(gWindow, GLFW_KEY_C) == GLFW_PRESS; }
		inline bool D() { return glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS; }
		inline bool E() { return glfwGetKey(gWindow, GLFW_KEY_E) == GLFW_PRESS; }
		inline bool F() { return glfwGetKey(gWindow, GLFW_KEY_F) == GLFW_PRESS; }
		inline bool G() { return glfwGetKey(gWindow, GLFW_KEY_G) == GLFW_PRESS; }
		inline bool H() { return glfwGetKey(gWindow, GLFW_KEY_H) == GLFW_PRESS; }
		inline bool I() { return glfwGetKey(gWindow, GLFW_KEY_I) == GLFW_PRESS; }
		inline bool J() { return glfwGetKey(gWindow, GLFW_KEY_J) == GLFW_PRESS; }
		inline bool K() { return glfwGetKey(gWindow, GLFW_KEY_K) == GLFW_PRESS; }
		inline bool L() { return glfwGetKey(gWindow, GLFW_KEY_L) == GLFW_PRESS; }
		inline bool M() { return glfwGetKey(gWindow, GLFW_KEY_M) == GLFW_PRESS; }
		inline bool N() { return glfwGetKey(gWindow, GLFW_KEY_N) == GLFW_PRESS; }
		inline bool O() { return glfwGetKey(gWindow, GLFW_KEY_O) == GLFW_PRESS; }
		inline bool P() { return glfwGetKey(gWindow, GLFW_KEY_P) == GLFW_PRESS; }
		inline bool Q() { return glfwGetKey(gWindow, GLFW_KEY_Q) == GLFW_PRESS; }
		inline bool R() { return glfwGetKey(gWindow, GLFW_KEY_R) == GLFW_PRESS; }
		inline bool S() { return glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS; }
		inline bool T() { return glfwGetKey(gWindow, GLFW_KEY_T) == GLFW_PRESS; }
		inline bool U() { return glfwGetKey(gWindow, GLFW_KEY_U) == GLFW_PRESS; }
		inline bool V() { return glfwGetKey(gWindow, GLFW_KEY_V) == GLFW_PRESS; }
		inline bool W() { return glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS; }
		inline bool X() { return glfwGetKey(gWindow, GLFW_KEY_X) == GLFW_PRESS; }
		inline bool Y() { return glfwGetKey(gWindow, GLFW_KEY_Y) == GLFW_PRESS; }
		inline bool Z() { return glfwGetKey(gWindow, GLFW_KEY_Z) == GLFW_PRESS; }

		inline bool UP() { return glfwGetKey(gWindow, GLFW_KEY_UP) == GLFW_PRESS; }
		inline bool DOWN() { return glfwGetKey(gWindow, GLFW_KEY_DOWN) == GLFW_PRESS; }
		inline bool LEFT() { return glfwGetKey(gWindow, GLFW_KEY_LEFT) == GLFW_PRESS; }
		inline bool RIGHT() { return glfwGetKey(gWindow, GLFW_KEY_RIGHT) == GLFW_PRESS; }
		

		inline bool NUM0() { return glfwGetKey(gWindow, GLFW_KEY_0) == GLFW_PRESS; }
		inline bool NUM1() { return glfwGetKey(gWindow, GLFW_KEY_1) == GLFW_PRESS; }
		inline bool NUM2() { return glfwGetKey(gWindow, GLFW_KEY_2) == GLFW_PRESS; }
		inline bool NUM3() { return glfwGetKey(gWindow, GLFW_KEY_3) == GLFW_PRESS; }
		inline bool NUM4() { return glfwGetKey(gWindow, GLFW_KEY_4) == GLFW_PRESS; }
		inline bool NUM5() { return glfwGetKey(gWindow, GLFW_KEY_5) == GLFW_PRESS; }
		inline bool NUM6() { return glfwGetKey(gWindow, GLFW_KEY_6) == GLFW_PRESS; }
		inline bool NUM7() { return glfwGetKey(gWindow, GLFW_KEY_7) == GLFW_PRESS; }
		inline bool NUM8() { return glfwGetKey(gWindow, GLFW_KEY_8) == GLFW_PRESS; }
		inline bool NUM9() { return glfwGetKey(gWindow, GLFW_KEY_9) == GLFW_PRESS; }

		inline bool ESCAPE() { return glfwGetKey(gWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS; }
		inline bool ENTER() { return glfwGetKey(gWindow, GLFW_KEY_ENTER) == GLFW_PRESS; }
		inline bool TAB() { return glfwGetKey(gWindow, GLFW_KEY_TAB) == GLFW_PRESS; }

		inline bool F1() { return glfwGetKey(gWindow, GLFW_KEY_F1) == GLFW_PRESS; }
		inline bool F2() { return glfwGetKey(gWindow, GLFW_KEY_F2) == GLFW_PRESS; }
		inline bool F3() { return glfwGetKey(gWindow, GLFW_KEY_F3) == GLFW_PRESS; }
		inline bool F4() { return glfwGetKey(gWindow, GLFW_KEY_F4) == GLFW_PRESS; }
		inline bool F5() { return glfwGetKey(gWindow, GLFW_KEY_F5) == GLFW_PRESS; }
		inline bool F6() { return glfwGetKey(gWindow, GLFW_KEY_F6) == GLFW_PRESS; }
		inline bool F7() { return glfwGetKey(gWindow, GLFW_KEY_F7) == GLFW_PRESS; }
		inline bool F8() { return glfwGetKey(gWindow, GLFW_KEY_F8) == GLFW_PRESS; }
		inline bool F9() { return glfwGetKey(gWindow, GLFW_KEY_F9) == GLFW_PRESS; }
		inline bool F10() { return glfwGetKey(gWindow, GLFW_KEY_F10) == GLFW_PRESS; }
		inline bool F11() { return glfwGetKey(gWindow, GLFW_KEY_F11) == GLFW_PRESS; }
		inline bool F12() { return glfwGetKey(gWindow, GLFW_KEY_F12) == GLFW_PRESS; }

		inline bool KEYPAD0() { return glfwGetKey(gWindow, GLFW_KEY_KP_0) == GLFW_PRESS; }
		inline bool KEYPAD1() { return glfwGetKey(gWindow, GLFW_KEY_KP_1) == GLFW_PRESS; }
		inline bool KEYPAD2() { return glfwGetKey(gWindow, GLFW_KEY_KP_2) == GLFW_PRESS; }
		inline bool KEYPAD3() { return glfwGetKey(gWindow, GLFW_KEY_KP_3) == GLFW_PRESS; }
		inline bool KEYPAD4() { return glfwGetKey(gWindow, GLFW_KEY_KP_4) == GLFW_PRESS; }
		inline bool KEYPAD5() { return glfwGetKey(gWindow, GLFW_KEY_KP_5) == GLFW_PRESS; }
		inline bool KEYPAD6() { return glfwGetKey(gWindow, GLFW_KEY_KP_6) == GLFW_PRESS; }
		inline bool KEYPAD7() { return glfwGetKey(gWindow, GLFW_KEY_KP_7) == GLFW_PRESS; }
		inline bool KEYPAD8() { return glfwGetKey(gWindow, GLFW_KEY_KP_8) == GLFW_PRESS; }
		inline bool KEYPAD9() { return glfwGetKey(gWindow, GLFW_KEY_KP_9) == GLFW_PRESS; }
		inline bool KEYPADENTER() { return glfwGetKey(gWindow, GLFW_KEY_KP_ENTER) == GLFW_PRESS; }


		inline bool LEFTSHIFT() { return glfwGetKey(gWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS; }
		inline bool RIGHTSHIFT() { return glfwGetKey(gWindow, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS; }
		inline bool LEFTCONTROL() { return glfwGetKey(gWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS; }
		inline bool RIGHTCONTROL() { return glfwGetKey(gWindow, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS; }
		inline bool LEFTALT() { return glfwGetKey(gWindow, GLFW_KEY_LEFT_ALT) == GLFW_PRESS; }
		inline bool RIGHTALT() { return glfwGetKey(gWindow, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS; }


		inline bool MOUSEBUTTONRIGHT() { return glfwGetKey(gWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS; }
		inline bool MOUSEBUTTONLEFT() { return glfwGetKey(gWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS; }
		inline bool MOUSEBUTTONMIDDLE() { return glfwGetKey(gWindow, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS; }
		
	}
}


#endif