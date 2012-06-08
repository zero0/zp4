#pragma once
#ifndef ZP_WINDOW_H
#define ZP_WINDOW_H

enum zpWindowStyle : zp_uint {
	ZP_WINDOW_STYLE_MINIMIZE =		0x0001,
	ZP_WINDOW_STYLE_MAXIMIZE =		0x0002,

	ZP_WINDOW_STYLE_NO_RESIZE =		0x0010,
	ZP_WINDOW_STYLE_RESIZE =		0x0020,

	ZP_WINDOW_STYLE_BORDERLESS =	0x0200,

	ZP_WINDOW_STYLE_DEFAULT =		0,
};

ZP_PURE_INTERFACE zpWindowFocusListener {
	virtual void onFocusGained() = 0;
	virtual void onFocusLost() = 0;
};

ZP_PURE_INTERFACE zpWindowProcListener {
	virtual void onWindowProc( zp_uint uMessage, zp_uint wParam, zp_ulong lParam ) = 0;
};

class zpWindow {
public:
	zpWindow();
	~zpWindow();

	void setTitle( const zpString& title );
	const zpString& getTitle() const;

	void setPosition( const zpVector2i& position );
	const zpVector2i& getPosition() const;

	void setScreenSize( const zpVector2i& size );
	const zpVector2i& getScreenSize() const;

	const zpVector2i& getWindowSize() const;

	void setStyle( zp_uint style );
	zp_uint getStyle() const;

	zp_ptr getWindowHandle() const;
	zp_ptr getInstanceHandle() const;

	void create();
	void destroy();

	zp_bool isCreated() const;

	void run();

	void windowProc( zp_uint uMessage, zp_uint wParam, zp_ulong lParam );

	void addFocusListener( zpWindowFocusListener* listener );
	void removeFocusListener( zpWindowFocusListener* listener );
	void removeAllFocusListeners();

	void addProcListener( zpWindowProcListener* listener );
	void removeProcListener( zpWindowProcListener* listener );
	void removeAllProcListeners();

private:
	typedef zpLinkedList<zpWindowFocusListener*> zpWindowFocusListenerList;
	typedef zpLinkedList<zpWindowProcListener*> zpWindowProcListenerList;

	void moveResize();
	void resizeWindow();

	zpString m_title;

	zpVector2i m_position;
	zpVector2i m_screenSize;
	zpVector2i m_windowSize;

	zp_uint m_style;

	zp_ptr m_hWnd;
	zp_ptr m_hInstance;

	zpWindowFocusListenerList m_focusListeners;
	zpWindowProcListenerList m_procListeners;
};

#endif