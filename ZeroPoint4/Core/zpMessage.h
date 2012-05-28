#pragma once
#ifndef ZP_MESSAGE_H
#define ZP_MESSAGE_H

template<typename MessageData>
class zpMessage {
public:

private:
	zp_uint m_type;
	void* m_sender;
	MessageData m_data;
};

#endif