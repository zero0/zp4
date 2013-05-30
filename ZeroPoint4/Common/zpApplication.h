#pragma once
#ifndef ZP_APPLICATION_H
#define ZP_APPLICATION_H

class zpApplication
{
public:
	zpApplication();
	~zpApplication();

	void initualize();
	void run();
	void shutdown();

private:
	zpWindow m_window;
};

#endif