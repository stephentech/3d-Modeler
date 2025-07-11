
#include <windows.h>
#include <windowsx.h>

#include <common.h>
#include <dxmain.h>

 // the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
							UINT message,
							WPARAM wParam,
							LPARAM lParam);

int WINAPI WinMain( HINSTANCE hInstance,     // handle to current instance
                    HINSTANCE hPrevInstance, // handle to previous instance
                    LPSTR     lpCmdLine,     // pointer to command line
                    int       nCmdShow )     // show state of window
{
    WNDCLASSEX winClass;
    HWND       hwnd;
    MSG        uMsg;
	memset(&uMsg,0,sizeof(uMsg));

	// The winClass structure will hold the information about our
    // new Window class that we plan on creating.
    winClass.lpszClassName = L"MY_WINDOWS_CLASS";
    winClass.cbSize        = sizeof(WNDCLASSEX);
    winClass.style         = CS_HREDRAW | CS_VREDRAW;
    winClass.lpfnWndProc   = (WNDPROC)WindowProc;
    winClass.hInstance     = hInstance;
    winClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    winClass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
    winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    winClass.hbrBackground = CreateSolidBrush(RGB(0xff,0x00,0x00));
    winClass.lpszMenuName  = NULL;
    winClass.cbClsExtra    = 0;
    winClass.cbWndExtra    = 0;

    // Register our new declared window class
    if( RegisterClassEx( &winClass) == 0 )
		return E_FAIL;

    // Create the window!
    hwnd = CreateWindowEx( NULL,
                           L"MY_WINDOWS_CLASS",
                           L"Simple Window",
                           WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                           0,0,
                           WINDOW_WIDTH,WINDOW_HEIGHT,
                           NULL,
                           NULL,
                           hInstance,
                           NULL );
    if( hwnd == NULL )
	    return E_FAIL;

    ShowWindow( hwnd, nCmdShow );
    UpdateWindow( hwnd );

	// Initialize Direct3D
    dxInit( hwnd );

    // The following while loop represents the main message loop
    // for our application. Every time our Window receives a windows
    // message, this loop will detect it, perform some preprocessing
    // on it and then dispatch it to our application's message handler
    // function, WindowProc().
	
	while( uMsg.message != WM_QUIT )
    {
		if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}
		else dxRender();
    }

	// cleanup code
	dxCleanup();
	UnregisterClass( L"MY_WINDOWS_CLASS", winClass.hInstance );
    return uMsg.wParam;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // sort through and find what code to run for the message given
    switch(message)
    {
		case WM_CREATE:
            // handle initialization here!
			break;

		case WM_LBUTTONDOWN:
            // The x/y position of the mouse click is stored in the lParam value
			dxLeftClickDown( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
        break;

		case WM_LBUTTONUP:
            // The x/y position of the mouse click is stored in the lParam value
			dxLeftClickUp( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
        break;

		case WM_RBUTTONDOWN:
            // The x/y position of the mouse click is stored in the lParam value
 			dxRightClickDown( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
        break;

		case WM_RBUTTONUP:
            // The x/y position of the mouse click is stored in the lParam value
 			dxRightClickUp( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
        break;

		case WM_KEYDOWN:
			dxKeyDown(wParam);
		break;

		case WM_KEYUP:
			dxKeyUp(wParam);
		break;

        // this message is read when the window is closed
        case WM_DESTROY:
            // close the application entirely
            PostQuitMessage(0);
            return 0;
			break;
   
		case WM_CLOSE:
			// Kill the application by posting the WM_DESTROY and WM_NCDESTROY messages
			DestroyWindow( hWnd );
			break;
	    
		default:
			// The DefWindowProc() function will process any messages that
			// we didn't bother to catch in the switch statement above.
			return DefWindowProc( hWnd, message, wParam, lParam );
			break;
	}
 }
